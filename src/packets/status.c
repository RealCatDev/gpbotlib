#include "../packets.h"

#include <assert.h>

// We do not use status state atm so we might as well not implement any status packets :3

Gp_Result _gp_parse_packet_status(void *buffer, Gp_Varint packetId, Gp_Packet **packet) {
  assert(0 && "Unimplemented");
  return GP_INTERNAL_ERROR;
}

Gp_Result _gp_write_packet_status(void *buffer, Gp_Packet *packet) {
  assert(0 && "Unimplemented");
  return GP_INTERNAL_ERROR;
}