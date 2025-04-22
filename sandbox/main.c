#include <stdio.h>
#include <stdint.h>

#include <gpbotlib/result.h>
#include <gpbotlib/bot.h>
#include <gpbotlib/varint.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#else
#error "Unsupported"
#endif // _WIN32

int winsock_send(void *data, const char *buffer, int length) {
  SOCKET sock = *(SOCKET*)data;

  int sent = send(sock, buffer, length, 0);
  if (sent == SOCKET_ERROR) {
    fprintf(stderr, "Send failed: %d\n", WSAGetLastError());
    return -1;
  }

  printf("Sent %d bytes!\n", sent);

  return sent;
}

int winsock_recv(void *data, char *buffer, int length) {
  SOCKET sock = *(SOCKET*)data;

  int received = recv(sock, buffer, length, 0);
  if (received == SOCKET_ERROR) {
    fprintf(stderr, "Recv failed: %d\n", WSAGetLastError());
    return -1;
  }

  printf("Recieved %d bytes!\n", received);

  return received;
}

int main(void) {
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

  Gp_Bot bot = {
    .version = GP_1_18_2,
    .data = &sock,
    .send = winsock_send,
    .recv = winsock_recv
  };

  Gp_Result result = GP_SUCCESS;
  if ((result = gp_bot_join(&bot)) < GP_SUCCESS) {
    fprintf(stderr, "Failed to join: %s!\n", gp_result_as_cstr(result));
    return 1;
  }

  (void)gp_bot_leave(&bot);

  closesocket(sock);
  WSACleanup();

  return 0;
}