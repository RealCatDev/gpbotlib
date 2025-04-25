#include "gpbotlib/string.h"

#include <stdlib.h>
#include <ctype.h>

// Gp_String:

Gp_Result gp_parse_string(void *buffer, Gp_String *value, Gp_Read_Byte_From_Buffer read) {
  if (!buffer || !value || !read) return GP_INVALID_ARGS;

  Gp_Result result = GP_SUCCESS;
  if ((result = gp_parse_varint(buffer, &value->length, read)) < GP_SUCCESS) return result;

  value->data = malloc(value->length);
  for (Gp_Varint i = 0; i < value->length && result >= GP_SUCCESS; ++i)
    result = read(buffer, &value->data[i]);

  return result;
}

Gp_Result gp_write_string(void *buffer, Gp_String value, Gp_Write_Byte_To_Buffer write) {
  if (!buffer || !write) return GP_INVALID_ARGS;

  Gp_Result result = GP_SUCCESS;
  if ((result = gp_write_varint(buffer, value.length, write)) < GP_SUCCESS) return result;

  for (Gp_Varint i = 0; i < value.length && result >= GP_SUCCESS; ++i)
    result = write(buffer, value.data[i]);

  return result;
}

void gp_string_free(Gp_String *string) {
  if (!string) return;
  free(string->data);
  string->data = NULL;
}

// Gp_String_View:

bool gp_string_view_is_eq(Gp_String_View lhs, Gp_String_View rhs) {
  return lhs.count == rhs.count && !strncmp(lhs.items, rhs.items, lhs.count);
}

Gp_String_View gp_string_view_trim(Gp_String_View sv) {
  return gp_string_view_trim_right(gp_string_view_trim_left(sv));
}

Gp_String_View gp_string_view_trim_left(Gp_String_View sv) {
  while (sv.count && isspace(*sv.items)) {
    ++sv.items;
    --sv.count;
  }

  return sv;
}

Gp_String_View gp_string_view_trim_right(Gp_String_View sv) {
  while (sv.count && isspace(sv.items[sv.count-1])) --sv.count;

  return sv;
}

Gp_String_View gp_string_view_chop(Gp_String_View *sv, size_t n) {
  if (n > sv->count) n = sv->count;
  Gp_String_View result = (Gp_String_View){ .items = sv->items, .count = n };
  sv->items += n;
  sv->count -= n;
  return result;
}

Gp_String_View gp_string_view_chop_delim(Gp_String_View *sv, char delim) {
  size_t n = 0;
  while (n < sv->count && sv->items[n] != delim) ++n;

  Gp_String_View result = (Gp_String_View){
    .items = sv->items,
    .count = n
  };

  if (n < sv->count) ++n;
  sv->items += n;
  sv->count -= n;

  return result;
}

Gp_String_View gp_string_view_chop_while(Gp_String_View *sv, bool (*predicate)(char)) {
  size_t n = 0;
  while (n < sv->count && predicate(sv->items[n])) ++n;
  return gp_string_view_chop(sv, n);
}

// Gp_String_Builder:

Gp_Result gp_string_builder_append_int(Gp_String_Builder *lhs, int64_t number) {
  if (!lhs) return GP_INVALID_ARGS;

  bool sign = number&((uint64_t)1<<63);
  number &= ~((uint64_t)1<<63);

  #define BUF_SIZE 32
  #define BUF_PUSH(x) buf[(BUF_SIZE-1)-(i++)] = x

  char buf[BUF_SIZE] = {0};
  int i = 0;

  do {
    BUF_PUSH('0' + (number%10));
    number /= 10;
  } while (number);

  if (sign) BUF_PUSH('-');

  return gp_string_builder_append_string_view(lhs, (Gp_String_View){
    .items = buf+(BUF_SIZE-i),
    .count = i
  });
}

Gp_Result gp_string_builder_append_float(Gp_String_Builder *lhs, double floating) { // https://www.geeksforgeeks.org/convert-floating-point-number-string/
  if (!lhs) return GP_INVALID_ARGS;

  Gp_Result result = GP_SUCCESS;
  if ((result = gp_string_builder_append_int(lhs, (int64_t)floating)) < GP_SUCCESS) return result;

  floating -= (int64_t)floating; // Leave only fraction part

  if (floating) gp_string_builder_append_char(lhs, '.');
  while (floating && result >= GP_SUCCESS) {
    int digit = floating *= 10;
    floating -= digit;
    result = gp_string_builder_append_char(lhs, '0' + digit);
  }

  return result;
}

Gp_Result gp_string_builder_append_string(Gp_String_Builder *lhs, Gp_String rhs) {
  return gp_string_builder_append_string_view(lhs, (Gp_String_View){ .items = rhs.data, .count = rhs.length });
}

Gp_Result gp_string_builder_append_string_view(Gp_String_Builder *lhs, Gp_String_View rhs) {
  if (!lhs) return GP_INVALID_ARGS;

  if (lhs->count+rhs.count > lhs->capacity) {
    if (!lhs->capacity) lhs->capacity = (1<<6);
    while (lhs->count+rhs.count > lhs->capacity)
      lhs->capacity <<= 1;

    lhs->items = realloc(lhs->items, lhs->capacity);
    if (!lhs->items) return GP_BUY_MORE_RAM;
  }

  if (!lhs->items) return GP_INVALID_ARGS;

  memcpy(&lhs->items[lhs->count], rhs.items, rhs.count);
  lhs->count += rhs.count;

  return GP_SUCCESS;
}

Gp_Result gp_string_builder_append_cstr(Gp_String_Builder *lhs, const char *rhs) {
  if (!lhs || !rhs) return GP_INVALID_ARGS;
  return gp_string_builder_append_string(lhs, GP_STRING((char*)rhs));
}

Gp_Result gp_string_builder_append_char(Gp_String_Builder *lhs, char rhs) {
  if (!lhs) return GP_INVALID_ARGS;

  if (lhs->count >= lhs->capacity) {
    if (lhs->capacity) lhs->capacity <<= 1;
    else lhs->capacity = (1<<6);

    lhs->items = realloc(lhs->items, lhs->capacity);
    if (!lhs->items) return GP_BUY_MORE_RAM;
  }

  if (!lhs->items) return GP_INVALID_ARGS;
  lhs->items[lhs->count++] = rhs;

  return GP_SUCCESS;
}