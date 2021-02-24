#ifndef THROTTLER_CNFMNG_H
#define THROTTLER_CNFMNG_H

#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <cctype>
#include "typedef.h"
#include "func.h"

#define DLEM		'='
#define CMNT		'#'

extern APP_SETTING GLB_SETTING;

void set_glb_setting();

#endif //THROTTLER_CNFMNG_H
