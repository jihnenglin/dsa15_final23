target = tree forest
CFLAGS = -O2 -Wall
CPPFLAGS = -O2 -std=c++11 -Wall
name = tree

all: $(target)

%: %.c
	gcc $(CFLAGS) $< -o $(name)

%: %.cpp
	g++ $(CPPFLAGS) $< -o $(name)
	
tree: tree.cpp
	g++ $(CPPFLAGS) tree.cpp -o $@
	
forest: forest.cpp
	g++ $(CPPFLAGS) forest.cpp -o $@

clean:
	rm -rf $(target)
