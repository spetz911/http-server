#include <stdexcept>
#include <string>

struct State {
	enum type {
		HTTP_OK,
		HTTP_BAD
	};
};

class HttpParser {
private:
	bool has_body;
	std::string uri;
	std::string host;
	State::type state;
	
	bool c_length_flag;
	size_t c_length;

public:
	HttpParser(char *buf, size_t len);

	size_t
	get_content_length();

	size_t
	write_response_header(int sock_fd) throw(std::exception);

	void
	write_file(const char *path, int sockfd);
	
};



