#include <stddef.h>

#include <gpbotlib/nbt.h>
#include <gpbotlib/result.h>

Gp_Result _gp_create_nbt_tag(Gp_Nbt_Tag **tag) {
  *tag = malloc(sizeof(Gp_Nbt_Tag));
  if(!tag) return GP_BUY_MORE_RAM;
  return GP_SUCCESS;
}

Gp_Result _gp_create_nbt_tag_and_alloc_data(Gp_Nbt_Tag **tag, size_t dataSize) {
  *tag = malloc(sizeof(Gp_Nbt_Tag));
  (*tag)->data = malloc(dataSize);
  if(!tag || !(*tag)->data) return GP_BUY_MORE_RAM;
  return GP_SUCCESS;
}

bool gp_is_valid_nbt(const Gp_Nbt nbt) {
  return nbt.type == GP_NBT_TAG_COMPOUND;
}

Gp_Result gp_nbt_parse(void *buffer, Gp_Nbt *nbt, const Gp_Read_Byte_From_Buffer read) {
  assert(0 && "NOT IMPLEMENTED");
  return GP_SUCCESS;
}

Gp_Result gp_nbt_write(void *buffer, Gp_Nbt nbt, const Gp_Write_Byte_From_Buffer write) {
  assert(0 && "NOT IMPLEMENTED");
  return GP_SUCCESS;
}
