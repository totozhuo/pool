OBJS = udp_server_json.o cJSON.o function.o threadpool.o
OBJ = udp_connect_json.o cJSON.o
FLAGS = -g -c
CC = gcc
GG = g++

all:server client

client:$(OBJ)
	$(GG) $^ -o $@
server:$(OBJS)
	$(GG) $^ -lmysqlclient -lpthread -o $@

%.o:%.c
	$(CC) $(FLAGS) $< -lmysqlclient -lpthread -o $@
%.o:%.cpp
	$(GG) $(FLAGS) $< -lmysqlclient -lpthread -o $@


.PHONY:
clean:
	rm -rf $(OBJS) $(OBJ) server client
