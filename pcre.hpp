#ifndef __PCRE_HPP__
#define __PCRE_HPP__

#include <iostream>
#include <libgen.h>
#include <string.h>
#include <pcre.h>
#include <boost/noncopyable.hpp>

// what is empty string??

class myPCRE : private boost::noncopyable {
private:
	pcre *re;
public:
	myPCRE (const std::string &pattern);
	~myPCRE ();
	void compile( const std::string &pattern);
	std::string test(const std::string &str);
	char *test(const char *str);
	};

#endif
