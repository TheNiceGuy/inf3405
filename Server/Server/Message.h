#pragma once

#include <types.h>
#include "Utils.h"

/** The maximum length of a username. */
const uint_t NAME_MAX_LENGTH = 32;

/** The maximum length of a password.*/
const uint_t PASS_MAX_LENGTH = 32;

/** The maximum length of the text. */
const uint_t TEXT_MAX_LENGTH = 200;

/**
 * This enumeration defines the types of message.
 */
enum type : uint8_t {
    /** This message is an authentification. */
    MSG_AUTH = 0,

    /** This message is a response from the server. */
    MSG_RESP = 1,

    /** This message is a client sending text to the server. */
    MSG_CLIENT_TEXT = 2,

    /** This message is the server sending text back to the other clients. */
    MSG_SERVER_TEXT = 3,
};

/**
 * This enumeration defines the statuses returned from the server.
 */
enum status : uint8_t {
    /** The last command executed with success. */
    STATUS_OK = 0;

    /** The client isn't authentificated. */
    STATUS_NOT_AUTH = 1;
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
 * This structure defines text that is sent from the server to clients.
 */
struct msg_server_text {
    /** The type of message, it should be MSG_SERVER_TEXT. */
    uint8_t type;

    /** The text that the user sent to the other users. */
    uint8_t text[TEXT_MAX_LENGTH];

    /** The address of the client sending the text. */
    uint8_t addr[4];

    /** The port of the client sending the text. */
    uint32_t port;

    /* TODO: add time */
};

