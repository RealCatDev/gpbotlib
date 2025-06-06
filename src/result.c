#include "gpbotlib/result.h"

#include <assert.h>

const char *gp_result_as_cstr(Gp_Result result) {
  switch (result) {
  case GP_DUPLICATE: return "Duplicate";
  case GP_INVALID_JSON: return "Invalid JSON";
  case GP_INVALID_PACKET: return "Invalid packet";
  case GP_DISCONNECTED: return "Disconnected";
  case GP_INTERNAL_ERROR: return "Internal error";
  case GP_UNDERFLOW: return "Underflow";
  case GP_BUY_MORE_RAM: return "Buy more ram";
  case GP_VARINT_TOO_LONG: return "Varint too long";
  case GP_INVALID_ARGS: return "Invalid args";
  case GP_READ_ERROR: return "Read error";
  case GP_WRITE_ERROR: return "Write error";
  case GP_SUCCESS: return "Success";
  }

  assert(0 && "Unreachable");
  return "<UNDEFINED>";
}
