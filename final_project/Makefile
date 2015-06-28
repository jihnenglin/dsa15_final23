CC = gcc
CPP = g++
CFLAGS = -O2 -std=c99 -Wall
CPPFLAGS = -O2 -std=c++11 -Wall
STRUCT1 = main
STRUCT2 = main_vector
STRUCT3 = main_log
STRUCT4 = main_hash
STRUCT5 = main_bst
STRUCT6 = main_rb
TARGET = $(STRUCT2)
OBJ1 = avl.o main.o md5.o
OBJ2 = main_vector.o md5.o
OBJ3 = avl.o main_log.o md5.o
OBJ4 = main_hash.o md5.o
OBJ5 = bst.o main_bst.o md5.o
OBJ6 = rb.o main_rb.o md5.o
OBJS = $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6)
DEBUG = -DD_INPUT

RM = rm -rf

.PHONY: run1 run2 clean cleanall

all: $(STRUCT1) $(STRUCT2) $(STRUCT3) $(STRUCT4)

$(STRUCT1): $(OBJ1)
	$(CPP) $(CPPFLAGS) $(OBJ1) -o $@
$(STRUCT2): $(OBJ2)
	$(CPP) $(CPPFLAGS) $(OBJ2) -o $@ 
$(STRUCT3): $(OBJ3)
	$(CPP) $(CPPFLAGS) $(OBJ3) -o $@ 
$(STRUCT4): $(OBJ4)
	$(CPP) $(CPPFLAGS) $(OBJ4) -o $@ 
$(STRUCT5): $(OBJ5)
	$(CPP) $(CPPFLAGS) $(OBJ5) -o $@ 
$(STRUCT6): $(OBJ6)
	$(CPP) $(CPPFLAGS) $(OBJ6) -o $@ 
	
%.o: %.c
	$(CC) $(CFLAGS) $(DEBUG) $< -c

%.o: %.cpp
	$(CPP) $(CPPFLAGS) $(DEBUG) $< -c

run1:
	./$(STRUCT1)

run2:
	./$(STRUCT2)

run:
	./$(TARGET) <../test_data/test_in_small.txt >../test_data/out.txt
	diff ../test_data/out.txt ../test_data/test_out_small.txt

cleanall:
	$(RM) $(STRUCT1) $(STRUCT2) $(STRUCT3) $(STRUCT4) $(STRUCT5) $(STRUCT6)
	$(RM) $(OBJS)

clean:
	$(RM) $(TARGET)
