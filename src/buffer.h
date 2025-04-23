#ifndef   __GPBOTLIB_BUFFER_H_
#define   __GPBOTLIB_BUFFER_H_

#include "gpbotlib/result.h"

#include <stdint.h>

typedef struct _Gp_Buffer {
  char *data;
  size_t current;
  size_t count;
  size_t capacity;
} _Gp_Buffer;

Gp_Result _gp_reserve_buffer(_Gp_Buffer *buffer, size_t capacity);
Gp_Result _gp_read_byte_from_buffer(void *buffer, uint8_t *byte);
Gp_Result _gp_write_byte_to_buffer(void *buffer, uint8_t byte);
Gp_Result _gp_shift_buffer(_Gp_Buffer *buffer, size_t offset);
Gp_Result _gp_copy_buffer(_Gp_Buffer *dst, const _Gp_Buffer *src, size_t position);

#endif // __GPBOTLIB_BUFFER_H_