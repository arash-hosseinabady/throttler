#ifndef THROTTLER_FUNC_H
#define THROTTLER_FUNC_H

#include <cstring>
#include <unistd.h>
#include <iostream>
#include <cstdarg>
#include <sys/stat.h>
#include "typedef.h"

extern APP_SETTING GLB_SETTING;

void logger(int err_code, LOG_LEVELS log_level, char *msg, ...);

void get_sys_time(char *format, char *datetime);

#endif //THROTTLER_FUNC_H
