#ifndef _GPBOTLIB_BUFFER_H_
#define _GPBOTLIB_BUFFER_H_

#include <stdint.h>

typedef Gp_Result (*Gp_Read_Byte_From_Buffer)(uint8_t *);
typedef Gp_Result (*Gp_Write_Byte_To_Buffer)(uint8_t);

#endif // _GPBOTLIB_BUFFER_H_
