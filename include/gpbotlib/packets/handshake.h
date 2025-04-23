#ifndef   _GPBOTLIB_PACKETS_HANDSHAKE_H_
#define   _GPBOTLIB_PACKETS_HANDSHAKE_H_

#include <gpbotlib/packet.h>

typedef struct Gp_Handshake_Packet_Data { // https://minecraft.wiki/w/Minecraft_Wiki:Projects/wiki.vg_merge/Protocol?oldid=2772783#Handshake
  Gp_Varint protocolVersion;
  Gp_String serverAddress;
  uint16_t serverPort;
  Gp_Varint nextState;
} Gp_Handshake_Packet_Data;

Gp_Result gp_write_handshake_packet_data(void *buffer, void *data, Gp_Write_Byte_To_Buffer write);

Gp_Packet *gp_create_handshake_packet(Gp_Varint protocolVersion, Gp_String serverAddress, uint16_t serverPort, Gp_Varint nextState);

#endif // _GPBOTLIB_PACKETS_HANDSHAKE_H_