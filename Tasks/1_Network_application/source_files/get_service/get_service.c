/* The program get_serv.cpp */
/*
 * Checking service  --port entries for a host
 */
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>

int main(void)
{

  printf("Enter service to look up: "); // exonet
  char service[10] = {'\0'};
  scanf("%9s", service);
  printf("Enter protocol to look up: "); // tcp
  char protocol[10] = {'\0'};
  scanf("%9s", protocol);

  struct servent *serv = getservbyname(service, protocol);
  if (serv == NULL)
  {
    printf("Service %s for protocol %s not found\n", service, protocol);
    exit(EXIT_FAILURE);
  }

  printf("Here is what I found:\n");
  printf("Official name:\t\t%s\n", serv->s_name);
  printf("Aliases:");
  while (*serv->s_aliases)
  {
    printf("\t- %s\n", *serv->s_aliases);
    ++serv->s_aliases;
  }
  printf("Port number:\t\t%i\n", htons(serv->s_port));
  printf("Protocol Family:\t\t%s\n\n", serv->s_proto);

}
