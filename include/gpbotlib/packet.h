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

#endif // _GPBOTLIB_PACKET_H_