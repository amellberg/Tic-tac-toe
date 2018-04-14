CC = gcc
LD = gcc
CFLAGS = -std=c99 -Wall -Wextra -g -Wno-unused
LDFLAGS = 
LIBS = -lncurses
OBJS = game.o draw.o board.o
EXECUTABLE = tictactoe

$(EXECUTABLE): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o $(EXECUTABLE)

game.o: game.c draw.h board.h
	$(CC) $(CFLAGS) -c game.c

draw.o: draw.h draw.c board.h
	$(CC) $(CFLAGS) -c draw.c

board.o: board.h board.c
	$(CC) $(CFLAGS) -c board.c

clean:
	rm -f $(EXECUTABLE) $(OBJS)

