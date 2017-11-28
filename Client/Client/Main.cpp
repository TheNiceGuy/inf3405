#include "Utils.h"
#include "Client.h"
#include "Window.h"

#ifdef __LINUX__
    #include <unistd.h>
    #include <sys/types.h>
    #include <pwd.h>
#endif
#ifdef __WIN32__
	#include <windows.h>
	#include <lmcons.h>
	#include <winsock2.h>

	#pragma comment(lib, "Lib/PDCurses/pdcurses.lib")
	#pragma comment(lib, "ws2_32.lib")

	/*************************************************************
	 * ATTENTION: UTILISEZ LE BUILD DEBUG X86 SOUS VISUAL STUDIO *
	 *************************************************************/
#endif
#include <iostream>
#include <string>

#ifdef __LINUX__
	#define EXIT(STATUS) {return (STATUS);}
#endif
#ifdef __WIN32__
	#define EXIT(STATUS) {system("pause"); return (STATUS);}
#endif

using namespace std;

/** The name of the executable. */
static char* executable = nullptr;

/** The server address if specified by the user. */
static const char* addr = nullptr;

/** The server port if specified by the user. */
static const char* port = nullptr;

/** The username used for authentification. */
static const char* user = nullptr;

/** The password used for authentification. */
static const char* pass = nullptr;

/** Whether to prompt the user for the settings. */
static bool prompt = true;

/** Whether the user wants to see the help message. */
static bool showhelp = false;

/** Whether the command parsing failed or not. */
static bool fail = false;

void showHelp() {
    cout << "Usage: " << executable << " [OPTIONS]" << endl;
    cout << "This command starts the chat server." << endl;
    cout << endl;
    cout << "The following arguments are valid:" << endl;
    cout << "  --noprompt                      " << "disable interactive configuration" << endl;
    cout << "  --addr ADDRESS                  " << "specify the server address" << endl;
    cout << "  --port PORT                     " << "specify the server port" << endl;
    cout << "  --user USERNAME                 " << "specify the username" << endl;
    cout << "  --pass PASSWORD                 " << "specify the password" << endl;
    cout << "  --help                          " << "show this help" << endl;
}

char** parseLongCommands(char** cmds, char** last) {
    /* parse a long argument */
    if(string("--noprompt").compare(cmds[0]) == 0) {
        /* disable prompting the configuration */
        prompt = false;
    } else if(string("--addr").compare(cmds[0]) == 0) {
        /* make sure the address is specified */
        if(++cmds == last) {
            cout << "Missing address for \"--addr\" argument." << endl;
            fail = true;
            return --cmds;
        }

        /* copy the filename  */
        addr = cmds[0];
    } else if(string("--port").compare(cmds[0]) == 0) {
        /* make sure the port is specified */
        if(++cmds == last) {
            cout << "Missing port for \"--port\" argument." << endl;
            fail = true;
            return --cmds;
        }

        /* copy the filename  */
        port = cmds[0];
    } else if(string("--user").compare(cmds[0]) == 0) {
        /* make sure the username is specified */
        if(++cmds == last) {
            cout << "Missing usernamefor \"--user\" argument." << endl;
            fail = true;
            return --cmds;
        }

        /* copy the username */
        user = cmds[0];
    } else if(string("--pass").compare(cmds[0]) == 0) {
        /* make sure the password is specified */
        if(++cmds == last) {
            cout << "Missing password for \"--pass\" argument." << endl;
            fail = true;
            return --cmds;
        }

        /* copy the password */
        pass = cmds[0];
    } else if(string("--help").compare(cmds[0]) == 0) {
        /* the user requested the help */
        showhelp = true;
    } else {
        cout << "Invalid option: " << cmds[0] << endl;
        fail = true;
    }

    return cmds;
}

string strAddr;
string strPort;
string strUser;
string strPass;
void promptSettings() {
	cout << "Configuring the server... (empty means default)" << endl;
	cout << "\tPlease enter the server address: ";
	getline(cin, strAddr);
	if (!strAddr.empty()) addr = strAddr.c_str();

	cout << "\tPlease enter the server port: ";
	getline(cin, strPort);
	if (!strPort.empty()) port = strPort.c_str();

	cout << "\tPlease enter the username: ";
	getline(cin, strUser);
	if (!strUser.empty()) user = strUser.c_str();

	cout << "\tPlease enter the password: ";
	getline(cin, strPass);
	if (!strPass.empty()) pass = strPass.c_str();
}

int main(int argc, char** argv) {
    /* get the executable */
    executable = argv[0];

    /* save the pointer to the first parameter */
    char** cmds = argv;

    /* get the end of the list */
    for(int i = 0; i < argc; i++) argv++;

    while(++cmds != argv) {
        /* make sure that argument starts with a dash */
        if(cmds[0][0] != '-' || cmds[0][1] != '-') {
            cout << "Invalid option: " << cmds[0] << endl;
            fail = true;

            continue;
        }

        /* parse the command */
        cmds = parseLongCommands(cmds, argv);
    }

    /* show help if specified */
    if(showhelp) {
        showHelp();
        EXIT(1);
    }

    /* check if the parsing failed */
    if(fail)
		EXIT(1);

	/* prompt for the client's settings */
    if(prompt)
		promptSettings();

    /* check if a listening address was specified */
    if(addr == nullptr) {
        cout << "Using default listening address." << endl;
        addr = "127.0.0.1";
    }

    /* check if a listening port was specified */
    if(port == nullptr) {
        cout << "Using default listening port." << endl;
        port = "5025";
    }

    /* check if a username was specified */
    if(user == nullptr) {
        cout << "Using system's username." << endl;
#ifdef __LINUX__
        struct passwd* passwd = getpwuid(getuid());
        user = passwd->pw_name;
#endif
#ifdef __WIN32__
		char usernamebuf[100];
		DWORD usernamelen = 100;
		GetUserName(usernamebuf, &usernamelen);
		user = usernamebuf;
#endif
    }

    /* check if a password was specified */
    if(pass == nullptr) {
        cout << "Using default weak password." << endl;
        pass = "1234567890";
    }

    /* print current configuration */
    cout << "Connecting to the server using the following options:" << endl;
    cout << "\t server address: " << addr << endl;
    cout << "\t server port   : " << port << endl;
    cout << "\t username      : " << user << endl;
    cout << "\t password      : " << pass << endl;

    /* convert the port into an int */
    int fport;
    try {
        fport = stou(port);
    } catch(const exception& ex) {
        cout << "Error: the specified port isn't valid" << endl;
		EXIT(1);
    }

#ifdef __WIN32__
	/* start the networking system */
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
#endif

    Client* client = nullptr;
    try {
        client = new Client(string(addr), fport);
    } catch(const exception& ex) {
        cout << ex.what() << endl;
    }

    if(client != nullptr) {
        /* initialise the window */
        Window::getInstance().init();

        /* main loop */
        client->mainLoop(user, pass);

        /* destroy the window */
        Window::getInstance().destroy();

        /* free the memory */
        delete client;
    }

#ifdef __WIN32__
	/* stop the networking system */
	WSACleanup();
#endif

	EXIT(0);
}
