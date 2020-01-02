
CXXFLAGS = -std=c++11 -g -Wall -O2 -flto
LINK_SO = -lzmq

tests = pub_zmq sub_zmq

all: $(tests)

pub_zmq: pub_zmq.o
	$(CXX) $(CXXFLAGS) $(LINK_SO) -o $@ $^

sub_zmq: sub_zmq.o
	$(CXX) $(CXXFLAGS) $(LINK_SO) -o $@ $^

clean:
	rm -rf *.o $(tests)
