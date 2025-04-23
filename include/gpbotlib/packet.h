#ifndef   _GPBOTLIB_PACKET_H_
#define   _GPBOTLIB_PACKET_H_

#include <gpbotlib/varint.h>
#include <gpbotlib/string.h>
#include <gpbotlib/uuid.h>

typedef struct Gp_Packet { // https://minecraft.wiki/w/Minecraft_Wiki:Projects/wiki.vg_merge/Protocol?oldid=2772783#Packet_format
  Gp_Varint length; // Size of data
  Gp_Varint packetID;
  char data[];
} Gp_Packet;

Gp_Packet *gp_packet_create(Gp_Varint packetID, Gp_Varint length);

typedef Gp_Result (*Gp_Parse_Packet_Data)(void *, void **, Gp_Read_Byte_From_Buffer);
typedef Gp_Result (*Gp_Write_Packet_Data)(void *, void *, Gp_Write_Byte_To_Buffer);

typedef struct Gp_Handshake_Packet_Data { // https://minecraft.wiki/w/Minecraft_Wiki:Projects/wiki.vg_merge/Protocol?oldid=2772783#Handshake
  Gp_Varint protocolVersion;
  Gp_String serverAddress;
  uint16_t serverPort;
  Gp_Varint nextState;
} Gp_Handshake_Packet_Data;

Gp_Result gp_parse_handshake_packet_data(void *buffer, void **data, Gp_Read_Byte_From_Buffer read);
Gp_Result gp_write_handshake_packet_data(void *buffer, void *data, Gp_Write_Byte_To_Buffer write);

typedef struct Gp_Login_Start_Packet_Data {
  Gp_String name;
} Gp_Login_Start_Packet_Data;

Gp_Result gp_parse_login_start_packet_data(void *buffer, void **data, Gp_Read_Byte_From_Buffer read);
Gp_Result gp_write_login_start_packet_data(void *buffer, void *data, Gp_Write_Byte_To_Buffer write);

typedef struct Gp_Login_Success_Packet_Data {
  Gp_Uuid uuid;
  Gp_String username;
} Gp_Login_Success_Packet_Data;

Gp_Result gp_parse_login_success_packet_data(void *buffer, void **data, Gp_Read_Byte_From_Buffer read);
Gp_Result gp_write_login_success_packet_data(void *buffer, void *data, Gp_Write_Byte_To_Buffer write);

typedef struct Gp_Login_Set_Compression_Packet_Data {
  Gp_Varint maximumNonCompressedPacket;
} Gp_Login_Set_Compression_Packet_Data;

Gp_Result gp_parse_login_set_compression_packet_data(void *buffer, void **data, Gp_Read_Byte_From_Buffer read);
Gp_Result gp_write_login_set_compression_packet_data(void *buffer, void *data, Gp_Write_Byte_To_Buffer write);


#endif // _GPBOTLIB_PACKET_H_
