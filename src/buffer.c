#include "buffer.h"

#include <stdlib.h>
#include <string.h>

Gp_Result _gp_reserve_buffer(Gp_Buffer *buffer, size_t capacity) {
  if (!buffer || !capacity) return GP_INVALID_ARGS;

  buffer->data = malloc(buffer->capacity += capacity);
  if (!buffer->data) return GP_BUY_MORE_RAM;

  return GP_SUCCESS;
}

Gp_Result _gp_read_byte_from_buffer(void *buffer, uint8_t *byte) {
  if (!buffer || !byte) return GP_INVALID_ARGS;

  Gp_Buffer *buf = (Gp_Buffer*)buffer;
  if (buf->current >= buf->count) return GP_UNDERFLOW;
  *byte = buf->data[buf->current++];

  return GP_SUCCESS;
}

Gp_Result _gp_write_byte_to_buffer(void *buffer, uint8_t byte) {
  if (!buffer) return GP_INVALID_ARGS;

  Gp_Buffer *buf = (Gp_Buffer*)buffer;
  if (buf->count >= buf->capacity) {
    if (buf->capacity) buf->capacity <<= 1;
    else buf->capacity = 64;

    buf->data = realloc(buf->data, buf->capacity);
    if (!buf->data) return GP_BUY_MORE_RAM;
  }

  if (!buf->data) return GP_INVALID_ARGS;
  buf->data[buf->count++] = byte;

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