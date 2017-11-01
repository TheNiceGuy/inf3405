#include <iostream>
#include <string>

#include "Utils.h"

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

/** Whether the command parsing failed or not. */
static bool fail = false;

void showHelp() {
    cout << "Usage: " << executable << " [OPTIONS]" << endl;
    cout << "This command starts the chat server." << endl;
    cout << endl;
    cout << "The following arguments are valid:" << endl;
    cout << "  -d, --database FILE                 " << "specify the database file" << endl;
    cout << "  -a, --addr ADDRESS                  " << "specify the listening address" << endl;
    cout << "  -p, --port PORT                     " << "specify the listening port" << endl;
    cout << "  -h, --help                          " << "show this help" << endl;
}

char** parseShortCommands(char** cmds, char** last) {
    fail = true;
    
    cout << "Short options not implemented" << endl;

    return cmds;
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
    } else if(string("--help").compare(cmds[0]) == 0) {
        /* the user requested the help */
        showhelp = true;
    } else {
        cout << "Invalid option: " << cmds[0] << endl;
        fail = true;
    }

    return cmds;
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
        if(cmds[0][0] != '-') {
            cout << "Invalid option: " << cmds[0] << endl;
            fail = true;

            continue;
        }

        /* parse the command */
        if(cmds[0][1] == '-')
            cmds = parseLongCommands(cmds, argv);
        else
            cmds = parseShortCommands(cmds, argv);
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
        port = "10025";
    }

    /* TODO: start the server */
    cout << "Starting the server using the following options:" << endl;
    cout << "\t database file    : " << database << endl;
    cout << "\t listening address: " << addr << endl;
    cout << "\t listening port   : " << port << endl;

    return 0;
}
