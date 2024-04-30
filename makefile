all: integrate

integrate: integrate.o
	g++ -Wall -Wextra -pedantic integrate.o -o integrate
	
integrate.o: integrate.cpp
	g++ -Wall -Wextra -pedantic -c integrate.cpp

clean: 
	rm -f *.o integrate
