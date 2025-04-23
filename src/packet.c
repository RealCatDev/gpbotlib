#include "gpbotlib/packet.h"
#include "gpbotlib/packets/handshake.h"
// #include "gpbotlib/packets/status.h"
#include "gpbotlib/packets/login.h"
// #include "gpbotlib/packets/play.h"

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