#include "gpbotlib/bot.h"

#include "gpbotlib/packet.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct _Gp_Buffer {
  char *data;
  size_t current;
  size_t count;
  size_t capacity;
} _Gp_Buffer;

Gp_Result _gp_read_byte_from_buffer(void *buffer, uint8_t *byte) {
  if (!buffer || !byte) return GP_INVALID_ARGS;

  _Gp_Buffer _gp_buffer = *(_Gp_Buffer*)buffer;
  if (_gp_buffer.current >= _gp_buffer.count) return GP_UNDERFLOW;
  *byte = _gp_buffer.data[_gp_buffer.current++];

  return GP_SUCCESS;
}

Gp_Result _gp_write_byte_to_buffer(void *buffer, uint8_t byte) {
  if (!buffer) return GP_INVALID_ARGS;

  _Gp_Buffer _gp_buffer = *(_Gp_Buffer*)buffer;
  if (_gp_buffer.count >= _gp_buffer.capacity) {
    if (_gp_buffer.capacity) _gp_buffer.capacity <<= 1;
    else _gp_buffer.capacity = 64;

    _gp_buffer.data = realloc(_gp_buffer.data, _gp_buffer.capacity);
    if (!_gp_buffer.data) return GP_BUY_MORE_RAM;
  }

  if (!_gp_buffer.data) return GP_INVALID_ARGS;
  _gp_buffer.data[_gp_buffer.count++] = byte;

  return GP_SUCCESS;
}

Gp_Result _gp_bot_handshake(Gp_Bot *, bool, Gp_Packet **);



Gp_Result gp_bot_join(Gp_Bot *bot) {
  if (!bot ||
      !bot->send ||
      !bot->recv) return GP_INVALID_ARGS;
  Gp_Result result = GP_SUCCESS;

  Gp_Packet *handshake = NULL;
  if ((result = _gp_bot_handshake(bot, false, &handshake)) < GP_SUCCESS) return result;

  _Gp_Buffer sendBuffer = {0};
  if ((result = gp_write_varint(&sendBuffer, handshake->length, _gp_write_byte_to_buffer)) < GP_SUCCESS) return result;
  if ((result = gp_write_varint(&sendBuffer, handshake->packetID, _gp_write_byte_to_buffer)) < GP_SUCCESS) return result;

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