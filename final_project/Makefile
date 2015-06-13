CC = gcc
CPP = g++
CFLAGS = -O2 -std=c99 -Wall
CPPFLAGS = -O2 -std=c++11 -Wall
TARGET = main
OBJS = avl.o main.o
DEBUG = -DD_INPUT

RM = rm -rf

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CPP) $(CPPFLAGS) $(OBJS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(DEBUG) $< -c

%.o: %.cpp
	$(CPP) $(CPPFLAGS) $(DEBUG) $< -c

cleanall:
	$(RM) $(TARGET)
	$(RM) $(OBJS)

clean:
	$(RM) $(TARGET)
