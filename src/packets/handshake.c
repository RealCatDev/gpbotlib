#include "../packets.h"

Gp_Result gp_write_handshake_packet_data(void *buffer, void *data) {
  if (!buffer || !data) return GP_INVALID_ARGS;
  Gp_Handshake_Packet_Data handshake = *(Gp_Handshake_Packet_Data*)data;

  Gp_Result result = GP_SUCCESS;
  if ((result = gp_write_varint(buffer, handshake.protocolVersion)) < GP_SUCCESS) return result;
  if ((result = gp_write_string(buffer, handshake.serverAddress)) < GP_SUCCESS) return result;

  if ((result = gp_write_uint16_to_buffer(buffer, handshake.serverPort)) < GP_SUCCESS) return result;

  if ((result = gp_write_varint(buffer, handshake.nextState)) < GP_SUCCESS) return result;

  return GP_SUCCESS;
}

Gp_Packet *gp_create_handshake_packet(Gp_Varint protocolVersion, Gp_String serverAddress, uint16_t serverPort, Gp_Varint nextState) {
  Gp_Packet *packet = gp_packet_create(0, sizeof(Gp_Handshake_Packet_Data));
  if (!packet) return packet;

  Gp_Handshake_Packet_Data *data = (Gp_Handshake_Packet_Data*)packet->data;
  data->protocolVersion = protocolVersion;
  data->serverAddress = serverAddress;
  data->serverPort = serverPort;
  data->nextState = nextState;

  return packet;
}



Gp_Result _gp_parse_packet_handshake(void *buffer, Gp_Varint packetId, Gp_Packet **packet) {
  return GP_INVALID_PACKET; // There aren't any serverbound handshake packets
}

Gp_Result _gp_write_packet_handshake(void *buffer, Gp_Packet *packet) {
  if (!buffer || !packet || packet->packetID) return GP_INVALID_ARGS;

  Gp_Result result = GP_SUCCESS;
  if ((result = gp_write_handshake_packet_data(buffer, packet->data)) < GP_SUCCESS) return result;

  return GP_SUCCESS;
}