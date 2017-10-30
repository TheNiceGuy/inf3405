#include "Utils.h"
#include "Message.h"

void messageBufferInit(struct msg_buffer* buffer) {
    buffer->pos = 0;
    
    /* zero the buffer */
    uint8_t* buf = buffer->buffer;
    for(uint_t i = 0; i < MESSAGE_MAX_SIZE; i++)
        buf[i] = 0;
}

void rewindMessageBuffer(struct msg_buffer* buffer) {
    /* the first byte of the sent structure should always be the type */
    uint8_t* type = (uint8_t*) buffer->buffer;

    /* get the size of the struct */
    uint_t size;
    switch(*type) {
        case MSG_AUTH:
            size = sizeof(struct msg_auth);
            break;
        case MSG_RESP:
            size = sizeof(struct msg_resp);
            break;
        case MSG_CLIENT_TEXT:
            size = sizeof(struct msg_client_text);
            break;
        case MSG_SERVER_TEXT:
            size = sizeof(struct msg_server_text);
            break;
        default:
            return;
    }

    /* remove the first struct */
    uint8_t* buf = buffer->buffer;
    for(uint_t i = size; i < MESSAGE_MAX_SIZE; i++)
        buf[i-MESSAGE_MAX_SIZE] = buf[i];

    /* update the writing position */
    buffer->pos -= size;
}
