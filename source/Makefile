
CXX           = g++
DEFINES       = 
CXXFLAGS      = -O3 -pipe -ffast-math -funroll-loops -march=i686 -Wall
LINK          = g++
LFLAGS        = -O3 -s
LIBS          =



all: main

main: main.o
	$(LINK) $(LFLAGS) $(LIBS) *.o -o main

main.o: main.cpp turing-machine.hpp action.hpp random.hpp utils.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

clean:
	rm -rf *.o
	rm -rf main
