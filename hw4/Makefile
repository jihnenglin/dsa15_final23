target = tree
CFLAGS = -O2 -Wall
CPPFLAGS = -O2 -std=c++11 -Wall
name = tree

all: $(target)

%: %.c
	gcc $(CFLAGS) $< -o $(name)

%: %.cpp
	g++ $(CPPFLAGS) $< -o $(name)
	
hw2: hw2.cpp data.h
	g++ $(CPPFLAGS) hw2.cpp -o $(name)

clean:
	rm -rf $(target)

run:
	./demo
