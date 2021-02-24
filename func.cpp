#include "func.h"

void get_sys_time(char *format, char *datetime) {
    struct tm *time_st;
    time_t timer;

    timer = time(NULL);
    time_st = localtime(&timer);
    strftime(datetime, 20, format, time_st);
}

int check_log_dir() {
    char log_dir[255] = {0};
    sprintf(log_dir, "%s/log", GLB_SETTING.app_path);
    struct stat info{};
    if (stat(log_dir, &info) != 0) {
        return mkdir(log_dir, S_IRWXG | S_IRWXU | S_IROTH);
    } else if (info.st_mode & S_IFDIR) {
        return 1;
    }
    return -1;
}

void logger(int err_code, LOG_LEVELS log_level, char *msg, ...) {
    if (check_log_dir() >= 0) {
        FILE *fd = NULL;

        va_list args;
        char log_file[512];
        char log_msg[2048];
        char log_line[2048];
        char strtime[32];
        char *log_type[] = {"INFO", "WARNING", "ERROR", "EXCEPTION"};

        va_start(args, msg);
        vsprintf(log_msg, msg, args);
        va_end(args);

        memset(log_file, 0x00, 512);
        memset(strtime, 0x00, 32);
        get_sys_time("%Y%m%d", strtime);
        sprintf(log_file, "%s/log/%s", GLB_SETTING.app_path, strtime);

        fd = fopen(log_file, "a+");
        if (fd != NULL) {
            memset(strtime, 0x00, 32);
            get_sys_time("%Y%m%d%H%M%S", strtime);
            sprintf(log_line, "%s  [%.6d] [%s]\t%s\n", strtime, err_code, log_type[(int) log_level], log_msg);
            fprintf(fd, log_line);
            fclose(fd);
        }
    } else {
        printf("cannot access to log directory!\n");
    }
}
