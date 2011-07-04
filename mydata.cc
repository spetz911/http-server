#ifndef __MYDATA_CC__
#define __MYDATA_CC__

#include <string.h>

//add operator= and test it

// FIXME
// Объявление класса нужно было вынести в mydata.h
// А реализацию методов оставить в mydata.cc
// Инклюдим mydata.h в main.cpp
class myData
{
private:
	int sockfd;
	const char *msg;

public:
	myData();
	myData(const myData &other);
	~myData();
	
	void setMsg(const char *str);
	const char* getMsg() const;
	void setfd(int fd);
	int getfd() const;

};


myData::myData()
:msg(NULL), sockfd(0) {}

myData::myData(const myData &other)
:msg(NULL), sockfd(0)
{
	setfd(other.getfd());
	setMsg(other.getMsg());
}

myData::~myData(){
}

void
myData::setMsg(const char *str)
{
		msg = str;
// FIXME такие простые функции лучше делать инлайновыми (определять прям в теле
// объявления класса)
}

const char*
myData::getMsg() const
{
	return msg;
}

void
myData::setfd(int fd)
{
	sockfd = fd;
}

int
myData::getfd() const
{
	return sockfd;
}

#endif
