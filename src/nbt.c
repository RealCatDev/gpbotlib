#include <gpbotlib/nbt.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

bool gp_is_valid_nbt(const Gp_Nbt nbt) {
  return nbt.type == GP_NBT_TAG_COMPOUND;
}

Gp_Result gp_parse_nbt_tag_as(void *buffer, Gp_Nbt_Tag_As *as, Gp_Nbt_Tag_Type type) {
  if (!buffer || !as) return GP_INVALID_ARGS;

  memset(as, 0, sizeof(*as));

  Gp_Result result = GP_SUCCESS;
  switch (type) {
  case GP_NBT_TAG_BYTE: {
    return gp_read_byte_from_buffer(buffer, &as->Byte);
  } break;
  case GP_NBT_TAG_SHORT: {
    return gp_read_uint16_from_buffer(buffer, &as->Short);
  } break;
  case GP_NBT_TAG_INT: {
    return gp_read_uint32_from_buffer(buffer, &as->Int);
  } break;
  case GP_NBT_TAG_LONG: {
    return gp_read_uint64_from_buffer(buffer, &as->Long);
  } break;
  case GP_NBT_TAG_FLOAT: {
    return gp_read_float_from_buffer(buffer, &as->Float);
  } break;
  case GP_NBT_TAG_DOUBLE: {
    return gp_read_double_from_buffer(buffer, &as->Double);
  } break;
  case GP_NBT_TAG_BYTE_ARRAY: {
    if ((result = gp_read_uint32_from_buffer(buffer, &as->ByteArray.size)) < GP_SUCCESS) return result;

    as->ByteArray.array = malloc(sizeof(*as->ByteArray.array) * as->ByteArray.size);
    if (!as->ByteArray.array) return GP_BUY_MORE_RAM;

    if ((result = gp_read_bytes_from_buffer(buffer, as->ByteArray.array, as->ByteArray.size)) < GP_SUCCESS) return result;
  } break;
  case GP_NBT_TAG_STRING: {
    if ((result = gp_read_uint16_from_buffer(buffer, &as->String.length)) < GP_SUCCESS) return result;

    as->String.string = malloc(as->String.length);
    if (!as->String.string) return GP_BUY_MORE_RAM;

    if ((result = gp_read_bytes_from_buffer(buffer, as->String.string, as->String.length)) < GP_SUCCESS) return result;
  } break;
  case GP_NBT_TAG_LIST: {
    if ((result = gp_read_byte_from_buffer(buffer, &as->List.tagType)) < GP_SUCCESS) return result;
    if ((result = gp_read_uint32_from_buffer(buffer, &as->List.length)) < GP_SUCCESS) return result;

    as->List.payload = malloc(sizeof(*as->List.payload) * as->List.length);
    if (!as->List.payload) return GP_BUY_MORE_RAM;
    for (int32_t i = 0; i < as->List.length; ++i)
      if ((result = gp_parse_nbt_tag_as(buffer, &as->List.payload[i], as->List.tagType)) < GP_SUCCESS) return result;
  } break;
  case GP_NBT_TAG_COMPOUND: {
    Gp_Nbt_Tag currentTag = {0};
    while ((result = gp_parse_nbt_tag(buffer, &currentTag)) >= GP_SUCCESS && currentTag.type != GP_NBT_TAG_END) {
      if (as->Compound.count >= as->Compound.capacity) {
        if (as->Compound.capacity) as->Compound.capacity <<= 1;
        else as->Compound.capacity = 8;

        as->Compound.tags = realloc(as->Compound.tags, sizeof(*as->Compound.tags) * as->Compound.capacity);
        if (!as->Compound.tags) return GP_BUY_MORE_RAM;
      }

      assert(as->Compound.tags);
      as->Compound.tags[as->Compound.count++] = currentTag;
    }
  } break;
  case GP_NBT_TAG_INT_ARRAY: {
    if ((result = gp_read_uint32_from_buffer(buffer, &as->IntArray.size)) < GP_SUCCESS) return result;

    as->IntArray.data = malloc(sizeof(*as->IntArray.data) * as->IntArray.size);
    if (!as->IntArray.data) return GP_BUY_MORE_RAM;

    if ((result = gp_read_bytes_from_buffer(buffer, as->IntArray.data, sizeof(*as->IntArray.data) * as->IntArray.size)) < GP_SUCCESS) return result;

    for (int32_t i = 0; i < as->IntArray.size; ++i)
      if ((result = gp_read_uint32_from_buffer(buffer, &as->IntArray.data[i])) < GP_SUCCESS) return result;
  } break;
  case GP_NBT_TAG_LONG_ARRAY: {
    if ((result = gp_read_uint32_from_buffer(buffer, &as->LongArray.size)) < GP_SUCCESS) return result;

    as->LongArray.data = malloc(sizeof(*as->LongArray.data) * as->LongArray.size);
    if (!as->LongArray.data) return GP_BUY_MORE_RAM;

    for (int32_t i = 0; i < as->LongArray.size; ++i)
      if ((result = gp_read_uint64_from_buffer(buffer, &as->LongArray.data[i])) < GP_SUCCESS) return result;
  } break;
  }

  return GP_SUCCESS;
}

