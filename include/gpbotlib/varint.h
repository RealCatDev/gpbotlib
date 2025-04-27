#ifndef _GPBOTLIB_VARINT_H_
#define _GPBOTLIB_VARINT_H_

#include <gpbotlib/buffer.h>

#include <stddef.h>
#include <stdint.h>

#define GP_VARINT_CONTINUE_BIT   1<<7
#define GP_VARINT_SEGMENT_BITS ~(GP_VARINT_CONTINUE_BIT)

typedef int32_t Gp_Varint;
typedef int64_t Gp_Varlong;

Gp_Result gp_parse_varint(void *buffer, Gp_Varint *value);
Gp_Result gp_parse_varlong(void *buffer, Gp_Varlong *value);

Gp_Result gp_write_varint(void *buffer, Gp_Varint val);
Gp_Result gp_write_varlong(void *buffer, Gp_Varlong val);

#endif // _GPBOTLIB_VARINT_H_