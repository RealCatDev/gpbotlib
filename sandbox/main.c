#include <stdio.h>
#include <stdint.h>

#include <gpbotlib/result.h>
#include <gpbotlib/bot.h>
#include <gpbotlib/varint.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#elif defined(__linux__)
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#else
#error "Unsupported"
#endif // _WIN32

#if defined(_WIN32)
int _send(void *data, const char *buffer, int length) {
  SOCKET sock = *(SOCKET*)data;

  int sent = send(sock, buffer, length, 0);
  if (sent == SOCKET_ERROR) {
    fprintf(stderr, "Send failed: %d\n", WSAGetLastError());
    return -1;
  }

  printf("Sent %d bytes!\n", sent);

  return sent;
}

int _recv(void *data, char *buffer, int length) {
  SOCKET sock = *(SOCKET*)data;

  int received = recv(sock, buffer, length, 0);
  if (received == SOCKET_ERROR) {
    fprintf(stderr, "Recv failed: %d\n", WSAGetLastError());
    return -1;
  }

  printf("Recieved %d bytes!\n", received);

  return received;
}
#elif defined(__linux__)
int _send(void *data, const char *buffer, int length) {
  int sock = *(int *)data;

  int sent = send(sock, buffer, length, 0);
  if (sent == -1) {
    perror("Send failed");
    return -1;
  }

  printf("Sent %d bytes!\n", sent);

  return sent;
}

int _recv(void *data, char *buffer, int length) {
  int sock = *(int *)data;

  int received = recv(sock, buffer, length, 0);
  if(received == -1) {
    perror("Recv failed");
  }

  printf("Recieved %d bytes!\n", received);
  printf("\n");
  return received;
}
#endif

int main(void) {
  #if defined(_WIN32)
  WSADATA wsaData; // Thanks ChatGPT for adding WinSock2 here, I didn't care enough to do it myself
  if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
    fprintf(stderr, "WSAStartup failed.\n");
    return 1;
  }

  SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock == INVALID_SOCKET) {
    fprintf(stderr, "Socket creation failed: %d\n", WSAGetLastError());
    WSACleanup();
    return 1;
  }

  struct sockaddr_in serverAddr = {
    .sin_family = AF_INET,
    .sin_port = htons(25565),
    .sin_addr.s_addr = inet_addr("127.0.0.1")
  };

  if (connect(sock, (SOCKADDR *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
    fprintf(stderr, "Connect failed: %d\n", WSAGetLastError());
    closesocket(sock);
    WSACleanup();
    return 1;
  }
  #elif defined(__linux__)
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in serverAddr = {
    .sin_family = AF_INET,
    .sin_port = htons(25565),
    .sin_addr.s_addr = inet_addr("127.0.0.1")
  };
  connect(sock, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr_in));
  #endif

  Gp_Bot bot = {
    .version = GP_1_18_2,
    .username = GP_STRING("gpbotlibtest"),
    .data = &sock,
    .send = _send,
    .recv = _recv
  };

  Gp_Result result = GP_SUCCESS;
  if ((result = gp_bot_join(&bot)) < GP_SUCCESS) {
    fprintf(stderr, "Failed to join: %s!\n", gp_result_as_cstr(result));
    return 1;
  }

  while (!gp_bot_is_offline(&bot)) {
    if ((result = gp_bot_update(&bot)) < GP_SUCCESS) {
      fprintf(stderr, "Failed to update bot: %s!\n", gp_result_as_cstr(result));
      return 1;
    }

    Gp_Event event = {0};
    while ((result = gp_poll_event(&bot.eventQueue, &event)) > GP_SUCCESS) {
      switch (event.type) {
      case GP_EVENT_JOIN: {

      } break;
      case GP_EVENT_DISCONNECT: {

      } break;
      }
    }

    if (result < GP_SUCCESS) {
      fprintf(stderr, "Failed to poll event: %s!\n", gp_result_as_cstr(result));
      return 1;
    }
  }

  (void)gp_bot_leave(&bot);

  #if defined(_WIN32)
  closesocket(sock);
  WSACleanup();
  #elif defined(_WIN32)
  close(sock);
  #endif // _WIN32

  return 0;
}
