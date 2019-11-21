# date 2019-11-08 15:23:07 sunquan
.PHONY:clean  
COMPILE_PLATFORM := -m64
LIBRARIES += -lpthread 
BIN = app aio_test

all:$(BIN)

%.o: %.c
		g++ $(COMPILE_PLATFORM) -Wall -g -c $< -o $@

app: epoll_lt_et.c
		gcc -Wall -g $<  -o  $@

aio_test: aio_sample.cpp
		g++ -Wall -g -std=c++11 -lrt -laio  $< -o $@

clean:
		rm $(BIN) *.o
