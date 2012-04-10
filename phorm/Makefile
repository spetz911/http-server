# The project full build example:
#
#  $ CXX=g++44 BOOST_LIB=/opt/boost_1_46_1/lib/ BOOST_INC=/opt/boost_1_46_1/include make
#
# Use TDMN_WITH_VMSPLICE=1 option to build with vmsplice.
# Use PRODUCTION=1 to build in production mode.
#
#  2011. Written by NatSys Lab. (info@natsys-lab.com).
#


CFLAGS		+= -std=gnu++0x -ggdb3 -Wall
LDFLAGS		= -lpthread
INC		= 

HEADERS		= epoll.hpp sock.h http_parser.h thread_pool.h tqueue.h thread2.cc
OBJECTS		= epoll.o sock.o http_parser.o main.o
TARGETS		= http_serv


all : http_serv
#	gcc -std=gnu++0x dpi_ctl.cc -lboost_program_options


run :
	./http_serv 0.0.0.0 80 1 .


http_serv : $(OBJECTS)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)


%.o : %.cc $(HEADERS)
	$(CXX) $(CFLAGS) $(DEBUG) $(INC) -c $< -o $@


clean :
	rm -f *.o* *~ $(TARGETS)


