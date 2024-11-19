
CC = gcc


CFLAGS = -Wall -Werror -Wextra -I/usr/include/SDL2 -D_REENTRANT


LDFLAGS = -lSDL2 -lSDL2_image -lm -lSDL2_ttf -I/usr/include/SDL2


SRC = lost.c


TARGET = Lost.out


all: $(TARGET)


$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)


clean:
	rm -f $(TARGET) *.o


.PHONY: all clean

