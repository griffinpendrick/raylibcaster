CC = gcc 
CFLAGS = -w -Wall -std=c99 -I$(INCLUDE)

LIBS = -LRaylib/lib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL Raylib/lib/libraylib.a
INCLUDE = Raylib/include

OUTPUT = game

SRC = $(wildcard *.c)

all: $(OUTPUT)

$(OUTPUT): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LIBS) -o $(OUTPUT)

clean:
	rm -f $(OUTPUT)
