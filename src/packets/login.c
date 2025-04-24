#include "../packets.h"

#include "../buffer.h"

#include <assert.h>

Gp_Result gp_parse_disconnect_packet_data(void *buffer, Gp_Packet **packet, Gp_Read_Byte_From_Buffer read) {
  if (!buffer || !packet || !read) return GP_INVALID_ARGS;

  *packet = gp_packet_create(0, sizeof(Gp_Disconnect_Packet_Data));
  if (!*packet) return GP_BUY_MORE_RAM;
  Gp_Disconnect_Packet_Data *disconnect = (Gp_Disconnect_Packet_Data*)(*packet)->data;
  return gp_parse_string(buffer, &disconnect->reason, read);
}

Gp_Result gp_parse_encryption_request_packet_data(void *buffer, Gp_Packet **packet, Gp_Read_Byte_From_Buffer read) {
  if (!buffer || !packet || !read) return GP_INVALID_ARGS;

  *packet = gp_packet_create(1, sizeof(Gp_Encryption_Request_Packet_Data));
  if (!*packet) return GP_BUY_MORE_RAM;
  Gp_Encryption_Request_Packet_Data *request = (Gp_Encryption_Request_Packet_Data*)(*packet)->data;

  Gp_Result result = GP_SUCCESS;
  if ((result = gp_parse_string(buffer, &request->serverId, read)) < GP_SUCCESS) return result;
  if ((result = gp_parse_string(buffer, &request->publicKey, read)) < GP_SUCCESS) return result;
  return gp_parse_string(buffer, &request->verifyToken, read);
}

Gp_Result gp_parse_login_success_packet_data(void *buffer, Gp_Packet **packet, Gp_Read_Byte_From_Buffer read) {
  if (!buffer || !packet || !read) return GP_INVALID_ARGS;

  *packet = gp_packet_create(2, sizeof(Gp_Login_Success_Packet_Data));
  if (!*packet) return GP_BUY_MORE_RAM;
  Gp_Login_Success_Packet_Data *loginSuccess = (Gp_Login_Success_Packet_Data*)(*packet)->data;

  Gp_Result result = GP_SUCCESS;
  if ((result = gp_parse_uuid(buffer, &loginSuccess->uuid, read)) < GP_SUCCESS) return result;
  if ((result = gp_parse_string(buffer, &loginSuccess->username, read)) < GP_SUCCESS) return result;

  return GP_SUCCESS;
}

Gp_Result gp_parse_set_compression_packet_data(void *buffer, Gp_Packet **packet, Gp_Read_Byte_From_Buffer read) {
  if (!buffer || !packet || !read) return GP_INVALID_ARGS;

  *packet = gp_packet_create(3, sizeof(Gp_Set_Compression_Packet_Data));
  if (!*packet) return GP_BUY_MORE_RAM;
  Gp_Set_Compression_Packet_Data *setCompression = (Gp_Set_Compression_Packet_Data*)(*packet)->data;
  return gp_parse_varint(buffer, &setCompression->threshold, read);
}

Gp_Result gp_parse_login_plugin_request_packet_data(void *buffer, Gp_Packet **packet, Gp_Read_Byte_From_Buffer read) {
  if (!buffer || !packet || !read) return GP_INVALID_ARGS;

  Gp_Result result = GP_SUCCESS;

  _Gp_Buffer *_buffer = (_Gp_Buffer*)buffer;

  Gp_Login_Plugin_Request_Packet_Data data = {0};
  if ((result = gp_parse_varint(buffer, &data.messageId, read)) < GP_SUCCESS) return result;
  if ((result = gp_parse_string(buffer, &data.channel, read)) < GP_SUCCESS) return result;
  data.dataLength = _buffer->count-_buffer->current;

  *packet = gp_packet_create(4, sizeof(Gp_Login_Plugin_Request_Packet_Data)+data.dataLength);
  if (!*packet) return GP_BUY_MORE_RAM;
  memcpy((*packet)->data, &data, sizeof(data));
  memcpy(&(*packet)->data[offsetof(Gp_Login_Plugin_Request_Packet_Data, data)], &_buffer->data[_buffer->current], data.dataLength);

  return GP_SUCCESS;
}

Gp_Result gp_write_login_start_packet_data(void *buffer, void *data, Gp_Write_Byte_To_Buffer write) {
  if (!buffer || !data || !write) return GP_INVALID_ARGS;
  Gp_Login_Start_Packet_Data loginStart = *(Gp_Login_Start_Packet_Data*)data;
  return gp_write_string(buffer, loginStart.username, write);
}

Gp_Packet *gp_create_login_start_packet(Gp_String username) {
  Gp_Packet *packet = gp_packet_create(0, sizeof(Gp_Login_Start_Packet_Data));
  if (!packet) return packet;

  Gp_Login_Start_Packet_Data *data = (Gp_Login_Start_Packet_Data*)packet->data;
  data->username = username;

  return packet;
}

