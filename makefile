#This is a Makefile for the Date example
CCC = g++
CXXFLAGS = -Wall -g
CXXLINK = $(CCC)
OBJS = Cache.o Sets.o cacheSim.o
RM = rm -f

#Default target s(usually "all")
all: cacheSim
#Creating the executables
cacheSim: $(OBJS)
	$(CXXLINK) -o cacheSim $(OBJS)
#Creating object files using default rules
Sets.o: sets.cpp sets.h
Cache.o: sets.cpp sets.h Cache.h Cache.cpp
cacheSim.o:sets.cpp sets.h Cache.h Cache.cpp cacheSim.cpp
#Cleaning old files before new make
clean:
	$(RM) cacheSim *.o *.bak *~ "#"* core