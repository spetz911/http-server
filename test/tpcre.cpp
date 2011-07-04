#include <iostream>
#include "stdio.h"

#include "../pcre.hpp"

using namespace std;
int
main(int argc, char *argv[])
{
	std::string path;

	std::string rec;
	
	//take pcre pattern
	char dafault_p[] = "test";
	char *argv1 = NULL;
	
	switch (argc) {
	case 1 :
		argv1 = dafault_p; // pattern (regexp)
		break;
	case 2:
		argv1 = argv[1];
		break;
	default:
		std::cerr << "too many args" << std::endl;
	}
	
//	char* pattern = (char *) argv1;
	myPCRE parser(argv1);

	std::string msg("");
	char sss;
	sss = getc(stdin);
	while(!feof(stdin)){
		msg += sss;
		sss = getc(stdin);
	}
	
	path = parser.test(msg);
	
	cout << "<" << path << ">"<< endl;
	cout << "text:" << endl << "\"\"\"" << endl << msg << "\"\"\""<< endl;
	
	
	return 0;
}
