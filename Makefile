CC = gcc
CFLAGS = -Wall -g
LIBS = -lSDL2 -lGL -lGLEW -lGLU   # Ajout de -lGLU ici

SRC = main.c
OBJ = $(SRC:.c=.o)
OUT = main

$(OUT): $(OBJ)
	$(CC) $(OBJ) -o $(OUT) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(OUT)
