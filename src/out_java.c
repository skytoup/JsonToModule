//
//  out_java.c
//  SkyJsonToModule
//
//  Created by skytoup on 15/8/3.
//  Copyright (c) 2015年 skytoup. All rights reserved.
//

#include "out_java.h"
#include <stdlib.h>
#include <string.h>
#include "out_data.h"
#include "json_to_module_util.h"

int java_ready_out(OUT_RESULT_FUNCS *funcs) {
    out_data *out_data = funcs->data;
    FILE *f = open_file(out_data->path, "r");
    if(!f) {
        return 0;
    }
    return 1;
}

int java_out_class(OUT_RESULT_FUNCS *funcs, const char* class_name) {
    head_capital((char*)class_name);
    out_data *out_data = funcs->data;
    size_t size = strlen(out_data->path)+strlen(class_name)+strlen(".java")+3+1;
    char *fn = (char*)malloc(sizeof(char)*size);
    memset(fn, 0, size);
    
    sprintf(fn, "%s/%s%s", out_data->path, class_name, ".java");
    FILE *f = open_file(fn, "w+");
    if(f) {
        FILE *head_info = open_file("java_head.txt", "r");
        if(head_info) {
            merge_file(f, head_info);
            fclose(head_info);
            fputc('\n', f);
            fputc('\n', f);
        }
        out_data->file = f;
    } else {
        free(fn);
        return 0;
    }
    
    strcpy(fn+size-4, "tmp");
    FILE *f_tmp = open_file(fn, "w+");
    if(f_tmp) {
        fprintf(f_tmp, "\npublic class %s {\n\n", class_name);
        out_data->tmp_file = f_tmp;
        out_data->tmp_path = fn;
        return 1;
    } else {
        free(fn);
        return 0;
    }
}

void java_will_out_property(OUT_RESULT_FUNCS *funcs) { // 准备输出属性
    //
}

void java_out_property(OUT_RESULT_FUNCS *funcs, const char* property_name, PropertyType property_type, PropertyType obj_type) {
    out_data *out_data = funcs->data;
    char *property_str = NULL;
    switch (property_type) {
        case PropertyTypeArray: {
            char *arr_p = NULL;
            switch (obj_type) {
                case PropertyTypeArray:
                    //
                    break;
                case PropertyTypeObject:
                    arr_p = malloc(sizeof(char)*(strlen(property_name)+1));
                    strcpy(arr_p, property_name);
                    head_capital(arr_p);
                    tail_wipe_char(arr_p, 's');
                    char *pack_name = out_data->data;
                    if(pack_name) {
                        fprintf(out_data->file, "import %s.%s;\n", pack_name, arr_p);
                    } else {
                        fprintf(out_data->file, "import %s;\n", arr_p);
                    }
                    break;
                case PropertyTypeBool:
                    arr_p = "Booleam";
                    break;
                case PropertyTypeNumber:
                    arr_p = "Double";
                    break;
                case PropertyTypeString:
                    arr_p = "String";
                    break;
            }
            property_str = malloc(sizeof(char)*(strlen(arr_p)+strlen("ArrayList<>")+1));
            sprintf(property_str, "ArrayList<%s>", arr_p);
            break;
        }
        case PropertyTypeString:
            property_str = "String";
            break;
        case PropertyTypeObject:
            property_str = malloc(sizeof(char)*(strlen(property_name)+1));
            strcpy(property_str, property_name);
            head_capital(property_str);
            fprintf(out_data->file, "import %s;\n", property_str);
            break;
        case PropertyTypeNumber:
            property_str = "Double";
            break;
        case PropertyTypeBool:
            property_str = "booleam";
            break;
    }
    fprintf(out_data->tmp_file, "\tprivace %s %s;\n", property_str, property_name);
    if(property_type == PropertyTypeObject || property_type == PropertyTypeArray) {
        free(property_str);
    }
}

void java_to_next_property(OUT_RESULT_FUNCS *funcs) {
    //
}

void java_to_next_class(OUT_RESULT_FUNCS *funcs) {

    out_data *out_data = funcs->data;
    fprintf(out_data->tmp_file, "\n}");
    
    fflush(out_data->file);
    fflush(out_data->tmp_file);
    
    merge_file(out_data->file, out_data->tmp_file);

    fclose(out_data->file);
    fclose(out_data->tmp_file);
    
    remove(out_data->tmp_path);
    free(out_data->tmp_path);
}

void java_out_end(OUT_RESULT_FUNCS *funcs) { 
    //
}

void out_result_java(link_m *m, const char *path, const char *pack_name) {
    out_data *out_data_t = malloc(sizeof(struct out_data));
    memset(out_data_t, 0, sizeof(out_data));
    
    if(pack_name && strlen(pack_name)) {
        out_data_t->data = malloc(sizeof(char)*(strlen(path)+1));
        strcpy(out_data_t->data, pack_name);
        tail_wipe_char(out_data_t->data, '.');
    }
    if(path && strlen(path)) {
        out_data_t->path = malloc(sizeof(char)*(strlen(path)+1));
        strcpy(out_data_t->path, path);
        tail_wipe_char(out_data_t->path, '/');
    }
    
    out_result_funcs funcs = {java_ready_out, java_out_class, java_will_out_property, java_out_property, java_to_next_property, java_to_next_class, java_out_end, out_data_t};
    out_result(m, &funcs);
    
    if(out_data_t->data) {
        free(out_data_t->data);
    }
    if(out_data_t->path) {
        free(out_data_t->path);
    }
    free(out_data_t);
}
