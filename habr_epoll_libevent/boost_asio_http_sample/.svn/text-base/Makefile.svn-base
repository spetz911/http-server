PROJECT_NAME = boost_asio_http_sample

CPP = $(wildcard *.cpp)

INCLUDE = -I

LIBS = -lboost_system-mt -lboost_thread-mt

LIB_DIR = -L

all: make_project

make_project:
	g++ -Wall -O3 -g0 $(CPP) $(INCLUDE) -o $(PROJECT_NAME) $(LIB_DIR) $(LIBS)
