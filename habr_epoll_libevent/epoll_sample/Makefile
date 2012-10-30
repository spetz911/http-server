PROJECT_NAME = epoll_sample

CPP = $(wildcard *.cpp)

INCLUDE = -I../common

LIBS = -lpthread -lCommon

LIB_DIR = -L../libs

all: make_libs make_project

make_libs:
	make -C ../common

make_project:
	g++ -Wall -O3 -g0 $(CPP) $(INCLUDE) -o $(PROJECT_NAME) $(LIB_DIR) $(LIBS)
