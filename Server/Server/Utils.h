#ifndef UTILS_H
#define UTILS_H

#include <string>

#ifdef __LINUX__
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR   -1

    typedef int socket_t;
    typedef pid_t tid_t;
    typedef pthread_t thread_t;
    typedef pthread_mutex_t mutex_t;
#endif

#ifdef _MSC_VER
    #define __WIN32__
#endif

#ifdef __WIN32__
	typedef long long unsigned int socket_t;
    typedef unsigned long tid_t;
    typedef long long int ssize_t;
    typedef void* thread_t;
    typedef void* mutex_t;

    #ifdef _MSC_VER
        #pragma warning(disable : 4996)
    #endif
#endif

typedef unsigned int uint_t;

const uint_t MIN_PORT_ALLOWED = 5000;

const uint_t MAX_PORT_ALLOWED = 5050;

std::string getStringNullOrLength(uint8_t* buffer, uint_t len);

bool convertPort(uint_t port, struct sockaddr_in* out);

bool convertAddr(const std::string& addr, struct sockaddr_in* out);

uint_t stou(const std::string& str, size_t* idx = 0, int base = 10);

#ifdef __WIN32__
std::wstring widen(const std::string& input);
#endif

#endif
