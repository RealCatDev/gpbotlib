#include "gpbotlib/bot.h"

#include "gpbotlib/packet.h"
#include "gpbotlib/packets/handshake.h"
// #include "gpbotlib/packets/status.h"
#include "gpbotlib/packets/login.h"
// #include "gpbotlib/packets/play.h"

#include "buffer.h"
#include "packets.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#ifndef   max
#define   max(a, b) (a)>(b)?(a):(b)
#endif // max

Gp_Result _gp_bot_handshake(Gp_Bot *, Gp_Bot_State, Gp_Packet **);
Gp_Result _gp_bot_login_start(Gp_Bot *, Gp_String, Gp_Packet **);

Gp_Result _gp_bot_send_packet(Gp_Bot *, Gp_Packet *);
Gp_Result _gp_bot_recv_packet(Gp_Bot *, Gp_Packet **);



Gp_Result gp_bot_join(Gp_Bot *bot) {
  if (!bot ||
      !bot->send ||
      !bot->recv) return GP_INVALID_ARGS;
  Gp_Result result = GP_SUCCESS;

  { // Handshake
    Gp_Packet *handshake = gp_create_handshake_packet(bot->version, GP_STRING(""), 0, GP_BOT_LOGIN);
    if ((result = _gp_bot_send_packet(bot, handshake)) < GP_SUCCESS) return result;
    free(handshake);
  }

  bot->state = GP_BOT_LOGIN;
  { // Login start
    Gp_Packet *loginStart = gp_create_login_start_packet(bot->username);
    if ((result = _gp_bot_send_packet(bot, loginStart)) < GP_SUCCESS) return result;
    free(loginStart);
  }

  return GP_SUCCESS;
}

Gp_Result gp_bot_leave(Gp_Bot *bot) {
  if (!bot) return GP_INVALID_ARGS;

  return GP_SUCCESS;
}

Gp_Result gp_bot_update(Gp_Bot *bot) {
  if (!bot) return GP_INVALID_ARGS;

  Gp_Result result = GP_SUCCESS;

  Gp_Packet *packet = NULL;
  if ((result = _gp_bot_recv_packet(bot, &packet)) < GP_SUCCESS) return result;

  switch (bot->state) {
  case GP_BOT_LOGIN: {
    result = _gp_handle_packet_login(bot, packet);
  } break;
  case GP_BOT_PLAY: {
    result = _gp_handle_packet_play(bot, packet);
  } break;
  default: assert(0 && "Unreachable");
  }

  return GP_SUCCESS;
}

Gp_Result gp_bot_is_offline(Gp_Bot *bot) {
  if (!bot) return GP_INVALID_ARGS;
  return bot->state == GP_BOT_OFFLINE;
}



Gp_Result _gp_bot_handshake(Gp_Bot *bot, Gp_Bot_State nextState, Gp_Packet **packet) {
  if (!bot || !packet) return GP_INVALID_ARGS;

  Gp_Handshake_Packet_Data data = {
    .protocolVersion = (Gp_Varint)bot->version,
    .serverAddress = {0},
    .serverPort = 0,
    .nextState = nextState
  };

  *packet = gp_packet_create(0, sizeof(data));
  if (!(*packet)) return GP_BUY_MORE_RAM;

  memcpy((*packet)->data, &data, sizeof(data));

  return GP_SUCCESS;
}

Gp_Result _gp_bot_login_start(Gp_Bot *bot, Gp_String name, Gp_Packet **packet) {
  if (!bot || !packet) return GP_INVALID_ARGS;

  Gp_Login_Start_Packet_Data data = {
    .username = name
  };

  *packet = gp_packet_create(0, sizeof(data));
  if (!(*packet)) return GP_BUY_MORE_RAM;

  memcpy((*packet)->data, &data, sizeof(data));

  return GP_SUCCESS;
}

