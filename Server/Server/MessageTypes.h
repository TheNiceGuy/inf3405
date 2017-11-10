#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>
#include "Utils.h"

/** The maximum length of a username. */
const uint_t NAME_MAX_LENGTH = 32;

/** The maximum length of a password.*/
const uint_t PASS_MAX_LENGTH = 32;

/** The maximum length of the text. */
const uint_t TEXT_MAX_LENGTH = 200;

/** The maximum length of a message structure. */
const uint_t MESSAGE_MAX_SIZE = 400;

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
    DB_MESSAGE = 4,

    /** This structure is a database user. */
    DB_USER = 5,
};

/**
 * This enumeration defines the statuses returned from the server.
 */
enum status : uint8_t {
    /** The last command executed with success. */
    STATUS_OK = 0,

    /** The client isn't authentificated. */
    STATUS_NOT_AUTH = 1,
};

/**
 * This structure defines the authentification message of a user.
 */
struct msg_auth {
    /** The type of message, it should be MSG_AUTH. */
    uint8_t type;

    /** The name of the user. */
    uint8_t name[NAME_MAX_LENGTH];

    /** The password of the user. */
    uint8_t pass[PASS_MAX_LENGTH];
};

/**
 * This structure defines the message when the user quits.
 */
struct msg_quit {
    /** The type of message, it should be MSG_QUIT. */
    uint8_t type;
};

/**
 * This structure defines a response from the server.
 */
struct msg_resp {
    /** The type of message, it should be MSG_RESP. */
    uint8_t type;

    /** This status returned from the server. */
    uint8_t status;
};

/**
 * This structure defines text that is sent from a user to the server.
 */
struct msg_client_text {
    /** The type of message, it should be MSG_CLIENT_TEXT. */
    uint8_t type;

    /** The text that the user sent. */
    uint8_t text[TEXT_MAX_LENGTH];
};

/**
 * This structure defines a message stored in the database.
 */
struct db_message {
    /** The type of message, it should be DB_MESSAGE. */
    uint8_t type;

    /** The name of the user sending the message.  */
    uint8_t name[NAME_MAX_LENGTH];

    /** The address of the client sending the text. */
    uint8_t addr[4];

    /** The port of the client sending the text. */
    uint32_t port;

    /** The time  */
    uint64_t time;

    /** The text that the user sent to the other users. */
    uint8_t text[TEXT_MAX_LENGTH];
};

/**
 * This structure defines a user stored in the database.
 */
struct db_user {
    /** The type of the message, it should be DB_USER. */
    uint8_t type;

    /** The name of the user. */
    uint8_t name[NAME_MAX_LENGTH];

    /** The password of the user. */
    uint8_t pass[PASS_MAX_LENGTH];
};

/**
 * This structure is used to read message from the socket efficiently. After
 * the initialisation, buffer will look like this :
 *
 *     | 0 | 0 | ... | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
 *       ^
 *       |
 *
 * The arrow denotes the position to write. At first the buffer is zeroed.
 * After a read from the socket, we might get something like this :
 *
 *     |------ complete message struct -----| partial message struct |
 *                                                                     ^
 *                                                                     |
 *
 * The client should always send a complete struct and it will always fit
 * inside the buffer. Thus, a message should always be ready to be process
 * after a read from the socket.
 *
 * After the message has been processed, we might want to read more messages.
 * In order to reuse the same buffer without allocating more memory, we have
 * to rewind it. Basically, it means removing the first struct from the buffer
 * like this :
 *
 *     |-- partial message struct -| X | X | X | ... | X | X | X | X |
 *                                   ^
 *                                   |
 *
 * Thus, we can keep one reading more data to complete the incomplete struct :
 *
 *     |------- complete message struct -----| X | X | X | X | X | X |
 *                                             ^
 *                                             |
 */
struct msg_buffer {
    /** The current position to write. */
    ssize_t pos;

    /** The buffer itself. */
    uint8_t buffer[MESSAGE_MAX_SIZE];
};

/**
 * This function initialise a reading buffer.
 *
 * @param buffer The buffer to initialise.
 */
void messageBufferInit(struct msg_buffer* buffer);

/**
 * This function rewind the buffer.
 *
 * @param buffer The buffer to rewind.
 */
void rewindMessageBuffer(struct msg_buffer* buffer);

#endif
