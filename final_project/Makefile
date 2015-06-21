CC = gcc
CPP = g++
CFLAGS = -O2 -std=c99 -Wall
CPPFLAGS = -O2 -std=c++11 -Wall
STRUCT1 = main
STRUCT2 = main_vector
STRUCT3 = main_log
TARGET = $(STRUCT1) $(STRUCT2) $(STRUCT3)
OBJ1 = avl.o main.o md5.o
OBJ2 = main_vector.o md5.o
OBJ3 = avl.o main_log.o md5.o
OBJS = $(OBJ1) $(OBJ2) $(OBJ3)
DEBUG = -DD_INPUT

RM = rm -rf

all: $(TARGET)

$(STRUCT1): $(OBJ1)
	$(CPP) $(CPPFLAGS) $(OBJ1) -o $@

$(STRUCT2): $(OBJ2)
	$(CPP) $(CPPFLAGS) $(OBJ2) -o $@ 

$(STRUCT3): $(OBJ3)
	$(CPP) $(CPPFLAGS) $(OBJ3) -o $@ 
	
%.o: %.c
	$(CC) $(CFLAGS) $(DEBUG) $< -c

%.o: %.cpp
	$(CPP) $(CPPFLAGS) $(DEBUG) $< -c

run1:
	./$(STRUCT1)

run2:
	./$(STRUCT2)

cleanall:
	$(RM) $(TARGET)
	$(RM) $(OBJS)

clean:
	$(RM) $(TARGET)
