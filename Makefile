.PHONY : clean

CC=g++
CPPFLAGS=-g -Wall -std=c++11

BINDIR=bin
SAMPLE_SRCS=sample.cpp
SAMPLE_OBJS=$(SAMPLE_SRCS:.cpp=.o)
SAMPLE=$(BINDIR)/sample


all: $(SAMPLE)

clean:
	rm -rf *.o
	rm -rf $(BINDIR)

$(SAMPLE) : $(SAMPLE_OBJS)
	mkdir -p $(BINDIR)
	$(CC) $(CPPFLAGS) $(SAMPLE_OBJS) -o $@
