#include "Utils.h"

#ifdef __LINUX__
    #include <arpa/inet.h>
#endif
#ifdef __WIN32__
    #include <ws2tcpip.h>
#endif

using namespace std;

string getStringNullOrLength(uint8_t* buffer, uint_t len) {
    int pos = len-1;

    /* check if there's a null byte in the buffer */
    while(pos >= 0 && buffer[pos] != '\0') pos--;

    /* if there's not a null byte, the entire buffer is the string */
    if(pos < 0)
        return string((char*) buffer, len);

    /* there's a null byte */
    return string((char*) buffer);
}

bool convertPort(uint_t port, struct sockaddr_in* out) {
    /* lower bound */
    if(port < MIN_PORT_ALLOWED)
        return false;

    /* upper bound */
    if(port > MAX_PORT_ALLOWED)
        return false;

    /* convert the port */
    out->sin_port = htons(port);

    /* should be good */
    return true;
}

bool convertAddr(const std::string& addr, struct sockaddr_in* out) {
    /* convert the IP address */
    out->sin_addr.s_addr = inet_addr(addr.c_str());
    
    /* return error */
    return (out->sin_addr.s_addr != INADDR_NONE);
}
