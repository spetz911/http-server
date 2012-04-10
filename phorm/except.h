/**
 * Common definitions and routines for the TDMN project.
 *
 * 2011. Written by NatSys Lab. (info@natsys-lab.com).
 */
#ifndef __TDMN_H__
#define __TDMN_H__

#include <assert.h>
#include <errno.h>
#include <execinfo.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sstream>
#include <string>

// Environment checks and sesstings.
#ifndef __x86_64__
#warning "Your architecture is not supported. Please use x86-64 only."
#endif
#if !defined(LEVEL1_DCACHE_LINESIZE) || !LEVEL1_DCACHE_LINESIZE
#ifdef LEVEL1_DCACHE_LINESIZE
#undef LEVEL1_DCACHE_LINESIZE
#endif
#define LEVEL1_DCACHE_LINESIZE	64
#endif

#ifndef NDEBUG // Debugging
#define DBG_STMT(x)	x;
#define error_exit(n)	::abort()

#define SCHED_YIELD_SLEEP	100000

#else // No Debugging
#define DBG_STMT(x)
#define error_exit(n)	::exit(n)

// 0.001 sec, equal to Linux 2.6 scheduler tick
#define SCHED_YIELD_SLEEP	1000
#endif

#define INVARIANT(cond)	assert(cond)







class Except : public std::exception {
private:
	static const size_t maxmsg = 256;
	std::string str_;

public:
	Except(const char* fmt, ...) throw()
	{
		va_list ap;
		char msg[maxmsg];
		va_start(ap, fmt);
		vsnprintf(msg, maxmsg, fmt, ap);
		va_end(ap);
		str_ = msg;

		// Add system error code (errno).
		if (errno) {
			std::stringstream ss;
			ss << " (" << strerror(errno)
				<< ", errno=" << errno << ")";
			str_ += ss.str();
		}

		// Add OpenSSL error code if exists.
//		unsigned long ossl_err = ERR_get_error();
//		if (ossl_err) {
//			char buf[256];
//			str_ += std::string(": ")
//				+ ERR_error_string(ossl_err, buf);
//		}

		// Add call trace symbols.
		call_trace();
	}

	~Except() throw()
	{}

	const char *
	what() const throw()
	{
		return str_.c_str();
	}

private:
	void
	call_trace() throw()
	{
		// Do not print more that BTRACE_CALLS_NUM calls in the trace.
		static const size_t BTRACE_CALLS_NUM	= 32;

		void *trace_addrs[BTRACE_CALLS_NUM];
		int n_addr = backtrace(trace_addrs,
				sizeof(trace_addrs) / sizeof(trace_addrs[0]));
		if (!n_addr)
			return;

		char **btrace = backtrace_symbols(trace_addrs, n_addr);
		if (!btrace)
			return;

		for (int i = 0; i < n_addr; ++i)
			str_ += std::string("\n\t") + btrace[i];

		free(btrace);
	}
};

#endif // __TDMN_H__
