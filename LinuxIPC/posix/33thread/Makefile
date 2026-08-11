.PHONY: clean all 
CC=gcc 
CFLAGS= -g 
BIN= thread clientThread

all:$(BIN) 
%.o:%.c 
	$(CC) $(CFLAGS) -c $< -o $@ 
thread: thread.o
	$(CC) $(CFLAGS) $^ -o $@ -lpthread
clientThread: clientThread.o
	$(CC) $(CFLAGS) $^ -o $@ -lpthread
clean:
	rm -f *.o *~ $(BIN)


