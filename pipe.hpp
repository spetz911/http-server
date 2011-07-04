#ifndef __PIPE_HPP__
#define __PIPE_HPP__

#include <iostream>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <boost/noncopyable.hpp>

class myPipe : private boost::noncopyable {
private:
	int filedes[2];
//	int tmp_cin;
	char **argv2;
	const char *py;
	char *path1;

public:
	myPipe(const char *path);
	~myPipe();
	void doFork();
	void doWait();
	int get_cin();
	void close_cin();
	bool lookup(const std::string& path);
	bool lookup(const char *path);
	void send_file(int output, int input, size_t len);
};

#endif
