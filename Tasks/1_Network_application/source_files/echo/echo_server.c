/*
** echo_server.cpp -- the echo server for echo_cient.cpp; demonstrates UNIX sockets
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

  struct sockaddr_un local;
  local.sun_family = AF_UNIX;
  //	remote.sun_family = AF_UNIX;       //
  strcpy(local.sun_path, SOCK_PATH);
  unlink(local.sun_path);
  int len = strlen(local.sun_path) + sizeof(local.sun_family);
  if (bind(mySocket, (struct sockaddr *)&local, len) == -1)
  {
    perror("bind");
    exit(1);
  }

  if (listen(mySocket, 5) == -1)
  {
    perror("listen");
    exit(1);
  }

  char str[100] = {'\0'};
  for (;;)
  {
    printf("Waiting for a connection...\n");
    struct sockaddr_un remote;
    int rlen = sizeof(remote);
    int remSocket = -1;
    if ((remSocket = accept(mySocket, (struct sockaddr *)&remote, (socklen_t *)&rlen)) == -1)
    {
      perror("accept");
      exit(1);
    }

    printf("Connected.\n");
    int done = 0;
    do
    {
      int textLength = recv(remSocket, str, 100, 0);
      if (textLength <= 0)
      {
        if (textLength < 0)
          perror("recv");
        done = 1;
      }

      if (!done)
        if (send(remSocket, str, textLength, 0) < 0)
        {
          perror("send");
          done = 1;
        }
    } while (!done);

    close(remSocket);
  }

  return 0;
}
