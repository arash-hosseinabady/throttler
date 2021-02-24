#include "handleThread.h"

void *thread_func(void *args) {
    SESSIONS *my_session = (SESSIONS *) args;
    printf("in thread_func\n");
    int dispose = 1;
    while (dispose) {
        switch (my_session->stat) {
            case SESSION_STATE_IDLE:
                usleep(100);
                break;
            case SESSION_STATE_PROCESSING:
                if (recv(my_session->client_socket, my_session->data, sizeof(my_session->data), 0) < 0) {
                    logger(errno, ERROR, strerror(errno));
                    exit(-1);
                }

                //handle client request
                char srv_response[255];
                memset(srv_response, 0x00, sizeof(srv_response));
                sprintf(srv_response, "server get your message: %s", my_session->data);
                if (send(my_session->client_socket, srv_response, sizeof(srv_response), 0) < 0) {
                    logger(errno, ERROR, strerror(errno));
                    exit(-1);
                }

                sleep(1);
                close(my_session->client_socket);
                my_session->stat = SESSION_STATE_IDLE;
                dispose = 0;
                break;
        }
    }
    pthread_exit(NULL);
}
