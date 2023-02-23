TARGET_EXEC := socketpair

CC := gcc
CFLAGS := -Wall -Wextra -pedantic-errors -std=c99 -O2
LDFLAGS :=

SRCS != echo *.c
OBJS := $(SRCS:.c=.o)

$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean
clean:
	-rm -f $(OBJS) $(TARGET_EXEC)
