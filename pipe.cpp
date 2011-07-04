#include "pipe.hpp"
#include <string.h>

#include <sys/sendfile.h>

const int COUNT_KEYS = 512;
const int COUNT_STR = 1024;

myPipe::myPipe(const char *path )
	:py("python")
{
	if(path == NULL){
		std::cerr << "pipe:empty path" << std::endl;
		exit(1);
	}
	argv2 = new char* [2];
	//for '\0' do len+1
	int size1 = strlen(py) + 1;
	argv2[0] = new char[size1];
	memcpy(argv2[0], py, size1);
	argv2[0][size1 - 1] = '\0'; //may be
	
	int size2 = strlen(path) + 1;
	argv2[1] = new char[size2];
	memcpy( argv2[1], path, size2);
	argv2[1][size2 - 1] = '\0'; //may be

	// filedes[0] for read, filedes[1] for write.
	if(pipe(filedes)){
		perror("error: pipe_create");
	}
}

myPipe::~myPipe()
{
	delete[] argv2[0];
	delete[] argv2[1];
	delete[] argv2;
}

void
myPipe::close_cin()
{
//	dup2(tmp_cin, 0);
	if(filedes[0] > 0) {
	close(filedes[0]);
	filedes[0] = 0;
	}
}

bool
myPipe::lookup(const std::string& path)
{
	return myPipe::lookup(path.c_str() );
}

bool
myPipe::lookup(const char *path)
{
	FILE *f;
	f = fopen(path, "r");
	if(f != NULL){
		fclose(f);
		return true;
	}
	else{
		return false;
	}
}

int
myPipe::get_cin()
{
	return filedes[0];
}

void
myPipe::send_file(int output, int input, size_t len)
{
	char buf[1000];
	size_t count = 0;
	int sz = 1;
	
	while ((count < len)and(sz > 0)) {
		if( (sz = read( input, buf, 1000)) < 0) { break;}
		count += sz;
		if( write( output, buf, sz) < 0) { break;}
	}
}

void
myPipe::doWait()
{
	int stat;
	wait(&stat);
	if(WIFEXITED(stat)){
		printf("wait_OK\n");
	}
	myPipe::close_cin();
}

void
myPipe::doFork()
{
	pid_t pid;
	
	//! Fork
	if ( (pid = fork()) ){
		//! parrent
		printf("Parrent\n");
		close (filedes[1]);
	
		//	sendfile(1, 0, NULL, 100500);
		//  myPipe::send_file(1,filedes[0], 9000);

		// cin is 0
	//	close (filedes[1]);
	//	dup2(0, tmp_cin);
	//	dup2(filedes[0],0);
	//	close(filedes[0]);
		return;
	}
	else {
		//! child
		printf("Child\n");
		std::cerr << "argv2 = " << argv2[0] << " " << argv2[1] << ";" << std::endl;
		// cout is 1
		close (filedes[0]);
		dup2(filedes[1],1);
		close(filedes[1]);
		// set new process
		
		// BRED memory lost
		char **argv1;
		argv1 = new char* [2];
		argv1[0] = argv2[0];
		argv1[1] = argv2[1];
		
		execvp(argv1[0], argv1);
		perror("error: exec child");
		exit(0);
		// catch zombie SIGCHLD
	}	
}
