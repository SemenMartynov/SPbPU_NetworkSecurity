/* The program sock_c_u_clt.cpp */
/*
 * Client - Unix domain, connection oriented
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
  static char buf[10] = {'\0'}; /* Buffer for messages	       */

  int orig_sock = -1; /* Original socket descriptor in client */
  if ((orig_sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
  { /* SOCKET */
    perror("generate error");
    exit(1);
  }

  static struct sockaddr_un  serv_adr;        /* Unix address of the server process */
  serv_adr.sun_family = AF_UNIX;   /* Set tag appropriately	*/
  strcpy(serv_adr.sun_path, NAME); /* Assign name			*/

  /* CONNECT */
  if (connect(orig_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr.sun_family) + strlen(serv_adr.sun_path)) < 0)
  {
    perror("connect error");
    exit(1);
  }

  for (int i = 1; i <= 10; i++)
  { /* Send messages */
    sprintf(buf, "c: %d\n", i);
    write(orig_sock, buf, sizeof(buf));
    read(orig_sock, buf, sizeof(buf));
    printf("c-> %s", buf);
  }

  close(orig_sock);
  exit(0);
}
