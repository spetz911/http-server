#include <cstdio>
#include <cstring>

#include <fcntl.h>
#include <unistd.h>

#include <sys/sendfile.h>
#include "http_parser.h"

/*

HTTP/1.1 400 Bad Request
Server: nginx
Date: Mon, 09 Apr 2012 12:08:25 GMT
Content-Type: text/html; charset=windows-1251
Content-Length: 166
Connection: close

HTTP/1.1 200 Ok
Server: nginx
Date: Mon, 09 Apr 2012 12:08:46 GMT
Content-Type: text/html; charset=UTF-8
Connection: close
Cache-Control: no-cache,no-store,max-age=0,must-revalidate
Content-Length: 83176
Expires: Mon, 09 Apr 2012 12:08:47 GMT
Last-Modified: Mon, 09 Apr 2012 12:08:47 GMT

*/


inline size_t
find_space(const char *str, size_t len)
{
	size_t z = 0;
	while (len > 0 && !isspace(*str)) {
		str++;
		len --;
		z++;
	}
	return z;
}


HttpParser::HttpParser(char *buf, size_t len)
	: has_body(false),
	  state(State::HTTP_BAD),
	  c_length_flag(false)
{
#define move(N) buf += (N); len -= (N)
#define skip_spaces() while (len > 0 && isspace(*buf)) {buf++; len --;}

	// Sanity checks
	if (len < sizeof("GET / HTTP/1.1\nHost:\n"))
		return;
	
	if (len > 3 && !strncasecmp(buf, "GET", 3)) {
		move(3);
		has_body = true;
	} else if (len > 4 && !strncasecmp(buf, "HEAD", 4)) {
		move(4);
	} else {
		return;
	}
	skip_spaces();
	/*
	if (*len > 7 && !strncasecmp(*str, "http://", 7)) {
		move(7);
		req->host.str = *str;
		req->host.len = 0;
		while (*len && (isalnum(**str) || **str == '.' || **str == '-')) {
			// Fetch host name from absolute URI.
			// Process plain IP in Host as refering request.
			if (!isdigit(**str))
				referer = 0;
			move(1);
			req->host.len++;
		}
		if (req->host.len) {
			if (**str != '/' && !isspace(**str))
				return HPS_BAD;
			req->relative.str = *str;
			req->relative.len = 0;
			while (*len && !isspace(**str)) {
				req->relative.len++;
				move(1);
			}
		}
	}
	*/
	
	if (*buf != '/' && *buf != '*')
		return;
	// TODO not skip GET\s+http://
	uri = std::string((char *)buf, find_space(buf, len));
	move(uri.length());
	// FIXME HTTP/1.0 does not support
	if (not (len > 8 && !strncasecmp(buf, "HTTP/1.1", 8))) {
		move(8);
	} else {
		return;
	}
	skip_spaces(); // andalso \n\r
	// FIXME should be host the first??
	if (len > 5 && !strncasecmp(buf, "Host:", 5)) {
		move(5);
	} else {
		return;
	}
	skip_spaces();
	if (*(buf-1) == '\n' || *(buf-1) == '\r')
		return; // means empty field
	host = std::string(buf, find_space(buf, len));
	move(host.length());
	// TODO parse another args
	skip_spaces();
	state = State::HTTP_OK;
#undef move
#undef skip_spaces
}


size_t
HttpParser::get_content_length()
{
	if (c_length_flag)
		return c_length;
	else
		return 166; // TODO for wrong request and 
}




size_t
HttpParser::write_response_header(int sockfd) throw(std::exception)
{
#define write_s(S) write_s(std::string(S))
#define write_s2(S) if (len <= response.length()) throw std::logic_error("too small buf");\
                 memcpy(buf, (S).data(), (S).legth()); buf += (S).legth(); len -= (S).legth()
#define write_sock(S) fill_bytes += write(sockfd, (S), sizeof(S))
	size_t fill_bytes = 0;
	
	std::string response;
	switch (state) {
		case State::HTTP_OK:
			write_sock("HTTP/1.1 200 Ok\n");
			break;
		case State::HTTP_BAD:
			write_sock("HTTP/1.1 400 Bad Request\n");
			break;
		default:
			throw std::logic_error("response construction");
	}
	write_sock("Server: nginx\n"); // FIXME
	write_sock("Date: Mon, 09 Apr 2012 21:17:39 GMT\n"); // FIXME
	write_sock("Content-Type: text/html; charset=UTF-8\n"); // FIXME
	write_sock("Connection: close\n");
	write_sock("Content-Length: ");
	
	size_t buf_size = 42;
	char buf[buf_size];
	
	int tmp = snprintf(buf, buf_size, "%d\n", (int)get_content_length());
	if (tmp <= 0)
		throw std::logic_error("Content-Length snprintf exception");
	fill_bytes += write(sockfd, buf, tmp); // FIXME 'write' can return bad value
	write_sock("\n"); // pre-body empty string
	
	return fill_bytes;
#undef write_sock
#undef write_s2
#undef write_s
}


void
HttpParser::write_file(const char *path, int sockfd)
{
//	std::cout << "send file, before open" << std::endl;
	int fd_open = open(path, O_RDONLY, "r");
	if (fd_open > 0)
		sendfile(sockfd, fd_open, NULL, 100500);
	else
		throw std::logic_error("file not found exception");
	return;
}

