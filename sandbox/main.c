#include <stdio.h>

#include <gpbotlib/result.h>
#include <gpbotlib/bot.h>

int main() {
  Gp_Bot bot = {
    .version = GP_1_18_2,
  };

  Gp_Result result = GP_SUCCESS;
  if ((result = gp_bot_join(&bot)) < GP_SUCCESS) {
    fprintf(stderr, "Failed to join!\n");
    return 1;
  }

  (void)gp_bot_leave(&bot);

  return 0;
}
