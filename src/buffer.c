#include "buffer.h"

#include <stdlib.h>
#include <string.h>

Gp_Result _gp_reserve_buffer(_Gp_Buffer *buffer, size_t capacity) {
  if (!buffer || !capacity) return GP_INVALID_ARGS;

  buffer->data = malloc(buffer->capacity += capacity);
  if (!buffer->data) return GP_BUY_MORE_RAM;

  return GP_SUCCESS;
}

Gp_Result _gp_read_byte_from_buffer(void *buffer, uint8_t *byte) {
  if (!buffer || !byte) return GP_INVALID_ARGS;

  _Gp_Buffer *_gp_buffer = (_Gp_Buffer*)buffer;
  if (_gp_buffer->current >= _gp_buffer->count) return GP_UNDERFLOW;
  *byte = _gp_buffer->data[_gp_buffer->current++];

  return GP_SUCCESS;
}

Gp_Result _gp_write_byte_to_buffer(void *buffer, uint8_t byte) {
  if (!buffer) return GP_INVALID_ARGS;

  _Gp_Buffer *_gp_buffer = (_Gp_Buffer*)buffer;
  if (_gp_buffer->count >= _gp_buffer->capacity) {
    if (_gp_buffer->capacity) _gp_buffer->capacity <<= 1;
    else _gp_buffer->capacity = 64;

    _gp_buffer->data = realloc(_gp_buffer->data, _gp_buffer->capacity);
    if (!_gp_buffer->data) return GP_BUY_MORE_RAM;
  }

  if (!_gp_buffer->data) return GP_INVALID_ARGS;
  _gp_buffer->data[_gp_buffer->count++] = byte;

  return GP_SUCCESS;
}

Gp_Result _gp_shift_buffer(_Gp_Buffer *buffer, size_t offset) {
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

Gp_Result _gp_copy_buffer(_Gp_Buffer *dst, const _Gp_Buffer *src, size_t position) {
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