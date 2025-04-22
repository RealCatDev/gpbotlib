#ifndef   _GPBOTLIB_STRING_H_
#define   _GPBOTLIB_STRING_H_

#include "result.h"
#include "varint.h"

#include <string.h>

typedef struct Gp_String {
  Gp_Varint length;
  char *data;
} Gp_String;

Gp_Result gp_parse_string(void *buffer, Gp_String *value, Gp_Read_Byte_From_Buffer read);
Gp_Result gp_write_string(void *buffer, Gp_String value, Gp_Write_Byte_To_Buffer write);

void gp_string_free(Gp_String *string);

#define GP_STRING(cstr) (Gp_String){ .length = strlen(cstr), .data = cstr }

#endif // _GPBOTLIB_STRING_H_