# date 2018-03-01 sunquan
.PHONY:clean  
CFLAG := -Wall -g -std=c++11 
COMPILE_PLATFORM := -m64
INCLUDES := -I./inc
LIBRARIES := -L../../../bin/linux  
LIBRARIES += -lpthread -lzmq -luv 
BIN =  uv_sock

all:$(BIN)

%.o: %.c
		g++ $(COMPILE_PLATFORM) -Wall -g -c $< -o $@
%.o: %.cpp
		g++ $(COMPILE_PLATFORM) $(CFLAG) $(INCLUDES) -c $< -o $@

uv_sock: uv_zsock.o  uv_zsock_test.c
		g++ $(COMPILE_PLATFORM) $(CFLAG) $(LIBRARIES)  $^ -o $@
clean:
		rm $(BIN) *.o 