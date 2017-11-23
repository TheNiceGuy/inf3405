#ifndef WINDOW_H
#define WINDOW_H

#include "Utils.h"

#ifdef __LINUX__
    #include <ncurses.h>
#endif
#ifdef __WIN32__
#endif
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>

class Window {
public:
    /**
     * This class is a singleton. We don't want the copy constructor or the
     * assign operator.
     */
    Window(const Window& window) = delete;
    void operator=(const Window& window) = delete;

    /**
     * This method returns the instance of the singleton.
     */
    static Window& getInstance();

    void init();

    void destroy();

    std::string getLine(uint_t max);

    void printLine(const std::string& line);

#ifdef __DEBUG__
    void debug(const std::string& line);
#endif


private:
    WINDOW* screen_;
    WINDOW* input_;
    WINDOW* output_;
#ifdef __DEBUG__
    WINDOW* error_;
#endif

    Window();

    std::condition_variable cv_;

    std::unique_lock<std::mutex> lock_;

    std::deque<std::string> queue_;

    std::string text_;
    uint_t size_;
};

#ifdef __DEBUG__
    #define WINDOW_DEBUG(MSG) Window::getInstance().debug((MSG))
#else
    #define WINDOW_DEBUG(MSG)
#endif

#endif
