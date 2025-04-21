#include "gpbotlib/bot.h"

Gp_Result gp_bot_join(Gp_Bot *bot) {
  if (!bot ||
      !bot->send ||
      !bot->recv || bot->version >= COUNT_GP_VERSIONS)
    return GP_INVALID_ARGS;

  return GP_SUCCESS;
}

Gp_Result gp_bot_leave(Gp_Bot *bot) {
  if (!bot) return GP_INVALID_ARGS;

  return GP_SUCCESS;
}