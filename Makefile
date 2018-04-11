CC = gcc
LD = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wno-unused -g
LDFLAGS = -lncurses
LIBS =
OBJS = game.o
EXECUTABLE = tictactoe

$(EXECUTABLE): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o $(EXECUTABLE)

game.o: game.c
	$(CC) $(CFLAGS) -c game.c

clean:
	rm -f $(EXECUTABLE) $(OBJS)

