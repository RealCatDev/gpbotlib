#include "gpbotlib/json.h"

#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

Gp_Json gp_json_number(Gp_Json_Number number) {
  return (Gp_Json){
    .type = GP_JSON_NUMBER,
    .as.number = number
  };
}

Gp_Json gp_json_string(Gp_Json_String string) {
  return (Gp_Json){
    .type = GP_JSON_STRING,
    .as.string = string
  };
}

Gp_Json gp_json_boolean(Gp_Json_Boolean boolean) {
  return (Gp_Json){
    .type = GP_JSON_BOOLEAN,
    .as.boolean = boolean
  };
}

Gp_Json gp_json_array() {
  return (Gp_Json){
    .type = GP_JSON_ARRAY
  };
}

Gp_Json gp_json_object() {
  return (Gp_Json){
    .type = GP_JSON_OBJECT
  };
}

Gp_Json gp_json_null() {
  return (Gp_Json){
    .type = GP_JSON_NULL
  };
}



Gp_Result gp_json_object_insert(Gp_Json *object, Gp_Json_String key, Gp_Json value) {
  if (!object || object->type != GP_JSON_OBJECT || !key.items || !key.count) return GP_INVALID_ARGS;

  Gp_Result result = GP_SUCCESS;
  if ((result = gp_json_object_get(object, key, NULL)) < GP_SUCCESS) return result;
  if (result == GP_TRUE) return GP_DUPLICATE;

  Gp_Json_Object *jObj = &object->as.object;
  if (jObj->count >= jObj->capacity) {
    if (jObj->capacity) jObj->capacity <<= 1;
    else jObj->capacity = (1<<3);

    jObj->keys = realloc(jObj->keys, sizeof(*jObj->keys) * jObj->capacity);
    if (!jObj->keys) return GP_BUY_MORE_RAM;
    jObj->values = realloc(jObj->values, sizeof(*jObj->values) * jObj->capacity);
    if (!jObj->values) return GP_BUY_MORE_RAM;
  }

  jObj->keys[jObj->count] = key;
  jObj->values[jObj->count++] = value;

  return GP_SUCCESS;
}

Gp_Result gp_json_object_get(Gp_Json *object, Gp_Json_String key, Gp_Json **value) {
  if (!object || object->type != GP_JSON_OBJECT || !key.items || !key.count) return GP_INVALID_ARGS;

  for (size_t i = 0; i < object->as.object.count; ++i) {
    Gp_Json_String k = object->as.object.keys[i];
    if (!gp_string_view_is_eq(key, k)) continue;

    if (value) *value = &object->as.object.values[i];
    return GP_TRUE;
  }

  return GP_SUCCESS;
}

Gp_Result gp_json_array_append(Gp_Json *array, Gp_Json value) {
  if (!array || array->type != GP_JSON_ARRAY) return GP_INVALID_ARGS;

  Gp_Json_Array *jArr = &array->as.array;
  if (jArr->count >= jArr->capacity) {
    if (jArr->capacity) jArr->capacity <<= 1;
    else jArr->capacity = (1<<3);

    jArr->items = realloc(jArr->items, sizeof(*jArr->items) * jArr->capacity);
    if (!jArr->items) return GP_BUY_MORE_RAM;
  }
  if (!jArr->items) return GP_INVALID_ARGS;
  jArr->items[jArr->count++] = value;

  return GP_SUCCESS;
}



Gp_Result gp_string_builder_append_json(Gp_String_Builder *lhs, Gp_Json rhs) {
  if (!lhs) return GP_INVALID_ARGS;

  Gp_Result result = GP_SUCCESS;
  switch (rhs.type) {
  case GP_JSON_NUMBER: {
    return gp_string_builder_append_float(lhs, rhs.as.number);
  } break;
  case GP_JSON_STRING: {
    if ((result = gp_string_builder_append_cstr(lhs, "\"")) < GP_SUCCESS) return result;
    if ((result = gp_string_builder_append_string_view(lhs, rhs.as.string)) < GP_SUCCESS) return result;
    return gp_string_builder_append_cstr(lhs, "\"");
  } break;
  case GP_JSON_BOOLEAN: {
    return gp_string_builder_append_cstr(lhs, rhs.as.boolean?"true":"false");
  } break;
  case GP_JSON_ARRAY: {
    if ((result = gp_string_builder_append_cstr(lhs, "[")) < GP_SUCCESS) return result;

    for (size_t i = 0; i < rhs.as.array.count; ++i) {
      if (i && (result = gp_string_builder_append_cstr(lhs, ",")) < GP_SUCCESS) return result;
      if ((result = gp_string_builder_append_json(lhs, rhs.as.array.items[i])) < GP_SUCCESS) return result;
    }

    return gp_string_builder_append_cstr(lhs, "]");
  } break;
  case GP_JSON_OBJECT: {
    if ((result = gp_string_builder_append_cstr(lhs, "{")) < GP_SUCCESS) return result;

    for (size_t i = 0; i < rhs.as.object.count; ++i) {
      if (i && (result = gp_string_builder_append_cstr(lhs, ",")) < GP_SUCCESS) return result;
      if ((result = gp_string_builder_append_cstr(lhs, "\"")) < GP_SUCCESS) return result;
      if ((result = gp_string_builder_append_string_view(lhs, rhs.as.object.keys[i])) < GP_SUCCESS) return result;
      if ((result = gp_string_builder_append_cstr(lhs, "\":")) < GP_SUCCESS) return result;
      if ((result = gp_string_builder_append_json(lhs, rhs.as.object.values[i])) < GP_SUCCESS) return result;
    }

    return gp_string_builder_append_cstr(lhs, "}");
  } break;
  case GP_JSON_NULL: {
    return gp_string_builder_append_cstr(lhs, "null");
  } break;
  }

  return GP_SUCCESS;
}

