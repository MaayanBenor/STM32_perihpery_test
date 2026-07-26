##
# @file Makefile
# @brief Build rules for the PC UDP server.
##

CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = server

SRC = main.c udp.c eth_protocol.c records.c timer.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) -lsqlite3

%.o: %.c eth_protocol.h udp.h records.h timer.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)