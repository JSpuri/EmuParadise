all: src/main.cpp
	g++ src/main.cpp -o nesemu

clean: 
	$(RM) myprog

test:
	g++ src/main.cpp src/structures.cpp -o nesemu
	./nesemu UNRELATED/game.nes