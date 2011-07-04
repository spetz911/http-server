#include <iostream>
#include "stdio.h"
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "../pipe.hpp"

using namespace std;
int
main(int argc, char *argv[])
{
	//take path to script
	char dafault_p[] = "../1.py";
	char *argv1 = NULL;
	
	switch (argc) {
	case 1 :
		argv1 = dafault_p;
		break;
	case 2:
		argv1 = argv[1];
		break;
	default:
		std::cerr << "too many args" << std::endl;
	}
	
	myPipe cgi(argv1); //path
	int fd_open;
		
	if (cgi.lookup(argv1)) {
		cgi.doFork();
		fd_open = cgi.get_cin();
		std::cerr << "sending file..." << std::endl;
		cgi.send_file(1, fd_open, 100500);
		cgi.doWait();
	}
	else {
		std::cerr << "no such file or directory" << std::endl;
	}	

//	Debug sending simple file:
//	int fd_open = open( argv1, O_RDONLY, "r");
//	sendfile(0, fd_open, NULL, 100500);

	return 0;
}
