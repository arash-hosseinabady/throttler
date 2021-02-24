#ifndef THROTTLER_HANDLETHREAD_H
#define THROTTLER_HANDLETHREAD_H

#include "typedef.h"
#include "func.h"
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>

void *thread_func(void *args);

#endif //THROTTLER_HANDLETHREAD_H