Gp_Result gp_json_from_string_view(Gp_Json *json, Gp_String_View *sv) {
  if (!json) return GP_INVALID_ARGS;
  if (!sv->count) return GP_SUCCESS;

  *sv = gp_string_view_trim(*sv);

  Gp_Result result = GP_SUCCESS;
  if (*sv->items == '{') {
    gp_string_view_chop(sv, 1);

    json->type = GP_JSON_OBJECT;
    json->as.object = (Gp_Json_Object){0};

    do {
      *sv = gp_string_view_trim(*sv);

      if (*sv->items != '\"') return GP_INVALID_JSON;
      gp_string_view_chop(sv, 1);

      Gp_String_View key = gp_string_view_chop_delim(sv, '\"');

      *sv = gp_string_view_trim(*sv);
      if (*sv->items != ':') return GP_INVALID_JSON;
      gp_string_view_chop(sv, 1);

      *sv = gp_string_view_trim(*sv);
      Gp_Json value = {0};
      if ((result = gp_json_from_string_view(&value, sv)) < GP_SUCCESS) return result;

      if ((result = gp_json_object_insert(json, key, value)) < GP_SUCCESS) return result;

      *sv = gp_string_view_trim(*sv);
    } while (*sv->items == ',' && (gp_string_view_chop(sv, 1), true));

    if (*sv->items != '}') return GP_INVALID_JSON;
    gp_string_view_chop(sv, 1);
  } else if (*sv->items == '[') {
    gp_string_view_chop(sv, 1);

    json->type = GP_JSON_ARRAY;
    json->as.array = (Gp_Json_Array){0};

    do {
      *sv = gp_string_view_trim(*sv);
      Gp_Json value = {0};
      if ((result = gp_json_from_string_view(&value, sv)) < GP_SUCCESS) return result;

      if ((result = gp_json_array_append(json, value)) < GP_SUCCESS) return result;

      *sv = gp_string_view_trim(*sv);
    } while (*sv->items == ',' && (gp_string_view_chop(sv, 1), true));

    if (*sv->items != ']') return GP_INVALID_JSON;
    gp_string_view_chop(sv, 1);
  } else if (*sv->items == '\"') {
    gp_string_view_chop(sv, 1);
    Gp_String_View name = gp_string_view_chop_delim(sv, '\"');

    json->type = GP_JSON_STRING;
    json->as.string = name;
  } else if (isdigit(*sv->items) || *sv->items == '.' || *sv->items == '-') {
    bool sign = false;
    if (*sv->items == '-') {
      sign = true;
      gp_string_view_chop(sv, 1);
    }

    Gp_String_View intPart = gp_string_view_chop_while(sv, (bool(*)(char))isdigit);

    double number = 0;
    for (const char *ptr = intPart.items, *end = intPart.items+intPart.count; ptr != end; ++ptr)
      number = (number*10) + *ptr - '0';

    json->type = GP_JSON_NUMBER;
    json->as.number = number;

    if (*sv->items == '.') assert(0 && "Unimplemented");
  } else {
    Gp_String_View token = gp_string_view_chop_while(sv, (bool(*)(char))isalnum);
    if (gp_string_view_is_eq(token, GP_STRING_VIEW("true"))) {
      json->type = GP_JSON_BOOLEAN;
      json->as.boolean = true;
    } else if (gp_string_view_is_eq(token, GP_STRING_VIEW("false"))) {
      json->type = GP_JSON_BOOLEAN;
      json->as.boolean = false;
    } else if (gp_string_view_is_eq(token, GP_STRING_VIEW("null"))) {
      json->type = GP_JSON_NULL;
    } else return GP_INVALID_JSON;
  }

  return GP_SUCCESS;
}