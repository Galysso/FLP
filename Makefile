CC = g++
SOURCES = Parser.cpp Donnees.cpp SSCFLP.cpp
CFLAGS = -Wall -g
LDFLAGS = -lm -lglpk
.PHONY = clean

all: main

main: $(SOURCES:.cpp=.o) $(LDFLAGS)

clean: 
	rm ./*.o
