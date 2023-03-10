/* The program get_host.cpp */
/* The switch -lnsl must be added to the compilation line */
/*
 * Checking host entries
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


int main(void)
{
  printf("Enter host name to look up: ");
  char who[10] = {'\0'};;
  scanf("%9s", who);

  struct hostent *host = gethostbyname(who);
  if (host == NULL)
    exit(EXIT_FAILURE);

  printf("Here is what I found about %s:\n", who);
  printf("Official name:\t\t%s\n", host->h_name);

  printf("Aliases:\n");
  while (*host->h_aliases)
  {
    printf("\t- %s\n", *host->h_aliases);
    ++host->h_aliases;
  }

  printf("Address type:\t\t%i\n", host->h_addrtype);
  printf("Address length:\t\t%i\n", host->h_length);

  printf("Address list:\n");
  while (*host->h_addr_list)
  {
    struct in_addr in;
    memcpy(&in.s_addr, *host->h_addr_list, sizeof(in.s_addr));
    printf("\t- [%s] = %s\n", *host->h_addr_list, inet_ntoa(in));
    ++host->h_addr_list;
  }

  return 0;
}
