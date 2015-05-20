target = hw5
CPPFLAGS = -O2 -std=c++11 -Wall
name = hw5

all: $(target)

%: %.cpp
	g++ $(CPPFLAGS) $< -o $(name)
	
hw2: hw5.cpp binomial_tree.h
	g++ $(CPPFLAGS) hw5.cpp -o $(name)

clean:
	rm -rf $(target)

run:
	./hw5
