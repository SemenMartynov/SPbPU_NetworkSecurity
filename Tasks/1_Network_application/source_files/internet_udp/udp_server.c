/* The program sock_c_u_srv.cpp */
/*
 * Server - UNIX domain, connection oriented
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#define NAME "my_sock"

int main(void)
{
  static char buf[10] = {'\0'}; /* Buffer for messages */

  int orig_sock; /* Original socket descriptor */
  if ((orig_sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
  { /* SOCKET */
    perror("Generate error");
    exit(1);
  }

  static struct sockaddr_un serv_adr;
  serv_adr.sun_family = AF_UNIX;   /* Set tag appropriately	*/
  strcpy(serv_adr.sun_path, NAME); /* Assign name(108 chars max)	*/
  unlink(NAME);                    /* Remove old copy,if present	*/

  /* BIND */
  if (bind(orig_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr.sun_family) + strlen(serv_adr.sun_path)) < 0)
  {
    perror("bind error");
    exit(2);
  };

  /* LISTEN */
  listen(orig_sock, 3);

  /* ACCEPT */
  static struct sockaddr_un clnt_adr; /* UNIX addresses of client-server */
  int clnt_len = sizeof(clnt_adr); /* Lenght of client address */
  int new_sock = -1;  /* New socket descriptor from connect */
  if ((new_sock = accept(orig_sock, (struct sockaddr *)&clnt_adr, (socklen_t *)&clnt_len)) < 0)
  {
    perror("accept error");
    exit(3);
  }

  /*printf("new sock = %d\n",new_sock);*/
  for (int i = 1; i <= 10; i++)
  { /* PROCESS */
    sleep(1);
    read(new_sock, buf, sizeof(buf));
    printf("s-> %s", buf);
    write(new_sock, buf, sizeof(buf));
  }

  close(new_sock);
  exit(0);
}
