#include "gpbotlib/uuid.h"

Gp_Result gp_parse_uuid(void *buffer, Gp_Uuid *value) {
  if (!buffer || !value) return GP_INVALID_ARGS;

  return gp_read_bytes_from_buffer(buffer, value->bytes, sizeof(*value->bytes));
}

Gp_Result gp_write_uuid(void *buffer, Gp_Uuid value) {
  if (!buffer) return GP_INVALID_ARGS;

  return gp_write_bytes_to_buffer(buffer, value.bytes, sizeof(*value.bytes));
}