#include "../packets.h"

#include <stdlib.h>

Gp_Result _gp_parse_packet_play(void *buffer, Gp_Varint packetId, Gp_Packet **packet) {
  if (!buffer || !packet) return GP_INVALID_ARGS;

  switch (packetId) {
  case 38: return gp_parse_join_game_packet_data(buffer, packet);
  }

  return GP_INVALID_PACKET;
}

Gp_Result _gp_write_packet_play(void *buffer, Gp_Packet *packet) {
  if (!buffer || !packet) return GP_INVALID_ARGS;
  return GP_SUCCESS;
}

Gp_Result _gp_handle_packet_play(Gp_Bot *bot, Gp_Packet *packet) {
  if (!bot || !packet) return GP_INVALID_ARGS;

  switch (packet->packetID) { // Assume clientbound packets only
  case 38: {
    return _gp_push_event(&bot->eventQueue, (Gp_Event){
      .type = GP_EVENT_JOIN
    });
  } break;
  }

  return GP_SUCCESS;
}



Gp_Result gp_parse_join_game_packet_data(void *buffer, Gp_Packet **packet) {
  if (!buffer || !packet) return GP_INVALID_ARGS;

  *packet = gp_packet_create(38, sizeof(Gp_Join_Game_Packet_Data));
  if (!*packet) return GP_BUY_MORE_RAM;
  Gp_Join_Game_Packet_Data *joinGame = (Gp_Join_Game_Packet_Data*)(*packet)->data;
  memset(joinGame, 0, sizeof(*joinGame));

  Gp_Result result = GP_SUCCESS;
  if ((result = gp_read_uint32_from_buffer(buffer, &joinGame->entityID)) < GP_SUCCESS) return result;
  if ((result = gp_read_byte_from_buffer(buffer, &joinGame->isHardcore)) < GP_SUCCESS) return result;
  if ((result = gp_read_byte_from_buffer(buffer, &joinGame->gamemode)) < GP_SUCCESS) return result;
  if ((result = gp_read_byte_from_buffer(buffer, &joinGame->prevGamemode)) < GP_SUCCESS) return result;
  if ((result = gp_parse_varint(buffer, &joinGame->worldCount)) < GP_SUCCESS) return result;

  joinGame->dimensionNames = malloc(sizeof(*joinGame->dimensionNames) * joinGame->worldCount);
  if (!joinGame->dimensionNames) return GP_BUY_MORE_RAM;

  for (Gp_Varint i = 0; i < joinGame->worldCount; ++i)
    if ((result = gp_parse_string(buffer, &joinGame->dimensionNames[i])) < GP_SUCCESS) return result;

  if ((result = gp_parse_nbt_tag_compound(buffer, &joinGame->dimensionCodec)) < GP_SUCCESS) return result;
  if ((result = gp_parse_nbt_tag_compound(buffer, &joinGame->dimension)) < GP_SUCCESS) return result;
  if ((result = gp_parse_string(buffer, &joinGame->dimensionName)) < GP_SUCCESS) return result;

  if ((result = gp_read_uint64_from_buffer(buffer, &joinGame->hashedSeed)) < GP_SUCCESS) return result;

  if ((result = gp_parse_varint(buffer, &joinGame->maxPlayers)) < GP_SUCCESS) return result;
  if ((result = gp_parse_varint(buffer, &joinGame->viewDistance)) < GP_SUCCESS) return result;
  if ((result = gp_parse_varint(buffer, &joinGame->simulationDistance)) < GP_SUCCESS) return result;

  if ((result = gp_read_byte_from_buffer(buffer, &joinGame->reducedDebugInfo)) < GP_SUCCESS) return result;
  if ((result = gp_read_byte_from_buffer(buffer, &joinGame->enableRespawnScreen)) < GP_SUCCESS) return result;
  if ((result = gp_read_byte_from_buffer(buffer, &joinGame->isDebug)) < GP_SUCCESS) return result;
  if ((result = gp_read_byte_from_buffer(buffer, &joinGame->isFlat)) < GP_SUCCESS) return result;

  return GP_SUCCESS;
}