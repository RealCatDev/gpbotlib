#include <gpbotlib/nbt.h>

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

bool gp_is_valid_nbt(const Gp_Nbt nbt) {
  return nbt.type == GP_NBT_TAG_COMPOUND;
}

Gp_Result gp_nbt_parse(void *buffer, Gp_Nbt *nbt, const Gp_Read_Byte_From_Buffer read) {
  assert(0 && "NOT IMPLEMENTED");
  return GP_SUCCESS;
}

Gp_Result gp_nbt_write(void *buffer, Gp_Nbt nbt, const Gp_Write_Byte_To_Buffer write) {
  assert(0 && "NOT IMPLEMENTED");
  return GP_SUCCESS;
}