Gp_Result gp_write_encryption_response_packet_data(void *buffer, void *data, Gp_Write_Byte_To_Buffer write) {
  if (!buffer || !data || !write) return GP_INVALID_ARGS;

  Gp_Result result = GP_SUCCESS;
  Gp_Encryption_Response_Packet_Data encryptionResponse = *(Gp_Encryption_Response_Packet_Data*)data;
  if ((result = gp_write_string(buffer, encryptionResponse.sharedSecret, write)) < GP_SUCCESS) return result;
  return gp_write_string(buffer, encryptionResponse.verifyToken, write);
}

Gp_Packet *gp_create_encryption_response_packet(Gp_String sharedSecret, Gp_String verifyToken) {
  Gp_Packet *packet = gp_packet_create(0, sizeof(Gp_Encryption_Response_Packet_Data));
  if (!packet) return packet;

  Gp_Encryption_Response_Packet_Data *data = (Gp_Encryption_Response_Packet_Data*)packet->data;
  data->sharedSecret = sharedSecret;
  data->verifyToken = verifyToken;

  return packet;
}

Gp_Result gp_write_login_plugin_response_packet_data(void *buffer, void *data, Gp_Write_Byte_To_Buffer write) {
  if (!buffer || !data || !write) return GP_INVALID_ARGS;

  Gp_Result result = GP_SUCCESS;
  Gp_Login_Plugin_Response_Packet_Data loginPluginResponse = *(Gp_Login_Plugin_Response_Packet_Data*)data;
  if ((result = gp_write_varint(buffer, loginPluginResponse.messageId, write)) < GP_SUCCESS) return result;
  if ((result = write(buffer, loginPluginResponse.successful)) < GP_SUCCESS) return result;

  for (size_t i = 0; i < loginPluginResponse.dataLength && result >= GP_SUCCESS; ++i)
    result = write(buffer, loginPluginResponse.data[i]);

  return result;
}

Gp_Packet *gp_create_login_plugin_response_packet(Gp_Varint messageId, bool successful, size_t dataLength, const char *payloadData) {
  Gp_Packet *packet = gp_packet_create(0, sizeof(Gp_Login_Plugin_Response_Packet_Data)+dataLength);
  if (!packet) return packet;

  Gp_Login_Plugin_Response_Packet_Data *data = (Gp_Login_Plugin_Response_Packet_Data*)packet->data;
  data->messageId = messageId;
  data->successful = successful;
  data->dataLength = dataLength;
  memcpy(data->data, payloadData, dataLength);

  return GP_SUCCESS;
}



Gp_Result _gp_parse_packet_login(void *buffer, Gp_Varint packetId, Gp_Packet **packet, Gp_Read_Byte_From_Buffer read) { // Clientbound
  if (!buffer || !packet || !read) return GP_INVALID_ARGS;

  Gp_Result result = GP_SUCCESS;
  switch (packetId) {
  case 0: { // Disconnect
    result = gp_parse_disconnect_packet_data(buffer, packet, read);
  } break;
  case 1: { // Encryption request
    result = gp_parse_encryption_request_packet_data(buffer, packet, read);
  } break;
  case 2: { // Login success
    result = gp_parse_login_success_packet_data(buffer, packet, read);
  } break;
  case 3: { // Set compression
    result = gp_parse_set_compression_packet_data(buffer, packet, read);
  } break;
  case 4: { // Login plugin request
    result = gp_parse_login_plugin_request_packet_data(buffer, packet, read);
  } break;
  default: return GP_INVALID_PACKET;
  }

  return result;
}

Gp_Result _gp_write_packet_login(void *buffer, Gp_Packet *packet, Gp_Write_Byte_To_Buffer write) { // Serverbound
  if (!buffer || !packet || !write) return GP_INVALID_ARGS;

  Gp_Result result = GP_SUCCESS;
  switch (packet->packetID) {
  case 0: { // Login start
    result = gp_write_login_start_packet_data(buffer, packet->data, write);
  } break;
  case 1: { // Encryption response
    result = gp_write_encryption_response_packet_data(buffer, packet->data, write);
  } break;
  case 2: { // Login plugin response
    result = gp_write_login_plugin_response_packet_data(buffer, packet->data, write);
  } break;
  default: return GP_INVALID_PACKET;
  }

  return result;
}

Gp_Result _gp_handle_packet_login(Gp_Bot *bot, Gp_Packet *packet) {
  if (!bot || !packet) return GP_INVALID_ARGS;

  switch (packet->packetID) {
  case 0: { // Disconnect
    Gp_Disconnect_Packet_Data data = *(Gp_Disconnect_Packet_Data*)packet->data;

    bot->state = GP_BOT_OFFLINE;
    return _gp_push_event(&bot->eventQueue, (Gp_Event){
      .type = GP_EVENT_DISCONNECT,
      .as.disconnect.reason = data.reason
    });
  } break;
  case 1: { // Encryption request
    assert(0 && "Unimplemented");
  } break;
  case 2: { // Login success
    bot->state = GP_BOT_PLAY;
    return _gp_push_event(&bot->eventQueue, (Gp_Event){
      .type = GP_EVENT_JOIN
    });
  } break;
  case 3: { // Set compression
    assert(0 && "Unimplemented");
  } break;
  case 4: { // Login plugin request
    assert(0 && "Unimplemented");
  } break;
  default: return GP_INVALID_PACKET;
  }

  return GP_SUCCESS;
}