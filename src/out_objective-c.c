//
//  out_objective-c.c
//  SkyJsonToModule
//
//  Created by skytoup on 15/8/3.
//  Copyright (c) 2015年 skytoup. All rights reserved.
//

#include "out_objective-c.h"
#include "out_data.h"
#include "json_to_module_util.h"
#include <string.h>
#include <stdlib.h>

int oc_ready_out(OUT_RESULT_FUNCS *funcs) {
    out_data *out_data = funcs->data;
    FILE *f = open_file(out_data->path, "r");
    if(!f) {
        return 0;
    }
    return 1;
}

int oc_out_class(OUT_RESULT_FUNCS *funcs, const char* class_name) {
    head_capital((char*)class_name);
    out_data *out_data = funcs->data;
    size_t size = sizeof(char)*(strlen(out_data->path)+strlen(class_name)+strlen(".h")+3+1);
    char *fn = (char*)malloc(size);
    
    sprintf(fn, "%s/%s%s", out_data->path, class_name, ".h");
    FILE *f_h = open_file(fn, "w+");
    if(f_h) {
        FILE *head_info = open_file("objective-c_head.txt", "r");
        if(head_info) {
            merge_file(f_h, head_info);
            fclose(head_info);
        }
        fprintf(f_h, "\n%s\n", "#import <Foundation/Foundation.h>");
        out_data->file = f_h;
    } else {
        free(fn);
        return 0;
    }
    
    fn[size-4] = 'm';
    FILE *f_m = open_file(fn, "w+");
    if(f_m) {
        FILE *head_info = open_file("objective-c_head.txt", "r");
        if(head_info) {
            merge_file(f_m, head_info);
            fclose(head_info);
        }
        fprintf(f_m, "\n#import \"%s.h\"\n", class_name);
        fprintf(f_m, "\n@implementation %s\n", class_name);
        fprintf(f_m, "\n@end");
        fflush(f_m);
        fclose(f_m);
    } else {
        free(fn);
        return 0;
    }
    
    strcpy(fn+size-4, "tmp");
    FILE *f_tmp = open_file(fn, "w+");
    if(f_tmp) {
        fprintf(f_tmp, "\n\n@interface %s : NSObject\n\n", class_name);
        out_data->tmp_file = f_tmp;
        out_data->tmp_path = fn;
        return 1;
    } else {
        free(fn);
        return 0;
    }
}

void oc_will_out_property(OUT_RESULT_FUNCS *funcs) {
    //
}

void oc_out_property(OUT_RESULT_FUNCS *funcs, const char* property_name, PropertyType property_type, PropertyType obj_type) {
    out_data *out_data = funcs->data;
    char *property_str = NULL;
    char *modify = "strong";
    char *arr_p = NULL;
    switch (property_type) {
        case PropertyTypeArray: {
            property_str = "NSArray *";
            switch (obj_type) {
                case PropertyTypeArray:
                    //
                    break;
                case PropertyTypeObject:
                    arr_p = malloc(sizeof(char)*(strlen(property_name)+1));
                    strcpy(arr_p, property_name);
                    head_capital(arr_p);
                    tail_wipe_char(arr_p, 's');
                    fprintf(out_data->file, "#import \"%s.h\"\n", arr_p);
                    break;
                case PropertyTypeBool:
                    arr_p = "BOOL";
                    break;
                case PropertyTypeNumber:
                    arr_p = "NSNumber *";
                    break;
                case PropertyTypeString:
                    arr_p = "NSString *";
                    break;
            }
            break;
        }
        case PropertyTypeString:
            property_str = "NSString *";
            modify = "copy";
            break;
        case PropertyTypeObject:
            property_str = malloc(sizeof(char)*(strlen(property_name)+1));
            strcpy(property_str, property_name);
            head_capital(property_str);
            fprintf(out_data->file, "#import \"%s.h\"\n", property_name);
            break;
        case PropertyTypeNumber:
            property_str = "NSNumber *";
            break;
        case PropertyTypeBool:
            property_str = "BOOL";
            break;
    }
    fprintf(out_data->tmp_file, "@property (%s, nonatomic) %s %s;", modify, property_str, property_name);
    if(property_type == PropertyTypeObject) {
        free(property_str);
    } else if(property_type == PropertyTypeArray) {
        fprintf(out_data->tmp_file, " // %s", arr_p);
        if(obj_type == PropertyTypeObject) {
            free(arr_p);
        }
    }
    fputc('\n', out_data->tmp_file);
}

void oc_to_next_property(OUT_RESULT_FUNCS *funcs) {
    //
}

void oc_to_next_class(OUT_RESULT_FUNCS *funcs) {
    out_data *out_data = funcs->data;
    fprintf(out_data->tmp_file, "\n@end");
    
    fflush(out_data->tmp_file);
    fflush(out_data->file);
    
    merge_file(out_data->file, out_data->tmp_file);
    
    fclose(out_data->tmp_file);
    fclose(out_data->file);
    
    remove(out_data->tmp_path);
    free(out_data->tmp_path);
}

void oc_out_end(OUT_RESULT_FUNCS *funcs) {
    // 
}

void out_result_oc(link_m *m, char *path) {
    out_data *out_data_t = malloc(sizeof(struct out_data));
    memset(out_data_t, 0, sizeof(out_data));
    out_data_t->path = path;
    
    out_result_funcs funcs = {oc_ready_out, oc_out_class, oc_will_out_property, oc_out_property, oc_to_next_property, oc_to_next_class, oc_out_end, out_data_t};
    out_result(m, &funcs);
    
    free(out_data_t);
}