Gp_Result gp_write_nbt_tag_as(void *buffer, Gp_Nbt_Tag_As as, Gp_Nbt_Tag_Type type) {
  if (!buffer) return GP_INVALID_ARGS;

  Gp_Result result = GP_SUCCESS;
  switch (type) {
  case GP_NBT_TAG_BYTE: {
    return gp_write_byte_to_buffer(buffer, as.Byte);
  } break;
  case GP_NBT_TAG_SHORT: {
    return gp_write_uint16_to_buffer(buffer, as.Short);
  } break;
  case GP_NBT_TAG_INT: {
    return gp_write_uint32_to_buffer(buffer, as.Int);
  } break;
  case GP_NBT_TAG_LONG: {
    return gp_write_uint64_to_buffer(buffer, as.Long);
  } break;
  case GP_NBT_TAG_FLOAT: {
    return gp_write_float_to_buffer(buffer, as.Float);
  } break;
  case GP_NBT_TAG_DOUBLE: {
    return gp_write_double_to_buffer(buffer, as.Double);
  } break;
  case GP_NBT_TAG_BYTE_ARRAY: {
    if ((result = gp_write_uint32_to_buffer(buffer, as.ByteArray.size)) < GP_SUCCESS) return result;

    for (int32_t i = 0; i < as.ByteArray.size; ++i)
      if ((result = gp_write_byte_to_buffer(buffer, as.ByteArray.array[i])) < GP_SUCCESS) return result;
  } break;
  case GP_NBT_TAG_STRING: {
    if ((result = gp_write_uint16_to_buffer(buffer, as.String.length)) < GP_SUCCESS) return result;

    as.String.string = malloc(as.String.length);
    if (!as.String.string) return GP_BUY_MORE_RAM;

    if ((result = gp_write_bytes_to_buffer(buffer, as.String.string, as.String.length)) < GP_SUCCESS) return result;
  } break;
  case GP_NBT_TAG_LIST: {
    if ((result = gp_write_byte_to_buffer(buffer, as.List.tagType)) < GP_SUCCESS) return result;
    if ((result = gp_write_uint32_to_buffer(buffer, as.List.length)) < GP_SUCCESS) return result;

    for (int32_t i = 0; i < as.List.length; ++i)
      if ((result = gp_write_nbt_tag_as(buffer, as.List.payload[i], as.List.tagType)) < GP_SUCCESS) return result;
  } break;
  case GP_NBT_TAG_COMPOUND: {
    for (Gp_Nbt_Tag *ptr = as.Compound.tags, *end = as.Compound.tags+as.Compound.count; ptr != end; ++ptr)
      if ((result = gp_write_nbt_tag(buffer, *ptr)) < GP_SUCCESS) return result;

    return gp_write_nbt_tag(buffer, (Gp_Nbt_Tag){0});
  } break;
  case GP_NBT_TAG_INT_ARRAY: {
    if ((result = gp_write_uint32_to_buffer(buffer, as.IntArray.size)) < GP_SUCCESS) return result;

    for (int32_t i = 0; i < as.ByteArray.size; ++i)
      if ((result = gp_write_uint32_to_buffer(buffer, as.IntArray.data[i])) < GP_SUCCESS) return result;
  } break;
  case GP_NBT_TAG_LONG_ARRAY: {
    if ((result = gp_write_uint32_to_buffer(buffer, as.LongArray.size)) < GP_SUCCESS) return result;

    for (int32_t i = 0; i < as.LongArray.size; ++i)
      if ((result = gp_write_uint64_to_buffer(buffer, as.LongArray.data[i])) < GP_SUCCESS) return result;
  } break;
  }

  return GP_SUCCESS;
}

Gp_Result gp_parse_nbt_tag(void *buffer, Gp_Nbt_Tag *tag) {
  if (!buffer || !tag) return GP_INVALID_ARGS;

  memset(tag, 0, sizeof(*tag));

  Gp_Result result = GP_SUCCESS;
  if ((result = gp_read_byte_from_buffer(buffer, &tag->type)) < GP_SUCCESS) return result;

  if (tag->type == GP_NBT_TAG_END) return GP_SUCCESS;

  if ((result = gp_read_uint16_from_buffer(buffer, &tag->nameLength)) < GP_SUCCESS) return result;

  tag->name = malloc(tag->nameLength);
  if (!tag->name) return GP_BUY_MORE_RAM;

  if ((result = gp_read_bytes_from_buffer(buffer, tag->name, tag->nameLength)) < GP_SUCCESS) return result;

  return gp_parse_nbt_tag_as(buffer, &tag->as, tag->type);
}

Gp_Result gp_write_nbt_tag(void *buffer, Gp_Nbt_Tag tag) {
  if (!buffer) return GP_INVALID_ARGS;

  Gp_Result result = GP_SUCCESS;
  if ((result = gp_write_byte_to_buffer(buffer, tag.type)) < GP_SUCCESS) return result;

  if (tag.type == GP_NBT_TAG_END) return GP_SUCCESS;

  if ((result = gp_write_uint16_to_buffer(buffer, tag.nameLength)) < GP_SUCCESS) return result;

  if ((result = gp_write_bytes_to_buffer(buffer, tag.name, tag.nameLength)) < GP_SUCCESS) return result;

  return gp_write_nbt_tag_as(buffer, tag.as, tag.type);
}