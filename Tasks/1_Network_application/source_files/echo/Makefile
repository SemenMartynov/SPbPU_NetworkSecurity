PROGRAMS = echo_server echo_client

CC := gcc
CFLAGS := -Wall -Wextra -pedantic-errors -std=c99 -O2
LDFLAGS :=

SRCS != echo *.c
OBJS := $(SRCS:.c=.o)

all: $(PROGRAMS)

$(PROGRAMS): $(PROGRAM=$(.TARGET))

$(PROGRAM): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	-rm -f $(OBJS) $(PROGRAMS)
