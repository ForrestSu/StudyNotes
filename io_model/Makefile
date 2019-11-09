# date 2019-11-08 15:23:07 sunquan
.PHONY:clean  
CFLAG := -Wall -g -std=c++11 
COMPILE_PLATFORM := -m64
LIBRARIES += -lpthread 
BIN = app

all:$(BIN)

%.o: %.c
		g++ $(COMPILE_PLATFORM) -Wall -g -c $< -o $@

app: epoll_lt_et.c
		gcc -Wall -g $<  -o  $@

clean:
		rm $(BIN) *.o
