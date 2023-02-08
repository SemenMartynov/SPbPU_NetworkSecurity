/* The program server_game.cpp */

/* Server for the guess_word game (iterative schema) */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <syslog.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

int maxlives = 12;

#define MAXLEN 80 /* max size of string */
#define SERVER_GAME_TCP_PORT 1066

void guess_word(int, int);

int main(void)
{
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
  {
    perror("creating stream socket");
    exit(1);
  }

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(SERVER_GAME_TCP_PORT);

  if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
  {
    perror("binding socket");
    exit(2);
  }

  listen(sock, 5);
  printf("start to listen\n");

  while (1)
  {
    struct sockaddr_in client;
    int client_len = sizeof(client);
    int fd;
    if ((fd = accept(sock, (struct sockaddr *)&client, (socklen_t *)&client_len)) < 0)
    {
      perror("accepting connection");
      exit(3);
    }

    printf("guess_word start");
    if (fork() == 0)
    { /* In child process */
      guess_word(fd, fd);
    }

    close(fd);
  }
}

/*--------------------------- guess_word() function --------------------------*/

void guess_word(int in, int out)
{
  char word_[MAXLEN];
  word_[0] = 'g';
  word_[1] = 'r';
  word_[2] = 'e';
  word_[3] = 'e';
  word_[4] = 'n';
  word_[5] = '\0';

  char guess[MAXLEN];
  char outbuf[MAXLEN];

  char hostname[MAXLEN];
  gethostname(hostname, MAXLEN);
  sprintf(outbuf, "Playing on host: %s:\n\n", hostname);
  write(out, outbuf, strlen(outbuf));

  /* Pick a word */
  char *whole_word = word_;
  int word_length = 5;
  syslog(LOG_USER | LOG_INFO, "server_game chose word %s", whole_word);

  /* No letters are guessed initially */
  char part_word[MAXLEN];
  for (int i = 0; i < word_length; i++)
    part_word[i] = '-';
  part_word[word_length] = '\0';

  int lives = maxlives; /* Number of lives left */
  sprintf(outbuf, " %s   %d\n", part_word, lives);
  write(out, outbuf, strlen(outbuf));

  int game_state = 'I'; /* I => Incomplete, W => User Won, L => User Lost */
  while (game_state == 'I')
  /* Get guess letters from User */
  {
    while (read(in, guess, MAXLEN) < 0)
    {
      if (errno != EINTR)
        exit(4);
      printf("re-starting the read\n");
    } /* Re-start read() if interrupted by signal */

    int good_guess = 0;
    for (int i = 0; i < word_length; i++)
    {
      if (guess[0] == whole_word[i])
      {
        good_guess = 1;
        part_word[i] = whole_word[i];
      }
    }
    if (!good_guess)
      lives--;

    if (strcmp(whole_word, part_word) == 0)
    {
      game_state = 'W'; /* W => User Won  */
    }
    else if (lives == 0)
    {
      game_state = 'L';              /* L => User Lost */
      strcpy(part_word, whole_word); /* Show User the word */
    }
    sprintf(outbuf, " %s   %d\n", part_word, lives);
    write(out, outbuf, strlen(outbuf));
  }
  sprintf(outbuf, "You ween! Congrats!\n");
  write(out, outbuf, strlen(outbuf));
  close(in);
  exit(0);
}

/*----------------------- end of guess_word() function -------------------------*/
