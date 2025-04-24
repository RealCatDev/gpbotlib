#ifndef   __GPBOTLIB_BUFFER_H_
#define   __GPBOTLIB_BUFFER_H_

#include "gpbotlib/result.h"
#include "gpbotlib/buffer.h"

#include <stdint.h>

Gp_Result _gp_reserve_buffer(Gp_Buffer *buffer, size_t capacity);
Gp_Result _gp_read_byte_from_buffer(void *buffer, uint8_t *byte);
Gp_Result _gp_write_byte_to_buffer(void *buffer, uint8_t byte);
Gp_Result _gp_shift_buffer(Gp_Buffer *buffer, size_t offset);
Gp_Result _gp_copy_buffer(Gp_Buffer *dst, const Gp_Buffer *src, size_t position);

#endif // __GPBOTLIB_BUFFER_H_