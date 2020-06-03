.PHONY:clean  
CFLAG := -Wall -g -std=c++11
LIBRARIES += -lhttp_parser -luv
BIN := asyn_http

all:$(BIN)


%.o: %.cpp
		g++ $(CFLAG) -c $< -o $@

asyn_http: http_request_call.o main.o
		g++  $(CFLAG) $(LIBRARIES)  $^ -o $@
clean:
		rm $(BIN) *.o 