Gp_Result _gp_bot_send_packet(Gp_Bot *bot, Gp_Packet *packet) {
  if (!bot || !packet) return GP_INVALID_ARGS;

  Gp_Result result = GP_SUCCESS;

  _Gp_Buffer sendBuffer = {0};
  if ((result = gp_write_varint(&sendBuffer, packet->packetID, _gp_write_byte_to_buffer)) < GP_SUCCESS) return result;

  switch (bot->state) {
  case GP_BOT_HANDSHAKE: {
    result = _gp_write_packet_handshake(&sendBuffer, packet, _gp_write_byte_to_buffer);
  } break;
  case GP_BOT_STATUS: {
    result = _gp_write_packet_status(&sendBuffer, packet, _gp_write_byte_to_buffer);
  } break;
  case GP_BOT_LOGIN: {
    result = _gp_write_packet_login(&sendBuffer, packet, _gp_write_byte_to_buffer);
  } break;
  case GP_BOT_PLAY: {
    result = _gp_write_packet_play(&sendBuffer, packet, _gp_write_byte_to_buffer);
  } break;
  default: assert(0);
  }

  _Gp_Buffer lengthBuffer = {0};
  if ((result = gp_write_varint(&lengthBuffer, sendBuffer.count, _gp_write_byte_to_buffer)) < GP_SUCCESS) return result;

  if ((result = _gp_shift_buffer(&sendBuffer, lengthBuffer.count)) < GP_SUCCESS) return result;
  if ((result = _gp_copy_buffer(&sendBuffer, &lengthBuffer, 0)) < GP_SUCCESS) return result;

  free(lengthBuffer.data);

  bot->send(bot->data, sendBuffer.data, sendBuffer.count);

  free(sendBuffer.data);

  return GP_SUCCESS;
}

Gp_Result _gp_bot_recv_packet(Gp_Bot *bot, Gp_Packet **packet) {
  if (!bot || !packet) return GP_INVALID_ARGS;

  Gp_Result result = GP_SUCCESS;

  _Gp_Buffer recvBuffer = {0};

  Gp_Varint length = 0;
  size_t lengthSize = 0;
  do {
    if ((result = _gp_reserve_buffer(&recvBuffer, 1024)) < GP_SUCCESS) return result;
    int n = bot->recv(bot->data, &recvBuffer.data[recvBuffer.count], recvBuffer.capacity-recvBuffer.count);
    if (n < 0) return GP_INTERNAL_ERROR;
    else if (!n) return GP_DISCONNECTED;

    recvBuffer.count += n;

    if (!lengthSize) {
      if ((result = gp_parse_varint(&recvBuffer, &length, _gp_read_byte_from_buffer)) < GP_SUCCESS) return result;
      memmove(recvBuffer.data, &recvBuffer.data[lengthSize = recvBuffer.current], recvBuffer.count -= lengthSize);
      recvBuffer.current = 0;
    }
  } while (recvBuffer.count < length+lengthSize);

  Gp_Varint packetId = 0;
  if ((result = gp_parse_varint(&recvBuffer, &packetId, _gp_read_byte_from_buffer)) < GP_SUCCESS) return result;

  fprintf(stderr, "recv packet! STATE: %d, PACKET ID: %d\n", bot->state, packetId);

  switch (bot->state) {
  case GP_BOT_HANDSHAKE: {
    result = _gp_parse_packet_handshake(&recvBuffer, packetId, packet, _gp_read_byte_from_buffer);
  } break;
  case GP_BOT_STATUS: {
    result = _gp_parse_packet_status(&recvBuffer, packetId, packet, _gp_read_byte_from_buffer);
  } break;
  case GP_BOT_LOGIN: {
    result = _gp_parse_packet_login(&recvBuffer, packetId, packet, _gp_read_byte_from_buffer);
  } break;
  case GP_BOT_PLAY: {
    result = _gp_parse_packet_play(&recvBuffer, packetId, packet, _gp_read_byte_from_buffer);
  } break;
  default: assert(0);
  }

  free(recvBuffer.data);

  return GP_SUCCESS;
}