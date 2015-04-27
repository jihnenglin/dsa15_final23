#comment
target = hw3_1 hw3_2
CC = g++
CPPFLAGS = -O2 -std=c++11 -Wall

all: $(target)

hw3_1: hw3_1.cpp line.cpp element.cpp
	$(CC) $(CPPFLAGS) line.h line.cpp element.h element.cpp hw3_1.cpp -o 3_4_1.out
	
hw3_2: hw3_2.cpp line2.cpp element2.cpp
	$(CC) $(CPPFLAGS) line.h line2.cpp element2.h element2.cpp hw3_2.cpp -o 3_4_2.out
	
run1:
	./3_4_1.out

run2:
	./3_4_2.out
	

clean:
	rm -rf 3_4_1.out 3_4_2.out