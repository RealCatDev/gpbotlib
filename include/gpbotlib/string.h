#ifndef   _GPBOTLIB_STRING_H_
#define   _GPBOTLIB_STRING_H_

#include "result.h"
#include "varint.h"

#include <stdbool.h>
#include <string.h>

typedef struct Gp_String {
  Gp_Varint length;
  char *data;
} Gp_String;

#define GP_STRING(cstr) ((Gp_String){ .length = strlen(cstr), .data = cstr })

Gp_Result gp_parse_string(void *buffer, Gp_String *value, Gp_Read_Byte_From_Buffer read);
Gp_Result gp_write_string(void *buffer, Gp_String value, Gp_Write_Byte_To_Buffer write);

void gp_string_free(Gp_String *string);

typedef struct Gp_String_View {
  const char *items;
  size_t count;
} Gp_String_View;

#define GP_STRING_VIEW(cstr) ((Gp_String_View){ .items = cstr, .count = strlen(cstr) })

bool gp_string_view_is_eq(Gp_String_View lhs, Gp_String_View rhs);

Gp_String_View gp_string_view_trim(Gp_String_View sv);
Gp_String_View gp_string_view_trim_left(Gp_String_View sv);
Gp_String_View gp_string_view_trim_right(Gp_String_View sv);

Gp_String_View gp_string_view_chop(Gp_String_View *sv, size_t n);
Gp_String_View gp_string_view_chop_delim(Gp_String_View *sv, char delim);
Gp_String_View gp_string_view_chop_while(Gp_String_View *sv, bool (*predicate)(char));

typedef struct Gp_String_Builder {
  char *items;
  size_t capacity;
  size_t count;
} Gp_String_Builder;

Gp_Result gp_string_builder_append_int(Gp_String_Builder *lhs, int64_t number);
Gp_Result gp_string_builder_append_float(Gp_String_Builder *lhs, double floating);
Gp_Result gp_string_builder_append_string(Gp_String_Builder *lhs, Gp_String rhs);
Gp_Result gp_string_builder_append_string_view(Gp_String_Builder *lhs, Gp_String_View rhs);
Gp_Result gp_string_builder_append_cstr(Gp_String_Builder *lhs, const char *rhs);
Gp_Result gp_string_builder_append_char(Gp_String_Builder *lhs, char rhs);

#endif // _GPBOTLIB_STRING_H_