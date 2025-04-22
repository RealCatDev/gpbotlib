#include <stdbool.h>

#include <gpbotlib/buffer.h>
#include <gpbotlib/result.h>
#include <gpbotlib/varint.h>

Gp_Result gp_parse_varint(void *buffer, Gp_Varint *value, Gp_Read_Byte_From_Buffer read) {
  if (!buffer || !value || !read) return GP_INVALID_ARGS;

  Gp_Varint val = 0;
  uint8_t position = 0;
  uint8_t currentByte;

  while(true) {
    Gp_Result result = read(buffer, &currentByte);
    if(result != GP_SUCCESS) return result;

    val |= (currentByte & GP_VARINT_SEGMENT_BITS) << position;

    if((currentByte & GP_VARINT_CONTINUE_BIT) == 0) break;

    position += 7;

    if (position >= 32) return GP_VARINT_TOO_LONG;
  }

  *value = val;
  return GP_SUCCESS;
}

Gp_Result gp_parse_varlong(void *buffer, Gp_Varlong *value, Gp_Read_Byte_From_Buffer read) {
  if (!buffer || !value || !read) return GP_INVALID_ARGS;

  Gp_Varlong val = 0;
  uint8_t position = 0;
  uint8_t currentByte;

  while(true) {
    Gp_Result result = read(buffer, &currentByte);
    if(result != GP_SUCCESS) return result;

    val |= (currentByte & GP_VARINT_SEGMENT_BITS) << position;

    if((currentByte & GP_VARINT_CONTINUE_BIT) == 0) break;

    position += 7;

    if (position >= 64) return GP_VARINT_TOO_LONG;
  }

  *value = val;
  return GP_SUCCESS;
}


Gp_Result gp_write_varint(void *buffer, Gp_Varint val, Gp_Write_Byte_To_Buffer write) {
  if (!buffer || !write) return GP_INVALID_ARGS;

  Gp_Result result = GP_SUCCESS;

  uint32_t valUint = val;
  do {
    result = write(buffer, (valUint & GP_VARINT_SEGMENT_BITS) | ((valUint & ~GP_VARINT_SEGMENT_BITS)?GP_VARINT_CONTINUE_BIT:0));

    valUint >>= 7;
  } while (valUint && result >= GP_SUCCESS);

  return result;
}

Gp_Result gp_write_varlong(void *buffer, Gp_Varlong val, Gp_Write_Byte_To_Buffer write) {
  if (!buffer || !write) return GP_INVALID_ARGS;

  Gp_Result result = GP_SUCCESS;

  uint64_t valUint = val;
  do {
    result = write(buffer, (valUint & GP_VARINT_SEGMENT_BITS) | ((valUint & ~GP_VARINT_SEGMENT_BITS)?GP_VARINT_CONTINUE_BIT:0));

    valUint >>= 7;
  } while (valUint && result >= GP_SUCCESS);

  return result;
}