#ifndef   _GPBOTLIB_PACKETS_PLAY_H_
#define   _GPBOTLIB_PACKETS_PLAY_H_

#include <gpbotlib/packet.h>
#include <gpbotlib/string.h>
#include <gpbotlib/nbt.h>

#include <stdbool.h>

typedef struct Gp_Join_Game_Packet_Data {
  uint32_t entityID;
  uint8_t isHardcore;
  uint8_t gamemode;
  int8_t prevGamemode;
  Gp_Varint worldCount;
  Gp_String *dimensionNames;
  Gp_Nbt_Tag_Compound dimensionCodec;
  Gp_Nbt_Tag_Compound dimension;
  Gp_String dimensionName;
  uint64_t hashedSeed;
  Gp_Varint maxPlayers;
  Gp_Varint viewDistance;
  Gp_Varint simulationDistance;
  uint8_t reducedDebugInfo;
  uint8_t enableRespawnScreen;
  uint8_t isDebug, isFlat;
} Gp_Join_Game_Packet_Data;

Gp_Result gp_parse_join_game_packet_data(void *buffer, Gp_Packet **packet);

#endif // _GPBOTLIB_PACKETS_PLAY_H_