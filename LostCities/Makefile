
CC = gcc


CFLAGS = -Wall -Werror -Wextra -I/usr/include/SDL2 -D_REENTRANT


LDFLAGS = -lSDL2 -lSDL2_image -lm

SRC = Graphics.c animation.c mcts.c


TARGET = Lost.out


all: $(TARGET)


$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)


clean:
	rm -f $(TARGET) *.o


.PHONY: all clean

