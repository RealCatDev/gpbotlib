#include "buffer.h"

#include <stdlib.h>
#include <string.h>

Gp_Result gp_read_byte_from_buffer(void *buffer, uint8_t *data) {
  if (!buffer) return GP_INVALID_ARGS;
  return gp_read_bytes_from_buffer(buffer, data, sizeof(*data));
}

#define read_type(name, type, intType) \
Gp_Result gp_read_##name##_from_buffer(void *buffer, type *data) { \
  if (!buffer || !data) return GP_INVALID_ARGS; \
 \
  Gp_Result result = GP_SUCCESS; \
  for (uint8_t i = 0; i < sizeof(type); ++i) { \
    uint8_t byte = 0; \
    if ((result = gp_read_byte_from_buffer(buffer, &byte)) < GP_SUCCESS) return result; \
    *(intType*)data = (*(intType*)data << 8) | byte; \
  } \
 \
  return GP_SUCCESS; \
}

read_type(uint16, uint16_t, uint16_t)
read_type(uint32, uint32_t, uint32_t)
read_type(uint64, uint64_t, uint64_t)
read_type(float,  float,    uint32_t)
read_type(double, double,   uint64_t)

Gp_Result gp_read_bytes_from_buffer(void *buffer, void *bytes, size_t count) {
  if (!buffer) return GP_INVALID_ARGS;
  if (!count) return GP_SUCCESS;
  if (!bytes) return GP_INVALID_ARGS;

  Gp_Buffer *buf = (Gp_Buffer*)buffer;
  if (buf->current+count > buf->count) return GP_UNDERFLOW;
  memcpy(bytes, &buf->data[buf->current], count);
  buf->current += count;

  return GP_SUCCESS;
}

Gp_Result gp_write_byte_to_buffer(void *buffer, uint8_t data) {
  if (!buffer) return GP_INVALID_ARGS;
  return gp_write_bytes_to_buffer(buffer, &data, sizeof(data));
}

Gp_Result gp_write_uint16_to_buffer(void *buffer, uint16_t data) {
  if (!buffer) return GP_INVALID_ARGS;
  return gp_write_bytes_to_buffer(buffer, &data, sizeof(data));
}

Gp_Result gp_write_uint32_to_buffer(void *buffer, uint32_t data) {
  if (!buffer) return GP_INVALID_ARGS;
  return gp_write_bytes_to_buffer(buffer, &data, sizeof(data));
}

Gp_Result gp_write_uint64_to_buffer(void *buffer, uint64_t data) {
  if (!buffer) return GP_INVALID_ARGS;
  return gp_write_bytes_to_buffer(buffer, &data, sizeof(data));
}

Gp_Result gp_write_float_to_buffer(void *buffer, float data) {
  if (!buffer) return GP_INVALID_ARGS;
  return gp_write_bytes_to_buffer(buffer, &data, sizeof(data));
}

Gp_Result gp_write_double_to_buffer(void *buffer, double data) {
  if (!buffer) return GP_INVALID_ARGS;
  return gp_write_bytes_to_buffer(buffer, &data, sizeof(data));
}

Gp_Result gp_write_bytes_to_buffer(void *buffer, const void *bytes, size_t count) {
  if (!buffer) return GP_INVALID_ARGS;
  if (!count) return GP_SUCCESS;
  if (!bytes) return GP_INVALID_ARGS;

  Gp_Buffer *buf = (Gp_Buffer*)buffer;
  if (buf->count+count > buf->capacity) {
    if (!buf->capacity) buf->capacity = 64;
    while (buf->count+count > buf->capacity) buf->capacity <<= 1;

    buf->data = realloc(buf->data, buf->capacity);
    if (!buf->data) return GP_BUY_MORE_RAM;
  }

  if (!buf->data) return GP_INVALID_ARGS;
  memcpy(&buf->data[buf->count], bytes, count);
  buf->count += count;

  return GP_SUCCESS;
}



Gp_Result _gp_reserve_buffer(Gp_Buffer *buffer, size_t capacity) {
  if (!buffer || !capacity) return GP_INVALID_ARGS;

  buffer->data = malloc(buffer->capacity += capacity);
  if (!buffer->data) return GP_BUY_MORE_RAM;

  return GP_SUCCESS;
}

Gp_Result _gp_shift_buffer(Gp_Buffer *buffer, size_t offset) {
  if (!buffer) return GP_INVALID_ARGS;
  if (!offset) return GP_SUCCESS;

  if (buffer->count + offset > buffer->capacity) {
    if (!buffer->capacity) buffer->capacity = 64;
    while (buffer->count + offset > buffer->capacity)
      buffer->capacity <<= 1;

    buffer->data = realloc(buffer->data, buffer->capacity);
    if (!buffer->data) return GP_BUY_MORE_RAM;
  }

  if (!buffer->data) return GP_INVALID_ARGS;
  memmove(&buffer->data[offset], buffer->data, buffer->count);
  memset(buffer->data, 0, offset);
  buffer->count += offset;

  return GP_SUCCESS;
}

Gp_Result _gp_copy_buffer(Gp_Buffer *dst, const Gp_Buffer *src, size_t position) {
  if (!dst || !src) return GP_INVALID_ARGS;

  size_t dstCapacity = dst->count-(dst->count-position)+src->count;
  if (dstCapacity >= dst->capacity) {
    if (!dst->capacity) dst->capacity = 64;
    while (dstCapacity >= dst->capacity)
      dst->capacity <<= 1;

    dst->data = realloc(dst->data, dst->capacity);
    if (!dst->data) return GP_BUY_MORE_RAM;
  }

  if (!dst->data) return GP_INVALID_ARGS;
  memcpy(&dst->data[position], src->data, src->count);
  // dst->count += max(0, src->count-dst->count-position);

  return GP_SUCCESS;
}