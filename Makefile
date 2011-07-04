CXX		= g++
CFLAGS		= -O2 -ggdb # -Wall      #danger, need -Wall key!!!
LDFLAGS		= -lpthread -lpcre
DEBUG		= -DDEBUG
# INCLUDES	= -I/usr/local/include/boost-1_38
DEPENDS = mydata.cc tqueue.cc thread2.cc      ### i can't set this!!!
OBJECTS = main.o epoll.o pcre.o pipe.o sock.o

all : clean srv                       ### "clean" for no calculate depends

srv : $(OBJECTS) 
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o : %.cpp
	$(CXX) $(CFLAGS) $(DEBUG) $(INCLUDES) -c $< -o $@


run :
	./srv "(\w\.py)|(\w\.txt)"

clean : FORCE
	rm -f *.o* *~ srv

FORCE :
	
