OBJS = server_function.o other_function.o server.o threadpool.o
OBJ =  client_function.o other_function.o client.o server_function.o
FLAGS = -g -c
CC = gcc
GG = g++

all:server client

client:$(OBJ)
	$(GG) $^ -lmysqlclient -lpthread -o $@

server:$(OBJS)
	$(GG) $^ -lmysqlclient -lpthread -o $@

%.o:%.c
	$(CC) $(FLAGS) $< -lmysqlclient -lpthread -o $@

%.o:%.cpp
	$(GG) $(FLAGS) $< -lmysqlclient -lpthread -o $@

.PHONY:
clean:
		rm -rf $(OBJS) server client *.o
