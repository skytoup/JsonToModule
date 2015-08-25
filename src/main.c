//
//  main.m
//  SkyJsonToModule
//
//  Created by skytoup on 15/7/31.
//  Copyright (c) 2015年 skytoup. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "json_to_module.h"
#include "out_java.h"
#include "out_objective-c.h"
#include "json_to_module_util.h"

#include <unistd.h>
#include <getopt.h>

void print_help() {
    printf("\njson2moudle <json file path> [-n] [-o] [-p] [-h] [--java] [--objc]\n"
           "\n"
           "option:\n"
           "-n <module name> \t default is json file name\n"
           "-o <out path> \t\t default is run path\n"
           "-p <java pack name> \t default is \"\"\n"
           "-h \t\t\t help\n"
           "--java \t\t\t out java module file\n"
           "--objc \t\t\t out objective-c module file\n\n"
           "if not have --java or --objective-c, default is java\n\n");
}

void print_opt_err(char opt) {
    printf("Invalid Option character \'%c\',add the option '-h' get some help.\n",opt);
}

void print_not_input_file() {
    printf("not file input\n");
}


const struct option long_options[] = {
    {"java", no_argument, NULL, 'j'},
    {"objc", no_argument, NULL, 'c'},
    {0, 0, 0, 0}
};

char *json_file_path = NULL;
char *class_name = NULL;
char *out_path = NULL;
char *pack_name = NULL;
int need_java = 0;
int need_objc = 0;

void handle_arg(int argc, const char * argv[]) {
    
    int opt;
    int option_index = 0;
    while ( (opt = getopt_long(argc, (void*)argv, "n:o:p:h", long_options, &option_index)) != -1)
    {
        switch (opt) {
            case 'n':
                class_name = optarg;
                break;
            case 'o':
                out_path = optarg;
                break;
            case 'p':
                pack_name = optarg;
                break;
            case 'h': {
                print_help();
                exit(0);
                break;
            }
            case 'j': {
                need_java = 1;
                break;
            }
            case 'c': {
                need_objc = 1;
                break;
            }
            case '?': {
                print_opt_err(optopt);
                exit(0);
                break;
            }
            default:
                break;
        }
    }
    if(argc < 2) {
        print_not_input_file();
        exit(0);
    }
    const char *arg_1 = argv[argc-1];
    if(arg_1[0] == '-') {
        print_not_input_file();
        exit(0);
    }
    json_file_path = malloc(sizeof(char)*(strlen(arg_1)+1));
    strcpy(json_file_path, arg_1);
}

int main(int argc, const char * argv[]) {
    handle_arg(argc, argv);
    
    int need_free_class_name = 0;
    int need_free_out_path = 0;
    
    if(!class_name) {
        // json file name
        int sl = find_char_reverse(json_file_path, '/');
        int el = find_char_reverse(json_file_path, '.');
        sl = (sl==-1?0:sl+1);
        el = (el==-1?((int)strlen(json_file_path)-1):el-1);
        class_name = malloc(sizeof(char)*(el-sl+1));
        need_free_class_name = 1;
        strncpy(class_name, json_file_path+sl, el-sl+1);
    }
    if(!out_path) {
        // current path
        char *cur_path = malloc(1024*sizeof(char));
        getcwd(cur_path, 1024);
        out_path = malloc(sizeof(char)*(strlen(cur_path)+1));
        need_free_out_path = 1;
        strcpy(out_path, cur_path);
        free(cur_path);
    }
    if(!pack_name) {
        pack_name = "";
    }
    
    if(((need_java^need_objc)|need_objc|need_java) == 0) {
        need_java = 1;
    }
    
    link_m *m = link_m_create();
    link_m_set_func_free(m, obj_free);

    FILE *file = open_file(json_file_path, "r");
    if(file==NULL) {
        exit(0);
    }
    fseek(file, 0, SEEK_END);
    long flen = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *text = malloc(sizeof(char)*flen+1);
    memset(text, 0, sizeof(char)*flen+1);
    char *buf = malloc(1024*sizeof(char));
    size_t size = 0;
    for(;;) {
        size = fread(buf, 1, 1024, file);
        if(size) {
            strncat(text, buf, size);
        } else {
            break;
        }
    }
    free(buf);
    
    cJSON *json = cJSON_Parse(text);
    if(!json) { printf("json parse error : %s\n", cJSON_GetErrorPtr()); exit(0); }
    if(json->type == cJSON_Array) {
        json = json->child;
    }
    handle_object(m, json, class_name);
    if(need_java) {
        out_result_java(m, out_path, pack_name);
    }
    if(need_objc) {
        out_result_oc(m, out_path);
    }
    
    link_m_destory(m);
    cJSON_Delete(json);
    
    
    if(need_free_class_name) {
        free(class_name);
    }
    if(need_free_out_path) {
        free(out_path);
    }
    free(json_file_path);
    
    printf("\ndone\n");
    return 0;
}
