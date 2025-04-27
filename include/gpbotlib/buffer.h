#ifndef _GPBOTLIB_BUFFER_H_
#define _GPBOTLIB_BUFFER_H_

#include <stddef.h>
#include <stdint.h>

#include "result.h"

typedef struct Gp_Buffer {
  char *data;
  size_t current;
  size_t count;
  size_t capacity;
} Gp_Buffer;

Gp_Result gp_read_byte_from_buffer(void *buffer, uint8_t *data);
Gp_Result gp_read_uint16_from_buffer(void *buffer, uint16_t *data);
Gp_Result gp_read_uint32_from_buffer(void *buffer, uint32_t *data);
Gp_Result gp_read_uint64_from_buffer(void *buffer, uint64_t *data);
Gp_Result gp_read_float_from_buffer(void *buffer, float *data);
Gp_Result gp_read_double_from_buffer(void *buffer, double *data);
Gp_Result gp_read_bytes_from_buffer(void *buffer, void *bytes, size_t count);

Gp_Result gp_write_byte_to_buffer(void *buffer, uint8_t data);
Gp_Result gp_write_uint16_to_buffer(void *buffer, uint16_t data);
Gp_Result gp_write_uint32_to_buffer(void *buffer, uint32_t data);
Gp_Result gp_write_uint64_to_buffer(void *buffer, uint64_t data);
Gp_Result gp_write_float_to_buffer(void *buffer, float data);
Gp_Result gp_write_double_to_buffer(void *buffer, double data);
Gp_Result gp_write_bytes_to_buffer(void *buffer, const void *bytes, size_t count);

#endif // _GPBOTLIB_BUFFER_H_
