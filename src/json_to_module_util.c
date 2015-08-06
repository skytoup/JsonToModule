//
//  json_to_module_util.c
//  SkyJsonToModule
//
//  Created by skytoup on 15/8/3.
//  Copyright (c) 2015年 skytoup. All rights reserved.
//

#include "json_to_module_util.h"
#include <string.h>

void tail_wipe_char(char *str, char wipe_char) {
    if(str) {
        char *tail = str+strlen(str)-1;
        if(wipe_char == *tail) {
            *tail = '\0';
        }
    }
}

void head_lower(char *str) {
    if(str) {
        char *head = str;
        if(*head>='a' && *head<='z') {
            *head|=0x20;
        }
    }

}

void head_capital(char *str) {
    if(str) {
        char *head = str;
        if(*head>='a' && *head<='z') {
            *head&=0xDF;
        }
    }
}

int find_char_reverse(const char *str, char c) {
    int len = (int)strlen(str)-1;
    while (len>=0 && str[len]!=c) {
        --len;
    }
    return len;
}

FILE* open_file(const char *path, const char *mode) {
    FILE *file = fopen(path, mode);
    if(!file) {
        printf("\n\nwarning: fail open file with path : %s\n\n", path);
    }
    return file;
}

#define JSON_TO_MODULE_FILE_READ_BUF (1024)

int merge_file_path(const char *file_path, const char *merge_file_path) {
    FILE *file = open_file(file_path, "a+");
    FILE *m_file = open_file(merge_file_path, "r");
    int success = merge_file(file, m_file);
    if(file) {
        fclose(file);
    }
    if(m_file) {
        fclose(m_file);
    }
    return success;
}

int merge_file(FILE *file, FILE *m_file) {
    int success = 0;
    if(file && m_file) {
        fseek(file, 0, SEEK_END);
        fseek(m_file, 0, SEEK_SET);
        
        char *buf[JSON_TO_MODULE_FILE_READ_BUF];
        size_t size;
        for(;;) {
            size = fread(buf, 1, JSON_TO_MODULE_FILE_READ_BUF, m_file);
            if(size) {
                fwrite(buf, 1, size, file);
            } else {
                success = 1;
                break;
            }
        }
    }
    return success;
}