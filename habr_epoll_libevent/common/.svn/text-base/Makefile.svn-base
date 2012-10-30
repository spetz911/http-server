CPP = $(wildcard *.cpp)

DIR = ../libs

LIB_NAME = Common

%.o: %.cpp
	g++ -O3 -g0 -Wall -c $^ -o $@

OBJ = $(CPP:.cpp=.o)

all: $(OBJ)
	ar cr $(DIR)/lib$(LIB_NAME).a $(OBJ)
