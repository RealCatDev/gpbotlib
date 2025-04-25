#ifndef   _GPBOTLIB_JSON_H_
#define   _GPBOTLIB_JSON_H_

#include "gpbotlib/result.h"
#include "gpbotlib/string.h"

#include <stdbool.h>

typedef enum Gp_Json_Type {
  GP_JSON_NUMBER = 0,
  GP_JSON_STRING,
  GP_JSON_BOOLEAN,
  GP_JSON_ARRAY,
  GP_JSON_OBJECT,
  GP_JSON_NULL,

  COUNT_GP_JSON_TYPES
} Gp_Json_Type;

typedef struct Gp_Json Gp_Json;

typedef double         Gp_Json_Number;
typedef Gp_String_View Gp_Json_String;
typedef bool           Gp_Json_Boolean;

typedef struct Gp_Json_Array {
  Gp_Json *items;
  size_t capacity;
  size_t count;
} Gp_Json_Array;

typedef struct Gp_Json_Object {
  Gp_Json_String *keys;
  Gp_Json *values;
  size_t capacity;
  size_t count;
} Gp_Json_Object;

struct Gp_Json {
  Gp_Json_Type type;
  union Gp_Json_As {
    Gp_Json_Number  number;
    Gp_Json_String  string;
    Gp_Json_Boolean boolean;
    Gp_Json_Array   array;
    Gp_Json_Object  object;
  } as;
};

Gp_Json gp_json_number(Gp_Json_Number number);
Gp_Json gp_json_string(Gp_Json_String string);
Gp_Json gp_json_boolean(Gp_Json_Boolean boolean);
Gp_Json gp_json_array();
Gp_Json gp_json_object();
Gp_Json gp_json_null();

Gp_Result gp_json_object_insert(Gp_Json *object, Gp_Json_String key, Gp_Json value);
Gp_Result gp_json_object_get(Gp_Json *object, Gp_Json_String key, Gp_Json **value);

Gp_Result gp_json_array_append(Gp_Json *array, Gp_Json value);

Gp_Result gp_string_builder_append_json(Gp_String_Builder *lhs, Gp_Json rhs);

Gp_Result gp_json_from_string_view(Gp_Json *json, Gp_String_View *sv);

#endif // _GPBOTLIB_JSON_H_