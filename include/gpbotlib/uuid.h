#ifndef   _GPBOTLIB_UUID_H_
#define   _GPBOTLIB_UUID_H_

#include "result.h"
#include "buffer.h"

#include <stdint.h>

typedef struct Gp_Uuid {
  uint8_t bytes[16];
} Gp_Uuid;

Gp_Result gp_parse_uuid(void *buffer, Gp_Uuid *value);
Gp_Result gp_write_uuid(void *buffer, Gp_Uuid value);

#endif // _GPBOTLIB_UUID_H_