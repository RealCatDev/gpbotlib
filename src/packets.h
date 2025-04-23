#ifndef   __GPBOTLIB_PACKETS_H_
#define   __GPBOTLIB_PACKETS_H_

#include "gpbotlib/result.h"
#include "gpbotlib/buffer.h"
#include "gpbotlib/bot.h"

#include "gpbotlib/packet.h"
#include "gpbotlib/packets/handshake.h"
// #include "gpbotlib/packets/status.h"
#include "gpbotlib/packets/login.h"
// #include "gpbotlib/packets/play.h"

Gp_Result _gp_parse_packet_handshake(void *buffer, Gp_Varint packetId, Gp_Packet **packet, Gp_Read_Byte_From_Buffer read);
Gp_Result _gp_write_packet_handshake(void *buffer, Gp_Packet *packet, Gp_Write_Byte_To_Buffer write);

Gp_Result _gp_parse_packet_status(void *buffer, Gp_Varint packetId, Gp_Packet **packet, Gp_Read_Byte_From_Buffer read);
Gp_Result _gp_write_packet_status(void *buffer, Gp_Packet *packet, Gp_Write_Byte_To_Buffer write);

Gp_Result _gp_parse_packet_login(void *buffer, Gp_Varint packetId, Gp_Packet **packet, Gp_Read_Byte_From_Buffer read);
Gp_Result _gp_write_packet_login(void *buffer, Gp_Packet *packet, Gp_Write_Byte_To_Buffer write);
Gp_Result _gp_handle_packet_login(Gp_Bot *bot, Gp_Packet *packet);

Gp_Result _gp_parse_packet_play(void *buffer, Gp_Varint packetId, Gp_Packet **packet, Gp_Read_Byte_From_Buffer read);
Gp_Result _gp_write_packet_play(void *buffer, Gp_Packet *packet, Gp_Write_Byte_To_Buffer write);
Gp_Result _gp_handle_packet_play(Gp_Bot *bot, Gp_Packet *packet);

#endif // __GPBOTLIB_PACKETS_H_