/* The header file local_c_i.h */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 6996
#define h_addr h_addr_list[0] /* for backward compatibility */
static char buf[BUFSIZ]; /*Buffer for messages*/
