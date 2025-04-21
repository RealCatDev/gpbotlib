#include <stdio.h>
#include <stdint.h>

#include <gpbotlib/result.h>
#include <gpbotlib/bot.h>
#include <gpbotlib/varint.h>

int send(void *data, const char * buffer, int length) {
  printf("Send: ");
  for (int i = 0; i < length; ++i) {
    if (i) printf(", ");
    printf("%X", buffer[i]);
  }

  return length;
}

int recv(void *data, char * buffer, int length) {
  return 0;
}

int main(void) {
  Gp_Bot bot = {
    .version = GP_1_18_2,
    .send = send,
    .recv = recv
  };

  Gp_Result result = GP_SUCCESS;
  if ((result = gp_bot_join(&bot)) < GP_SUCCESS) {
    fprintf(stderr, "Failed to join: %s!\n", gp_result_as_cstr(result));
    return 1;
  }

  (void)gp_bot_leave(&bot);

  return 0;
}
