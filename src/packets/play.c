#include "../packets.h"

Gp_Result _gp_parse_packet_play(void *buffer, Gp_Varint packetId, Gp_Packet **packet) {
  if (!buffer || !packet) return GP_INVALID_ARGS;
  return GP_SUCCESS;
}

Gp_Result _gp_write_packet_play(void *buffer, Gp_Packet *packet) {
  if (!buffer || !packet) return GP_INVALID_ARGS;
  return GP_SUCCESS;
}

Gp_Result _gp_handle_packet_play(Gp_Bot *bot, Gp_Packet *packet) {
  if (!bot || !packet) return GP_INVALID_ARGS;

  bot->state = GP_BOT_OFFLINE;

  // switch (packet->packetID) { // Assume clientbound packets only
  // case
  // }

  return GP_SUCCESS;
}