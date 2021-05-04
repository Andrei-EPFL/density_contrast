CXX=g++
OPTIM=-O1 -march=native
CXXFLAGS=-std=c++14 -Wall -Wextra -pedantic
LIBS = -DOMP -fopenmp
SRCS = main.cpp load_conf.cpp functions.cpp
EXEC=main

all:
	$(CXX) $(CXXFLAGS) $(OPTIM) -o $(EXEC) $(SRCS) $(LIBS)
clean:
	rm -fv $(EXEC)
