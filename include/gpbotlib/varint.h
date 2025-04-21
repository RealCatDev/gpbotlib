#ifndef _GPBOTLIB_VARINT_H_
#define _GPBOTLIB_VARINT_H_

#include <gpbotlib/buffer.h>

#include <stddef.h> 
#include <stdint.h>

#define GP_VARINT_CONTINUE_BIT 0x80
#define GP_VARINT_SEGMENT_BITS 0x7F

Gp_Result gp_parse_varint(void *buffer, uint32_t *val, Gp_Read_Byte_From_Buffer read);
Gp_Result gp_parse_varlong(void *buffer, uint64_t *val, Gp_Read_Byte_From_Buffer read);

#endif // _GPBOTLIB_VARINT_H_
