#ifndef   _GPBOTLIB_BOT_H_
#define   _GPBOTLIB_BOT_H_

#include <gpbotlib/result.h>
#include <gpbotlib/event.h>
#include <gpbotlib/string.h>
#include <gpbotlib/uuid.h>
#include <stddef.h>

typedef enum Gp_Version {
  GP_1_18_2 = 758,
} Gp_Version;

typedef int (*Gp_Send_Func)(void *, const char *, int);
typedef int (*Gp_Recv_Func)(void *, char *, int);

typedef enum Gp_Bot_State {
  GP_BOT_HANDSHAKE = 0,
  GP_BOT_STATUS,
  GP_BOT_LOGIN,
  GP_BOT_PLAY,

  COUNT_GP_BOT_STATES
} Gp_Bot_State;

typedef struct Gp_Bot {
  Gp_Version version;
  Gp_String username;

  void *data;
  Gp_Send_Func send;
  Gp_Recv_Func recv;

  bool online;
  Gp_Uuid uuid;
  Gp_Bot_State state;
  Gp_Event_Queue eventQueue;
  
  size_t maximumWithoutCompresssion;
} Gp_Bot;

Gp_Result gp_bot_join(Gp_Bot *bot);
Gp_Result gp_bot_leave(Gp_Bot *bot);
Gp_Result gp_bot_update(Gp_Bot *bot);
Gp_Result gp_bot_is_offline(Gp_Bot *bot);

#endif // _GPBOTLIB_BOT_H_
