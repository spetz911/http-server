CXX		= g++
CFLAGS		= -O2 -ggdb -Wall
LDFLAGS		= -lpthread -lpcre

all : pcre pipe

#run:
#	./pcre "\w\.py" < in

#run:
#	./pipe "../1.py"

run:
	./client 127.0.0.1


pcre : tpcre.cpp pcre.o ../pcre.hpp
	$(CXX) $(CFLAGS) tpcre.cpp pcre.o -o pcre $(LDFLAGS)

pcre.o : ../pcre.cpp
	$(CXX) $(CFLAGS) ../pcre.cpp -c $(LDFLAGS)

pipe : tpipe.cpp pipe.o ../pipe.hpp
	$(CXX) $(CFLAGS) tpipe.cpp pipe.o -o pipe 

pipe.o : ../pipe.cpp
	$(CXX) $(CFLAGS) ../pipe.cpp -c 


epoll : tepoll.cpp epoll.o ../epoll.hpp sock.o ../sock.hpp
	$(CXX) $(CFLAGS) tepoll.cpp epoll.o sock.o -o epoll

epoll.o : ../epoll.cpp
	$(CXX) $(CFLAGS) ../epoll.cpp -c

client : client.cpp sock.o ../sock.hpp
	$(CXX) $(CFLAGS) client.cpp sock.o -o client $(LDFLAGS)

sock.o : ../sock.cpp 
	$(CXX) $(CFLAGS) ../sock.cpp -c



clean :
	rm -f *.o* *~ pcre epoll pipe
