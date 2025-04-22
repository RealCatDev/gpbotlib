#include "gpbotlib/packet.h"

#include <stdlib.h>
#include <string.h>

Gp_Packet *gp_packet_create(Gp_Varint packetID, Gp_Varint length) {
  Gp_Packet *packet = malloc(sizeof(*packet) + length);
  if (!packet) return NULL;
  memset(packet, 0, sizeof(*packet));

  packet->packetID = packetID;
  packet->length = length;

  return packet;
}

Gp_Result gp_parse_handshake_packet_data(void *buffer, Gp_Handshake_Packet_Data *data, Gp_Read_Byte_From_Buffer read) {
  if (!buffer || !data || !read) return GP_INVALID_ARGS;

  Gp_Result result = GP_SUCCESS;
  if ((result = gp_parse_varint(buffer, &data->protocolVersion, read)) < GP_SUCCESS) return result;
  if ((result = gp_parse_string(buffer, &data->serverAddress, read)) < GP_SUCCESS) return result;

  uint8_t byte = 0;
  if ((result = read(buffer, &byte)) < GP_SUCCESS) return result;
  data->serverPort = byte;
  if ((result = read(buffer, &byte)) < GP_SUCCESS) return result;
  data->serverPort |= byte<<8;

  if ((result = gp_parse_varint(buffer, &data->nextState, read)) < GP_SUCCESS) return result;

  return GP_SUCCESS;
}

Gp_Result gp_write_handshake_packet_data(void *buffer, Gp_Handshake_Packet_Data data, Gp_Write_Byte_To_Buffer write) {
  if (!buffer || !write) return GP_INVALID_ARGS;

  Gp_Result result = GP_SUCCESS;
  if ((result = gp_write_varint(buffer, data.protocolVersion, write)) < GP_SUCCESS) return result;
  if ((result = gp_write_string(buffer, data.serverAddress, write)) < GP_SUCCESS) return result;

  if ((result = write(buffer, data.serverPort & 0xFF)) < GP_SUCCESS) return result;
  if ((result = write(buffer, (data.serverPort>>8) & 0xFF)) < GP_SUCCESS) return result;

  if ((result = gp_write_varint(buffer, data.nextState, write)) < GP_SUCCESS) return result;

  return GP_SUCCESS;
}

void gp_handshake_packet_free(Gp_Handshake_Packet_Data *data) {
  if (!data) return;

  gp_string_free(&data->serverAddress);
}