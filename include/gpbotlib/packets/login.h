#ifndef   _GPBOTLIB_PACKETS_LOGIN_H_
#define   _GPBOTLIB_PACKETS_LOGIN_H_

#include <gpbotlib/packet.h>

#include <stdbool.h>

typedef struct Gp_Disconnect_Packet_Data {
  Gp_String reason;
} Gp_Disconnect_Packet_Data;

Gp_Result gp_parse_disconnect_packet_data(void *buffer, Gp_Packet **packet, Gp_Read_Byte_From_Buffer read);

typedef struct Gp_Encryption_Request_Packet_Data {
  Gp_String serverId;
  Gp_String publicKey;
  Gp_String verifyToken;
} Gp_Encryption_Request_Packet_Data;

Gp_Result gp_parse_encryption_request_packet_data(void *buffer, Gp_Packet **packet, Gp_Read_Byte_From_Buffer read);

typedef struct Gp_Login_Success_Packet_Data {
  Gp_Uuid uuid;
  Gp_String username;
} Gp_Login_Success_Packet_Data;

Gp_Result gp_parse_login_success_packet_data(void *buffer, Gp_Packet **packet, Gp_Read_Byte_From_Buffer read);

typedef struct Gp_Set_Compression_Packet_Data {
  Gp_Varint threshold;
} Gp_Set_Compression_Packet_Data;

Gp_Result gp_parse_set_compression_packet_data(void *buffer, Gp_Packet **packet, Gp_Read_Byte_From_Buffer read);

typedef struct Gp_Login_Plugin_Request_Packet_Data {
  Gp_Varint messageId;
  Gp_String channel;
  size_t dataLength;
  char data[];
} Gp_Login_Plugin_Request_Packet_Data;

Gp_Result gp_parse_login_plugin_request_packet_data(void *buffer, Gp_Packet **packet, Gp_Read_Byte_From_Buffer read);

typedef struct Gp_Login_Start_Packet_Data {
  Gp_String username;
} Gp_Login_Start_Packet_Data;

Gp_Result gp_write_login_start_packet_data(void *buffer, void *data, Gp_Write_Byte_To_Buffer write);
Gp_Packet *gp_create_login_start_packet(Gp_String username);

typedef struct Gp_Encryption_Response_Packet_Data {
  Gp_String sharedSecret;
  Gp_String verifyToken;
} Gp_Encryption_Response_Packet_Data;

Gp_Result gp_write_encryption_response_packet_data(void *buffer, void *data, Gp_Write_Byte_To_Buffer write);
Gp_Packet *gp_create_encryption_response_packet(Gp_String sharedSecret, Gp_String verifyToken);

typedef struct Gp_Login_Plugin_Response_Packet_Data {
  Gp_Varint messageId;
  bool successful;
  size_t dataLength;
  char data[];
} Gp_Login_Plugin_Response_Packet_Data;

Gp_Result gp_write_login_plugin_response_packet_data(void *buffer, void *data, Gp_Write_Byte_To_Buffer write);
Gp_Packet *gp_create_login_plugin_response_packet(Gp_Varint messageId, bool successful, size_t dataLength, const char *data);

#endif // _GPBOTLIB_PACKETS_LOGIN_H_