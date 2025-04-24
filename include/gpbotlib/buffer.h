#ifndef _GPBOTLIB_BUFFER_H_
#define _GPBOTLIB_BUFFER_H_

#include <stdint.h>

#include "result.h"

typedef Gp_Result (*Gp_Read_Byte_From_Buffer)(void *, uint8_t *);
typedef Gp_Result (*Gp_Write_Byte_To_Buffer)(void *, uint8_t);

typedef struct Gp_Buffer {
  char *data;
  size_t current;
  size_t count;
  size_t capacity;
} Gp_Buffer;

#endif // _GPBOTLIB_BUFFER_H_
