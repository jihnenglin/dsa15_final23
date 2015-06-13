CC = gcc
CPP = g++
CFLAGS = -O2 -std=c99 -Wall
CPPFLAGS = -O2 -std=c++11 -Wall
TARGET = main
TARGET2 = main_vector
OBJS = avl.o main.o
DEBUG = -DD_INPUT

RM = rm -rf

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CPP) $(CPPFLAGS) $(OBJS) -o $@

$(TARGET2): 
	$(CPP) $(CPPFLAGS) main_vector.cpp -o main_vector.exe
	
%.o: %.c
	$(CC) $(CFLAGS) $(DEBUG) $< -c

%.o: %.cpp
	$(CPP) $(CPPFLAGS) $(DEBUG) $< -c

cleanall:
	$(RM) $(TARGET)
	$(RM) $(TARGET2)
	$(RM) $(OBJS)

clean:
	$(RM) $(TARGET)
	$(RM) $(TARGET2)
