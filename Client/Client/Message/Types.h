#ifndef MESSAGE_TYPES_H
#define MESSAGE_TYPES_H

/**
 * This enumeration defines the types of message.
 */
enum type : uint8_t {
    /** This message is an authentification. */
    MSG_AUTH = 0,

    /** This message is when the user quits. */
    MSG_QUIT = 1,

    /** This message is a response from the server. */
    MSG_RESP = 2,

    /** This message is a client sending text to the server. */
    MSG_CLIENT_TEXT = 3,

    /** This structure is a database message. */
    MSG_SERVER_TEXT = 4,
};

#endif
