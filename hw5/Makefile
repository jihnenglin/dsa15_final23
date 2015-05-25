target = hw5
CPPFLAGS = -O2 -std=c++11 -Wall
name = system

all: $(target)

%: %.cpp
	g++ $(CPPFLAGS) $< -o $(name)
	
system: hw5.cpp binomial_heap.h
	g++ $(CPPFLAGS) hw5.cpp -o $(name)

clean:
	rm -rf $(target)

run:
	./system
