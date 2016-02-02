
all: relay

OS=$(shell uname -s)

ifeq ("$(OS)","Darwin")
	LIBS=-framework IOKit -framework CoreFoundation
	COBJS=hid-darwin.o
endif


CC=gcc
CXX=g++
CPPOBJS=relay.o
OBJS=$(COBJS) $(CPPOBJS)
CFLAGS+=-I../hidapi -Wall -g -c 


relay: $(OBJS)
	g++ -Wall -g $^ $(LIBS) -o relay

$(COBJS): %.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(CPPOBJS): %.o: %.cpp
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm -f *.o relay $(CPPOBJS)

.PHONY: clean
