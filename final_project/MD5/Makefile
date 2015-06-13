CC = gcc
CPP = g++
CFLAGS = -O2 -std=c99 -Wall
CPPFLAGS = -O2 -std=c++11 -Wall
TARGET = main
RM = rm -rf

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(DEBUG) $< -c

%.o: %.cpp
	$(CPP) $(CPPFLAGS) $(DEBUG) $< -c

$(TARGET): md5.h
	$(CPP) $(CPPFLAGS) main.cpp md5.cpp -o $@

run:
	./main

clean:
	$(RM) $(TARGET)
