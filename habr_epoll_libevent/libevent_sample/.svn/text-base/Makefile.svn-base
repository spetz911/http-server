PROJECT_NAME = libevent_sample

CPP = $(wildcard *.cpp)

INCLUDE = -I../common

LIBS = -levent -lpthread -lCommon

LIB_DIR = -L../libs

all: make_lib make_project

make_lib:
	make -C ../common

make_project:
	g++ -Wall -O3 -g0 $(CPP) $(INCLUDE) -o $(PROJECT_NAME) $(LIB_DIR) $(LIBS)
