#include "gpbotlib/uuid.h"

Gp_Result gp_parse_uuid(void *buffer, Gp_Uuid *value, Gp_Read_Byte_From_Buffer read) {
  if (!buffer || !value || !read) return GP_INVALID_ARGS;

  Gp_Result result = GP_SUCCESS;
  for (uint8_t i = 0; i < 16 && result >= GP_SUCCESS; ++i)
    result = read(buffer, &value->bytes[i]);

  return result;
}

Gp_Result gp_write_uuid(void *buffer, Gp_Uuid value, Gp_Write_Byte_To_Buffer write) {
  if (!buffer || !write) return GP_INVALID_ARGS;

  Gp_Result result = GP_SUCCESS;
  for (uint8_t i = 0; i < 16 && result >= GP_SUCCESS; ++i)
    result = write(buffer, value.bytes[i]);

  return result;
}