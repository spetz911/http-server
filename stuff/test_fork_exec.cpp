#include <iostream>
#include "stdio.h"
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


#include "../pipe.hpp"

using namespace std;
int
main(int argc, char *argv[])
{
	char** argv2 = new char* [2];
	argv2[0] = "python\0";
	argv2[1] = "1.py\0";
	
	pid_t pid;
	//! Fork
	if ( (pid = fork()) ){
		//! parrent
		printf("Parrent\n");
		int stat;
		wait(&stat);
		if(WIFEXITED(stat)){
			printf("wait_OK\n");
		}
		return 0;
	}
	else {
		//! child
		printf("Child\n");
		std::cerr << "argv2 = " << argv2[0] << " " << argv2[1] << std::endl;
		
		execvp(argv2[0], argv2);
		perror("error: fork child");
		exit(1);
		// catch zombie SIGCHLD
	}

	std::cerr << "argv2 = " << argv2[0] << " " << argv2[1] << std::endl;
	execvp(argv2[0], argv2);
	perror("error: exec");
	exit(1);
}
