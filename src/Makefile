CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.cpp)
OBJS=$(SRCS: .cpp=.o)

mxtxtprot:
	$(CC) -o ../load/mxtxtprot $(OBJS) $(LDFLAGS)

clean:
		rm -f ../load/mxtxtprot *.o *~ tmp*
