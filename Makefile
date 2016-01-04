
all: relay

CC=gcc
CXX=g++
COBJS=hid.o
CPPOBJS=relay.o
OBJS=$(COBJS) $(CPPOBJS)
CFLAGS+=-I../hidapi -Wall -g -c 
LIBS=-framework IOKit -framework CoreFoundation


relay: $(OBJS)
	g++ -Wall -g $^ $(LIBS) -o relay

$(COBJS): %.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(CPPOBJS): %.o: %.cpp
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm -f *.o relay $(CPPOBJS)

.PHONY: clean
