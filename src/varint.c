#include <stdbool.h>

#include <gpbotlib/buffer.h>
#include <gpbotlib/result.h>
#include <gpbotlib/varint.h>

Gp_Result gp_parse_varint(uint32_t *value, Gp_Read_Byte_From_Buffer read) {
  uint32_t val = 0;
  uint8_t position = 0;
  uint8_t currentByte;

  while(true) {
    Gp_Result result = read(&currentByte);
    if(result != GP_SUCCESS) return result;

    value |= (currentByte & GP_VARINT_SEGMENT_BITS) << position;

    if((currentByte & CONTINUE_BIT) == 0) break;

    position += 7;

    if (position >= 32) return GP_VARINT_TOO_LONG;
  }
  
  *value = val;
  return GP_SUCCESS;
}

Gp_Result gp_parse_varlong(uint64_t *val, Gp_Read_Byte_From_Buffer read) {
  uint64_t val = 0;
  uint8_t position = 0;
  uint8_t currentByte;

  while(true) {
    Gp_Result result = read(&currentByte);
    if(result != GP_SUCCESS) return result;

    value |= (currentByte & GP_VARINT_SEGMENT_BITS) << position;

    if((currentByte & CONTINUE_BIT) == 0) break;

    position += 7;

    if (position >= 64) return GP_VARINT_TOO_LONG;
  }
  
  *value = val;
  return GP_SUCCESS;
}
