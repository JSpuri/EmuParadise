all: src/main.cpp
	g++ src/main.cpp -o nesemu

clean: 
	$(RM) myprog
