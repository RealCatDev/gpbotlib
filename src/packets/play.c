#include "../packets.h"

Gp_Result _gp_parse_packet_play(void *buffer, Gp_Varint packetId, Gp_Packet **packet, Gp_Read_Byte_From_Buffer read) {
  if (!buffer || !packet || !read) return GP_INVALID_ARGS;
  return GP_SUCCESS;
}

Gp_Result _gp_write_packet_play(void *buffer, Gp_Packet *packet, Gp_Write_Byte_To_Buffer write) {
  if (!buffer || !packet || !write) return GP_INVALID_ARGS;
  return GP_SUCCESS;
}

Gp_Result _gp_handle_packet_play(Gp_Bot *bot, Gp_Packet *packet) {
  if (!bot || !packet) return GP_INVALID_ARGS;

  // switch (packet->packetID) { // Assume clientbound packets only
  // case
  // }

  return GP_SUCCESS;
}