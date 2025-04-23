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

Gp_Result gp_parse_handshake_packet_data(void *buffer, void **data, Gp_Read_Byte_From_Buffer read) {
  if (!buffer || !data || !read) return GP_INVALID_ARGS;
  Gp_Handshake_Packet_Data *handshake = *(Gp_Handshake_Packet_Data**)data;

  Gp_Result result = GP_SUCCESS;
  if ((result = gp_parse_varint(buffer, &handshake->protocolVersion, read)) < GP_SUCCESS) return result;
  if ((result = gp_parse_string(buffer, &handshake->serverAddress, read)) < GP_SUCCESS) return result;

  uint8_t byte = 0;
  if ((result = read(buffer, &byte)) < GP_SUCCESS) return result;
  handshake->serverPort = byte;
  if ((result = read(buffer, &byte)) < GP_SUCCESS) return result;
  handshake->serverPort |= byte<<8;

  if ((result = gp_parse_varint(buffer, &handshake->nextState, read)) < GP_SUCCESS) return result;

  return GP_SUCCESS;
}

Gp_Result gp_write_handshake_packet_data(void *buffer, void *data, Gp_Write_Byte_To_Buffer write) {
  if (!buffer || !data || !write) return GP_INVALID_ARGS;
  Gp_Handshake_Packet_Data handshake = *(Gp_Handshake_Packet_Data*)data;

  Gp_Result result = GP_SUCCESS;
  if ((result = gp_write_varint(buffer, handshake.protocolVersion, write)) < GP_SUCCESS) return result;
  if ((result = gp_write_string(buffer, handshake.serverAddress, write)) < GP_SUCCESS) return result;

  if ((result = write(buffer, handshake.serverPort & 0xFF)) < GP_SUCCESS) return result;
  if ((result = write(buffer, (handshake.serverPort>>8) & 0xFF)) < GP_SUCCESS) return result;

  if ((result = gp_write_varint(buffer, handshake.nextState, write)) < GP_SUCCESS) return result;

  return GP_SUCCESS;
}

Gp_Result gp_parse_login_start_packet_data(void *buffer, void **data, Gp_Read_Byte_From_Buffer read) {
  if (!buffer || !data || !read) return GP_INVALID_ARGS;
  Gp_Login_Start_Packet_Data *loginStart = *(Gp_Login_Start_Packet_Data**)data;
  return gp_parse_string(buffer, &loginStart->name, read);
}

Gp_Result gp_write_login_start_packet_data(void *buffer, void *data, Gp_Write_Byte_To_Buffer write) {
  if (!buffer || !data || !write) return GP_INVALID_ARGS;
  Gp_Login_Start_Packet_Data loginStart = *(Gp_Login_Start_Packet_Data*)data;
  return gp_write_string(buffer, loginStart.name, write);
}

Gp_Result gp_parse_login_success_packet_data(void *buffer, void **data, Gp_Read_Byte_From_Buffer read) {
  if (!buffer || !data || !read) return GP_INVALID_ARGS;
  Gp_Login_Success_Packet_Data *loginSuccess = *(Gp_Login_Success_Packet_Data**)data;

  Gp_Result result = GP_SUCCESS;
  if ((result = gp_parse_uuid(buffer, &loginSuccess->uuid, read)) < GP_SUCCESS) return result;
  if ((result = gp_parse_string(buffer, &loginSuccess->username, read)) < GP_SUCCESS) return result;

  return GP_SUCCESS;
}

Gp_Result gp_write_login_success_packet_data(void *buffer, void *data, Gp_Write_Byte_To_Buffer write) {
  if (!buffer || !data || !write) return GP_INVALID_ARGS;
  Gp_Login_Success_Packet_Data loginSuccess = *(Gp_Login_Success_Packet_Data*)data;

  Gp_Result result = GP_SUCCESS;
  if ((result = gp_write_uuid(buffer, loginSuccess.uuid, write)) < GP_SUCCESS) return result;
  if ((result = gp_write_string(buffer, loginSuccess.username, write)) < GP_SUCCESS) return result;

  return GP_SUCCESS;
}

Gp_Result gp_parse_login_set_compression_packet_data(void *buffer, void **data, Gp_Read_Byte_From_Buffer read) {
  if (!buffer || !data || !read) return GP_INVALID_ARGS;
  Gp_Login_Set_Compression_Packet_Data *loginSetCompression = *(Gp_Login_Set_Compression_Packet_Data**)data;

  Gp_Result result = GP_SUCCESS;
  if ((result = gp_parse_varint(buffer, &loginSetCompression->maximumNonCompressedPacket, read)) < GP_SUCCESS) return result;

  return GP_SUCCESS;
}

Gp_Result gp_write_login_set_compression_packet_data(void *buffer, void *data, Gp_Write_Byte_To_Buffer write) {
  
  if (!buffer || !data || !write) return GP_INVALID_ARGS;
  Gp_Login_Set_Compression_Packet_Data loginSetCompression = *(Gp_Login_Set_Compression_Packet_Data*)data;

  Gp_Result result = GP_SUCCESS;
  if ((result = gp_write_varint(buffer, loginSetCompression.maximumNonCompressedPacket, write)) < GP_SUCCESS) return result;

  return GP_SUCCESS;
}
