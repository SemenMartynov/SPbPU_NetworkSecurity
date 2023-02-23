/*
** echo_client.cpp -- the echo client for echo_server.cpp; demonstrates UNIX sockets
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_PATH "echo_socket"

int main(void)
{
  
  int mySocket = -1;
  if ((mySocket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
  {
    perror("socket");
    exit(1);
  }

  printf("Trying to connect...\n");
  struct sockaddr_un remote;
  remote.sun_family = AF_UNIX;
  strcpy(remote.sun_path, SOCK_PATH);
  int len = strlen(remote.sun_path) + sizeof(remote.sun_family);
  if (connect(mySocket, (struct sockaddr *)&remote, len) == -1)
  {
    perror("connect");
    exit(1);
  }

  printf("Connected.\n");
  char str[100] = {'\0'};
  while (printf("> "), fgets(str, 100, stdin), !feof(stdin))
  {
    if (send(mySocket, str, strlen(str), 0) == -1)
    {
      perror("send");
      exit(1);
    }

    int textLength;
    if ((textLength = recv(mySocket, str, 100, 0)) > 0)
    {
      str[textLength] = '\0';
      printf("echo> %s", str);
    }
    else
    {
      if (textLength < 0)
        perror("recv");
      else
        printf("Server closed connection\n");
      exit(1);
    }
  }

  close(mySocket);

  return 0;
}
