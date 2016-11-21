CC = g++
SOURCES =
CFLAGS = -Wall -g -lglpk -lm
.PHONY = clean

all: main

main: $(SOURCES:.cpp=.o)

clean: 
	rm ./*.o
