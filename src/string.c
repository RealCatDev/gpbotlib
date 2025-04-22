#include "gpbotlib/string.h"

#include <stdlib.h>

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