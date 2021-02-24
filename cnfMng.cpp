#include "cnfMng.h"

int mv_cr_lf(char *line) {
    int i = 0;
    int str_len = strlen(line);
    for (i = 0; i < str_len; i++) {
        if ((line[i] == '\r') || (line[i] == '\n') || (line[i] == EOF)) {
            line[i] = 0;
        }
    }
    return 0;
}

int chk_comment(char *line, char cmnt) {
    int i = 0;
    int str_len = strlen(line);
    if (str_len == 0) {
        return 0;
    }
    for (i = 0; i < str_len; i++) {
        if (line[i] != ' ' && line[i] != '\t') {
            break;
        }
    }
    if (line[i] == cmnt) {
        return 0;
    }
    return -1;
}

int parse_line(char *line, char dle, char cmnt, char *key, char *value) {
    int i = 0, ptr = 0;
    char tmp[512] = {0};

    for (i = 0; i < strlen(line); i++) {
        if (line[i] == dle) {
            break;
        }
        if (line[i] != ' ' && line[i] != '\t') {
            tmp[ptr++] = line[i];
        }
    }
    sprintf(key, "%s", tmp);
    memset(tmp, '\0', 256);
    ptr = 0;
    for (i++; i < strlen(line); i++) {
        if (line[i] == cmnt) {
            break;
        }
        if (line[i] != ' ' && line[i] != '\t') {
            tmp[ptr++] = line[i];
        }
    }
    sprintf(value, "%s", tmp);
    return 0;
}

int get_value_by_key(char *file_name, char *key, char *value) {
    FILE *fp = NULL;
    char *cnf_line = NULL;
    size_t len = 0;
    ssize_t read = 0;
    char tmp_key[256] = {0};
    char tmp_value[256] = {0};

    fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("can not open file\n");
        return -1;
    }
    while ((read = getline(&cnf_line, &len, fp)) != -1) {
        if (chk_comment(cnf_line, CMNT) == 0) {
            continue;
        }
        parse_line(cnf_line, DLEM, CMNT, tmp_key, tmp_value);
        if (strcmp(key, tmp_key) != 0) {
            continue;
        }
        mv_cr_lf(tmp_value);
        sprintf(value, "%s", tmp_value);
        fclose(fp);
        free(cnf_line);
        return 0;
    }
    fclose(fp);
    free(cnf_line);
    return -1;
}

void get_app_path() {
    memset(GLB_SETTING.app_path, 0x00, sizeof(GLB_SETTING.app_path));
    getcwd(GLB_SETTING.app_path, 255);
}

void get_config() {
    char cnf_file[512] = {0};
    char tmp[128] = {0};

    sprintf(cnf_file, "%s/config.cnf", GLB_SETTING.app_path);
    memset(tmp, 0, 128);
    if (get_value_by_key(cnf_file, "request_per_min", tmp) == 0) {
        GLB_SETTING.request_per_min = atoi(tmp);
        if (GLB_SETTING.request_per_min <= 0) {
            logger(0, ERROR, "invalid value for request_per_min config");
            exit(-1);
        }
    }
    if (get_value_by_key(cnf_file, "socket_port", tmp) == 0) {
        GLB_SETTING.socket_port = atoi(tmp);
    }
}

void set_glb_setting() {
    get_app_path();
    get_config();
}