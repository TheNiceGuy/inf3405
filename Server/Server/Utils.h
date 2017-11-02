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
	#define _WINSOCKAPI_
    #include <windows.h>
    #include <winsock2.h>
    #include <BaseTsd.h>

	typedef SOCKET socket_t;
    typedef unsigned long tid_t;
    typedef SSIZE_T ssize_t;
    typedef HANDLE thread_t;
    typedef HANDLE mutex_t;

    #pragma warning(disable : 4996)
#endif

typedef unsigned int uint_t;

const uint_t MIN_PORT_ALLOWED = 5000;

const uint_t MAX_PORT_ALLOWED = 5050;

std::string getStringNullOrLength(uint8_t* buffer, uint_t len);

bool convertPort(uint_t port, struct sockaddr_in* out);

bool convertAddr(const std::string& addr, struct sockaddr_in* out);

#endif
