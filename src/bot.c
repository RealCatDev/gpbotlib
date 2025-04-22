#include "gpbotlib/bot.h"

#include "gpbotlib/packet.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifndef   max
#define   max(a, b) (a)>(b)?(a):(b)
#endif // max

typedef struct _Gp_Buffer {
  char *data;
  size_t current;
  size_t count;
  size_t capacity;
} _Gp_Buffer;

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

Gp_Result _gp_bot_handshake(Gp_Bot *, bool, Gp_Packet **);

Gp_Result _gp_bot_send_packet(Gp_Bot *, Gp_Packet *, Gp_Write_Packet_Data);


Gp_Result gp_bot_join(Gp_Bot *bot) {
  if (!bot ||
      !bot->send ||
      !bot->recv) return GP_INVALID_ARGS;
  Gp_Result result = GP_SUCCESS;

  Gp_Packet *handshake = NULL;
  if ((result = _gp_bot_handshake(bot, true, &handshake)) < GP_SUCCESS) return result;
  if ((result = _gp_bot_send_packet(bot, handshake, gp_write_handshake_packet_data)) < GP_SUCCESS) return result;

  return GP_SUCCESS;
}

Gp_Result gp_bot_leave(Gp_Bot *bot) {
  if (!bot) return GP_INVALID_ARGS;

  return GP_SUCCESS;
}



Gp_Result _gp_bot_handshake(Gp_Bot *bot, bool status, Gp_Packet **packet) {
  if (!bot || !packet) return GP_INVALID_ARGS;

  Gp_Handshake_Packet_Data data = {
    .protocolVersion = (Gp_Varint)bot->version,
    .serverAddress = {0},
    .serverPort = 0,
    .nextState = 1+(!status)
  };

  *packet = gp_packet_create(0, sizeof(data));
  if (!(*packet)) return GP_BUY_MORE_RAM;

  memcpy((*packet)->data, &data, sizeof(data));

  return GP_SUCCESS;
}

Gp_Result _gp_bot_send_packet(Gp_Bot *bot, Gp_Packet *packet, Gp_Write_Packet_Data write) {
  if (!bot || !packet) return GP_INVALID_ARGS;

  Gp_Result result = GP_SUCCESS;

  _Gp_Buffer sendBuffer = {0};
  if ((result = gp_write_varint(&sendBuffer, packet->packetID, _gp_write_byte_to_buffer)) < GP_SUCCESS) return result;
  if ((result = write(&sendBuffer, &packet->data, _gp_write_byte_to_buffer)) < GP_SUCCESS) return result;

  _Gp_Buffer lengthBuffer = {0};
  if ((result = gp_write_varint(&lengthBuffer, sendBuffer.count, _gp_write_byte_to_buffer)) < GP_SUCCESS) return result;

  if ((result = _gp_shift_buffer(&sendBuffer, lengthBuffer.count)) < GP_SUCCESS) return result;
  if ((result = _gp_copy_buffer(&sendBuffer, &lengthBuffer, 0)) < GP_SUCCESS) return result;

  free(lengthBuffer.data);

  bot->send(bot->data, sendBuffer.data, sendBuffer.count);

  free(sendBuffer.data);

  return GP_SUCCESS;
}