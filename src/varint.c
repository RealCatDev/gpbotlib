#include <stdbool.h>

#include <gpbotlib/buffer.h>
#include <gpbotlib/result.h>
#include <gpbotlib/varint.h>

Gp_Result gp_parse_varint(void *buffer, Gp_Varint *value, Gp_Read_Byte_From_Buffer read) {
  Gp_Varint val = 0;
  uint8_t position = 0;
  uint8_t currentByte;

  while(true) {
    Gp_Result result = read(buffer, &currentByte);
    if(result != GP_SUCCESS) return result;

    value |= (currentByte & GP_VARINT_SEGMENT_BITS) << position;

    if((currentByte & CONTINUE_BIT) == 0) break;

    position += 7;

    if (position >= 32) return GP_VARINT_TOO_LONG;
  }
  
  *value = val;
  return GP_SUCCESS;
}

Gp_Result gp_parse_varlong(void *buffer, Gp_Varlong *val, Gp_Read_Byte_From_Buffer read) {
  Gp_Varlong val = 0;
  uint8_t position = 0;
  uint8_t currentByte;

  while(true) {
    Gp_Result result = read(buffer, &currentByte);
    if(result != GP_SUCCESS) return result;

    value |= (currentByte & GP_VARINT_SEGMENT_BITS) << position;

    if((currentByte & CONTINUE_BIT) == 0) break;

    position += 7;

    if (position >= 64) return GP_VARINT_TOO_LONG;
  }
  
  *value = val;
  return GP_SUCCESS;
}


Gp_Result gp_write_varint(void *buffer, Gp_Varint val, Gp_Write_Byte_From_Buffer write) {
  uint32_t valUint = val;
  while (true) {
    if ((valUint & ~GP_VARINT_SEGMENT_BITS) == 0) {
      return write(buffer, valUint);
    }

    Gp_Result result = write(buffer, (valUint & SEGMENT_BITS) | CONTINUE_BIT);

    valUint >>= 7;
  }
}

Gp_Result gp_write_varlong(void *buffer, Gp_Varlong val, Gp_Write_Byte_From_Buffer write) {
  uint64_t valUint = val;
  while (true) {
    if ((valUint & ~GP_VARINT_SEGMENT_BITS) == 0) {
      return write(buffer, valUint);
    }

    Gp_Result result = write(buffer, (valUint & SEGMENT_BITS) | CONTINUE_BIT);

    valUint >>= 7;
  }
}

