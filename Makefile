CC = g++
PROJECT = main
# SRC = main.cpp
SRC = ./**.cpp
LIBS = `pkg-config --cflags --libs opencv4`
$(PROJECT) : $(SRC)
	$(CC) $(SRC) -o $(PROJECT) $(LIBS)