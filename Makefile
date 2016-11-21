CC = g++
SOURCES = Parser.cpp Donnees.cpp SSCFLP.cpp
CFLAGS = -Wall -g -lm -lglpk
.PHONY = clean

all: main

main: $(SOURCES:.cpp=.o)

clean: 
	rm ./*.o
