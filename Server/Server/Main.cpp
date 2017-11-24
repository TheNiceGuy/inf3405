#include "Utils.h"
#include "Server.h"

#ifdef __LINUX__
    #include <csignal>
#endif
#ifdef __LINUX__
	#pragma comment(lib, "ws2_32.lib")
#endif
#include <iostream>
#include <string>

using namespace std;

/** The name of the executable. */
static char* executable = nullptr;

/** The database file if specified by the user. */
static const char* database = nullptr;

/** The listening address if specified by the user. */
static const char* addr = nullptr;

/** The listening port if specified by the user. */
static const char* port = nullptr;

/** Whether the user wants to see the help message. */
static bool showhelp = false;

/** Whether the user wants to see the help message. */
static string test = string("none");

/** Whether the command parsing failed or not. */
static bool fail = false;

void showHelp() {
    cout << "Usage: " << executable << " [OPTIONS]" << endl;
    cout << "This command starts the chat server." << endl;
    cout << endl;
    cout << "The following arguments are valid:" << endl;
    cout << "  --database FILE                 " << "specify the database file" << endl;
    cout << "  --addr ADDRESS                  " << "specify the listening address" << endl;
    cout << "  --port PORT                     " << "specify the listening port" << endl;
    cout << "  --help                          " << "show this help" << endl;
    cout << "  --test MODULE                   " << "test a part of this program" << endl;
}

char** parseLongCommands(char** cmds, char** last) {
    /* parse a long argument */
    if(string("--database").compare(cmds[0]) == 0) {
        /* make sure the filename is specified */
        if(++cmds == last) {
            cout << "Missing filename for \"--database\" argument." << endl;
            fail = true;
            return --cmds;
        }

        /* copy the filename  */
        database = cmds[0];
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
    } else if(string("--test").compare(cmds[0]) == 0) {
        /* make sure the port is specified */
        if(++cmds == last) {
            cout << "Missing module for \"--test\" argument." << endl;
            fail = true;
            return --cmds;
        }

        /* copy the filename  */
        test = string(cmds[0]);
    } else if(string("--help").compare(cmds[0]) == 0) {
        /* the user requested the help */
        showhelp = true;
    } else {
        cout << "Invalid option: " << cmds[0] << endl;
        fail = true;
    }

    return cmds;
}

/**
 * This method test the database.
 *
 * @param file The database file.
 */
void testDatabase(const string& file);

/** The server object. */
static Server* server = nullptr;

#ifdef __LINUX__
#define RETURN_VALUE 
void sigint_handler(int signal) {
#endif
#ifdef __WIN32__
#define RETURN_VALUE true
BOOL WINAPI sigint_handler(DWORD signal) {
#endif
    /* make sure the server have been created */
    if(server == nullptr)
        return RETURN_VALUE;

    /* shutdown the server */
    server->stop();

	return RETURN_VALUE;
}
#undef RETURN_VALUE

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
        return 1;
    }

    /* check if the parsing failed */
    if(fail)
        return 1;

    /* check if a database file was specified */
    if(database == nullptr) {
        cout << "Using default database file." << endl;
        database = "data.bin";
    }

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

    /* check if the user wants to run some test */
    if(test != "none") {
        if(test == "database") {
            testDatabase(database);
        } else {
            cout << "Unsupported module for testing." << endl;
            return 1;
        }

        return 0;
    }

    /* print current configuration */
    cout << "Starting the server using the following options:" << endl;
    cout << "\t database file    : " << database << endl;
    cout << "\t listening address: " << addr << endl;
    cout << "\t listening port   : " << port << endl;

    /* convert the port into an int */
    int fport;
    try {
        fport = stou(port);
    } catch(const exception& ex) {
        cout << "Error: the specified port isn't valid" << endl;
        return 1;
    }

#ifdef __LINUX__
    signal(SIGINT, sigint_handler);
#endif
#ifdef __WIN32__
	SetConsoleCtrlHandler(sigint_handler, TRUE);
#endif

    /* initialise the server */
    try {
        server = new Server(database, addr, fport);
        server->waitConnexion();
    } catch(const exception& e) {
        cout << e.what() << endl;
    }

    delete server;

    return 0;
}

void testDatabase(const string& file) {
    Database db(file);
    db.init();
    db.print();

    db.addMsg(new MessageServerText("user1", 1000, 1010, 5050, "hello world"));
    db.addMsg(new MessageServerText("user2", 2000, 3000, 5051, "sup m8"));
    db.addMsg(new MessageServerText("user1", 3000, 1010, 5050, "the quick brown fox jumps over the lazy dog"));

    db.addUser("user1", "password");
    db.addUser("user2", "passw0rrd123");
    db.addUser("user3", "1234567890");

    db.save();

}
