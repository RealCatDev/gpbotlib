#ifndef   _GPBOTLIB_PACKET_H_
#define   _GPBOTLIB_PACKET_H_

#include "varint.h"

typedef struct Gp_Packet { // https://minecraft.wiki/w/Minecraft_Wiki:Projects/wiki.vg_merge/Protocol?oldid=2772783#Packet_format
  Gp_Varint length;
  Gp_Varint packetID;
  char data[];
} Gp_Packet;

Gp_Packet *gp_packet_create(Gp_Varint packetID, Gp_Varint length);

typedef struct Gp_Packet_String {
  Gp_Varint length;
  char *data;
} Gp_Packet_String;

typedef struct Gp_Handshake_Packet_Data { // https://minecraft.wiki/w/Minecraft_Wiki:Projects/wiki.vg_merge/Protocol?oldid=2772783#Handshake
  Gp_Varint protocolVersion;
  Gp_Packet_String serverAddress;
  uint16_t serverPort;
  Gp_Varint nextState;
} Gp_Handshake_Packet_Data;

#endif // _GPBOTLIB_PACKET_H_