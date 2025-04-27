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

  if ((result = _gp_reserve_buffer(&bot->recvBuffer, 1024)) < GP_SUCCESS) return result;

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

  return result;
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

  Gp_Buffer sendBuffer = {0};
  if ((result = gp_write_varint(&sendBuffer, packet->packetID)) < GP_SUCCESS) return result;

  switch (bot->state) {
  case GP_BOT_HANDSHAKE: {
    result = _gp_write_packet_handshake(&sendBuffer, packet);
  } break;
  case GP_BOT_STATUS: {
    result = _gp_write_packet_status(&sendBuffer, packet);
  } break;
  case GP_BOT_LOGIN: {
    result = _gp_write_packet_login(&sendBuffer, packet);
  } break;
  case GP_BOT_PLAY: {
    result = _gp_write_packet_play(&sendBuffer, packet);
  } break;
  default: assert(0);
  }

  Gp_Buffer lengthBuffer = {0};
  if ((result = gp_write_varint(&lengthBuffer, sendBuffer.count)) < GP_SUCCESS) return result;

  if ((result = _gp_shift_buffer(&sendBuffer, lengthBuffer.count)) < GP_SUCCESS) return result;
  if ((result = _gp_copy_buffer(&sendBuffer, &lengthBuffer, 0)) < GP_SUCCESS) return result;

  free(lengthBuffer.data);

  bot->send(bot->data, sendBuffer.data, sendBuffer.count);

  free(sendBuffer.data);

  return GP_SUCCESS;
}

Gp_Result _gp_bot_recv(Gp_Bot *bot) {
  int n = bot->recv(bot->data, &bot->recvBuffer.data[bot->recvBuffer.count], bot->recvBuffer.capacity-bot->recvBuffer.count);
  if (n < 0) return GP_INTERNAL_ERROR;
  else if (!n) {
    __debugbreak();
    return GP_DISCONNECTED;
  }
  bot->recvBuffer.count += n;

  return GP_SUCCESS;
}

Gp_Result _gp_bot_recv_packet(Gp_Bot *bot, Gp_Packet **packet) {
  if (!bot || !packet) return GP_INVALID_ARGS;

  Gp_Result result = GP_SUCCESS;

  Gp_Varint length = 0;
  size_t lengthSize = 0;

  size_t save = bot->recvBuffer.current;
  do {
    result = gp_parse_varint(&bot->recvBuffer, &length);
    if (result == GP_UNDERFLOW) {
      if (bot->recvBuffer.count == bot->recvBuffer.capacity && (result = _gp_reserve_buffer(&bot->recvBuffer, 1024)) < GP_SUCCESS) return result;

      bot->recvBuffer.current = save;
      if ((result = _gp_bot_recv(bot)) < GP_SUCCESS) return result;
    } else if (result < GP_SUCCESS) return result;
    else lengthSize = bot->recvBuffer.current;
  } while (!lengthSize);

  size_t needed = length + lengthSize;
  while (bot->recvBuffer.count < needed) {
    size_t remaining = needed - bot->recvBuffer.count;
    if ((result = _gp_reserve_buffer(&bot->recvBuffer, remaining)) < GP_SUCCESS) return result;
    if ((result = _gp_bot_recv(bot)) < GP_SUCCESS) return result;
  }

  printf("Current: %d\n", bot->recvBuffer.current);

  Gp_Varint packetId = 0;
  if ((result = gp_parse_varint(&bot->recvBuffer, &packetId)) < GP_SUCCESS) return result;

  printf("Bot received packet %d with length: %d, current state: %d\n", packetId, length, bot->state);

  switch (bot->state) {
  case GP_BOT_HANDSHAKE: {
    result = _gp_parse_packet_handshake(&bot->recvBuffer, packetId, packet);
  } break;
  case GP_BOT_STATUS: {
    result = _gp_parse_packet_status(&bot->recvBuffer, packetId, packet);
  } break;
  case GP_BOT_LOGIN: {
    result = _gp_parse_packet_login(&bot->recvBuffer, packetId, packet);
  } break;
  case GP_BOT_PLAY: {
    result = _gp_parse_packet_play(&bot->recvBuffer, packetId, packet);
  } break;
  default: assert(0);
  }

  if (result != GP_SUCCESS) return result;

  printf("Current: %d\n", bot->recvBuffer.current);

  memmove(bot->recvBuffer.data, &bot->recvBuffer.data[bot->recvBuffer.current], bot->recvBuffer.count -= length+lengthSize);
  bot->recvBuffer.current -= length+lengthSize;

  return GP_SUCCESS;
}