#ifndef PS_SERVER_TYPEDEF_H
#define PS_SERVER_TYPEDEF_H

#include <cstdint>
#include <pthread.h>

#define SESSION_STATE_IDLE          0
#define SESSION_STATE_PROCESSING    1

typedef enum LOG_LEVELS_ {
    INFO, WARNING, ERROR, EXCEPTION
} LOG_LEVELS;

typedef struct APP_SETTING_ {
    char app_path[512];
    uint8_t request_per_min;
    uint16_t socket_port;
} __attribute__((packed)) APP_SETTING;

typedef struct _SESSIONS {
    uint8_t id;
    pthread_t thread;
    uint8_t stat;
    uint8_t client_socket;
    uint8_t data[128];
} SESSIONS;


#endif //PS_SERVER_TYPEDEF_H
