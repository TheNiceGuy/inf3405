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

#ifdef _MSC_VEC
    #define __WIN32__
#endif

#if __WIN32__
    #include <windows.h>

    typedef SOCKET socket_t;
    typedef DWORD tid_t;
    typedef HANDLE thread_t;
    typedef HANDLE mutex_t;
#endif

typedef unsigned int uint_t;

const uint_t MIN_PORT_ALLOWED = 10000;

const uint_t MAX_PORT_ALLOWED = 10050;

std::string getStringNullOrLength(uint8_t* buffer, uint_t len);

bool convertPort(uint_t port, struct sockaddr_in* out);

bool convertAddr(const std::string& addr, struct sockaddr_in* out);

#endif
