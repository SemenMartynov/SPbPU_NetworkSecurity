/* The program sock_c_i_clt.cpp */
/*
 * Client - Internet domain, connection-oriented
 */
#include "local_c_i.h"
int main(int argc, char *argv[])
{
  if (argc != 2)
  { /* Expect name of host on cmd line */
    fprintf(stderr, "usage: %s server\n", argv[0]);
    exit(1);
  }

  struct hostent *host;          /* The host(server)	  */
  host = gethostbyname(argv[1]); /* Get the host info */
  if (host == (struct hostent *)NULL)
  {
    perror("gethostbyname");
    exit(2);
  }

  struct sockaddr_in serv_adr;                              /* Internet address of server  */
  memset(&serv_adr, 0, sizeof(serv_adr));                   /* Clear the structure  */
  serv_adr.sin_family = AF_INET;                            /* Set the address type */
  memcpy(&serv_adr.sin_addr, host->h_addr, host->h_length); /*Adr   */
  serv_adr.sin_port = htons(PORT);                          /* Use our fake port    */

  int orig_sock = -1; /* Original socket descriptor in client */
  if ((orig_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  { /* SOCKET */
    perror("generate error");
    exit(3);
  }

  /* CONNECT */
  if (connect(orig_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) < 0)
  {
    perror("connect error");
    exit(4);
  };

  do
  {
    int len;                        /* Length of server address		   */
    write(fileno(stdout), "> ", 3); /* Prompt user */
    if ((len = read(fileno(stdin), buf, BUFSIZ)) > 0)
    { /* Get input */
      write(orig_sock, buf, len);
      if ((len = read(orig_sock, buf, len)) > 0) /* if returned */
        write(fileno(stdout), buf, len);         /* display it */
    }
  } while (buf[0] != '.');

  close(orig_sock);
  exit(0);
};
