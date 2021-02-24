#include "main.h"

static int disposing = 0;
static SESSIONS *sessions = {};

int get_free_session() {
    int i;
    for (i = 0; i < GLB_SETTING.request_per_min; i++) {
        printf("sessions[i].stat: %d\n", sessions[i].stat);
        if (sessions[i].stat == SESSION_STATE_IDLE) {
            if (sessions[i].client_socket >= 0) {
                close(sessions[i].client_socket);
            }
            return i;
        }
    }
    return -1;
}

int init_session() {
    free(sessions);
    sessions = static_cast<SESSIONS *>(malloc(GLB_SETTING.request_per_min * sizeof(SESSIONS)));
    memset(sessions, 0x00, GLB_SETTING.request_per_min * sizeof(SESSIONS));

    for (int i = 0; i < GLB_SETTING.request_per_min; i++) {
        sessions[i].id = i;
        sessions[i].stat = SESSION_STATE_IDLE;
        sessions[i].client_socket = -1;
    }
    return 0;
}

void *socket_acceptor() {

    pthread_attr_t attr;
    size_t stack_size = 1048576;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setstacksize(&attr, stack_size);

    int server_sock_fd = -1;
    socklen_t sock_len = sizeof(struct sockaddr);
    struct sockaddr_in server_addr{};
    int opt = 1, si = -1;

    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((unsigned short) GLB_SETTING.socket_port);

    char min1[2], min2[2];
    memset(min1, 0x00, sizeof(min1));
    memset(min2, 0x00, sizeof(min2));
    get_sys_time("%M", min1);
    bool start_flag = true;
    while (disposing != -1) {
        get_sys_time("%M", min2);
        if ((strcmp(min1, min2) < 0) || start_flag) {
            logger(0, INFO, "start new min...");
            start_flag = false;
            sprintf(min1, "%s", min2);
            set_glb_setting();
            init_session();
        }

        si = get_free_session();

        if (si < 0) {
            logger(0, WARNING, "No free session!");
            if (server_sock_fd > 0) {
                close(server_sock_fd);
                server_sock_fd = 1;
            }
            usleep(100);
            continue;
        }

        while (server_sock_fd < 0) {
            if ((server_sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
                logger(errno, ERROR, strerror(errno));
            }

            if (server_sock_fd < 0) {
                logger(errno, ERROR, strerror(errno));
                disposing = -1;
                sleep(1);
                continue;
            }

            if (setsockopt(server_sock_fd, SOL_SOCKET, SO_REUSEADDR, (void *) &opt, sizeof(int)) != 0) {
                logger(errno, ERROR, strerror(errno));
            }

            if (bind(server_sock_fd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr)) == -1) {
                logger(errno, ERROR, "error in binding server socket, error %s", strerror(errno));
                close(server_sock_fd);
                server_sock_fd = -1;
                disposing = -1;
                sleep(1);
                continue;
            }

            printf("wait for request...\n");
            if (listen(server_sock_fd, 20) == -1) {
                logger(errno, ERROR, "error on listening server socket, error %s", strerror(errno));
                close(server_sock_fd);
                server_sock_fd = -1;
                disposing = -1;
                sleep(1);
                continue;
            }
        }

        sessions[si].client_socket = -1;
        sessions[si].client_socket = accept(server_sock_fd, (struct sockaddr *) &server_addr, &sock_len);
        if (sessions[si].client_socket == -1) {
            logger(errno, ERROR, "error in accepting socket, error %s", strerror(errno));
            close(server_sock_fd);
            server_sock_fd = -1;
            usleep(1000);
            continue;
        }
        sessions[si].stat = SESSION_STATE_PROCESSING;
        pthread_create(&sessions[si].thread, &attr, thread_func, (void *) &sessions[si]);

        usleep(100);
    }
    close(server_sock_fd);
    return NULL;

}

int main() {
    disposing = 0;
    set_glb_setting();
    socket_acceptor();
    return 0;
}
