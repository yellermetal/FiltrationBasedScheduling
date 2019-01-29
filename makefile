# Makefile for the HBS program

CXX=g++
CC=g++
CXXFLAGS=-g -std=c++11 -Wall -pedantic
CCFLAGS = -g -Wall -pedantic
CXXLINK = $(CXX)
CCLINK = $(CC)

SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(addsuffix .o,$(basename $(SRCS)))

RM = rm -f
TARGET = HBS

# Creating the  executable
$(TARGET): $(OBJS)
	$(CXXLINK) -o $(TARGET) $(OBJS)
	

# Cleaning old files before new make
clean:
	$(RM) $(TARGET) $(OBJS)