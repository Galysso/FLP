CC = g++
<<<<<<< HEAD
SOURCES = Parser.cpp Donnees.cpp SSCFLP.cpp
CFLAGS = -Wall -g -lm -lglpk
=======
SOURCES =
CFLAGS = -Wall -g -lglpk -lm
>>>>>>> d341f1162891a2de341539a586cacf7afb1babcf
.PHONY = clean

all: main

main: $(SOURCES:.cpp=.o)

clean: 
	rm ./*.o
