#include "Window.h"

#ifdef __LINUX__
    #include <poll.h>
    #include <unistd.h>
    #include <stdio.h>
#endif
#ifdef __WIN32__
#endif
#include <stdexcept>

using namespace std;

void wrelmoveyx(WINDOW* win, int y, int x) {
    int cury, curx;
    getyx(win, cury, curx);
    wmove(win, cury+y, curx+x);
}

Window& Window::getInstance() {
    static Window instance;
    return instance;
}

Window::Window() {

}

void Window::init() {
    /* initialise ncurses */
    screen_ = initscr();

    /* get the dimensions of the terminal */
    int winx, winy;
    getmaxyx(screen_, winy, winx);

    /* create the input window */
    input_ = newwin(3, winx, winy-3, 0);
    wborder(input_, '|', '|', '-','-','+','+','+','+');
    wmove(input_, 1, 1);
    wtimeout(input_, 1);
    wrefresh(input_);

    /* create the output window */
#ifdef __DEBUG__
    output_ = newwin(winy-9, winx, 0, 0);
#else
    output_ = newwin(winy-3, winx, 0, 0);
#endif
    scrollok(output_, TRUE);
    wrefresh(output_);

#ifdef __DEBUG__
    /* create the error window */
    error_ = newwin(6, winx, winy-9, 0);
    scrollok(error_, TRUE);
    wmove(error_, 1, 1);
    wtimeout(error_, 1);
    wrefresh(error_);
#endif

    /* disable character echoing */
    noecho();

    /* reset input text */
    text_ = "";
    size_ = 0;
}

void Window::destroy() {
    /* delete windows */
    delwin(input_);
    delwin(output_);
#ifdef __DEBUG__
    delwin(error_);
#endif

    /* remove ncurses */
    endwin();
}

string Window::getLine(uint_t max) {
    string ret = "";

    /* loop until a line is built */
    int c;
    while(true) {
        /* get a single character */
        c = wgetch(input_);

        /* timeout means no more data  */
        if(c == ERR)
            break;

        /* newline means the user wants to send this string */
        if(c == '\n')
            break;

        /* check if it's a backspace */
        if(c == 127) {
            /* check if we can delete a character */
            if(size_ <= 0)
                continue;

            /* delete the character */
            text_.pop_back();
            size_--;

            /* remove a character */
            wrelmoveyx(input_, 0, -1);
            waddch(input_, ' ');
            wrelmoveyx(input_, 0, -1);

            continue;
        }

        /* make sure the user doesn't write a string too long */
        if(size_ >= max)
            continue;

        /* append the character */
        text_ += c;
        size_++;

        /* echo the written character */
        waddch(input_, c);
    }

    if(c == '\n') {
        /* clear the window */
        werase(input_);
        wborder(input_, '|', '|', '-','-','+','+','+','+');
        wmove(input_, 1, 1);
        wrefresh(input_);

        ret = text_;

        /* reset the text */
        text_ = "";
        size_ = 0;
    }

    return ret;
}

void Window::printLine(const string& line) {
    wprintw(output_, "%s\n", line.c_str());
    wrefresh(output_);
}

#ifdef __DEBUG__
void Window::debug(const string& line) {
    wprintw(error_, "%s\n", line.c_str());
    wrefresh(error_);
}
#endif
