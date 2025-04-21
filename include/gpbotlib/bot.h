#ifndef   _GPBOTLIB_BOT_H_
#define   _GPBOTLIB_BOT_H_

#include "result.h"

typedef enum Gp_Version {
  GP_1_18_2 = 758,
} Gp_Version;

typedef int (*Gp_Send_Func)(void *, const char *, int);
typedef int (*Gp_Recv_Func)(void *, char *, int);

typedef struct Gp_Bot {
  Gp_Version version;

  void *data;
  Gp_Send_Func send;
  Gp_Recv_Func recv;
} Gp_Bot;

Gp_Result gp_bot_join(Gp_Bot *bot);
Gp_Result gp_bot_leave(Gp_Bot *bot);

#endif // _GPBOTLIB_BOT_H_