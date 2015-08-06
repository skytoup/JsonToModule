//
//  json_to_module.c
//  SkyJsonToModule
//
//  Created by skytoup on 15/8/1.
//  Copyright (c) 2015年 skytoup. All rights reserved.
//

#include "json_to_module.h"
#include "json_to_module_util.h"
#include <string.h>
#include <stdlib.h>

void out_result(link_m *m, out_result_funcs *funcs) {
    link_node *node = link_m_get_head(m);
    if(funcs && funcs->ready_out) { // 准备输出，只调用一次
        if(!funcs->ready_out(funcs)) {
            node = NULL;
        }
    }
    while(node) {
        object_m *obj = link_node_get_val(node);
        if(obj) {
            const char *name = object_m_get_name(obj);
            if(funcs && funcs->out_class) { // 输出obj名称
                if(!funcs->out_class(funcs, name)) {
                    node = link_node_get_next(node);
                    continue;
                }
            }
            printf("-------------------------\n");
            printf("class : %s\n", name);
            if(funcs && funcs->will_out_property) {
                funcs->will_out_property(funcs); // 准备开始输出属性
            }
            printf("property list :\n");
            const link_m *propertys = object_m_get_property_list(obj);
            link_node *p_node = link_m_get_head((link_m*)propertys);
            while(p_node) {
                if(p_node) {
                    property *p = link_node_get_val(p_node);
                    const char *name = property_node_get_name(p);
                    char *t;
                    PropertyType type = property_node_get_type(p);
                    switch (type) {
                        case PropertyTypeBool:
                            t = "bool";
                            break;
                        case PropertyTypeString:
                            t = "string";
                            break;
                        case PropertyTypeArray:
                            t = "array";
                            break;
                        case PropertyTypeNumber:
                            t = "number";
                            break;
                        case PropertyTypeObject:
                            t = "object";
                            break;
                    }
                    // out property name and type
                    PropertyType obj_type = (type==PropertyTypeArray?property_node_get_arr_obj_type(p):-1);
                    if(funcs && funcs->out_property) {
                        funcs->out_property(funcs, name, type, obj_type); // 输出属性
                    }
                    printf("\t%s \t\t: %s\n", name, t);
                }
                p_node = link_node_get_next(p_node);
                if(funcs && funcs->to_next_property) {
                    funcs->to_next_property(funcs); // 准备下一个属性输出
                }
            }
        }
        printf("-------------------------\n\n\n");
        if(funcs && funcs->to_next_class) {
            funcs->to_next_class(funcs); // 一个类输出完毕，准备到下一个类
        }
        node = link_node_get_next(node);
    }
    if(funcs && funcs->out_end) {
        funcs->out_end(funcs); // 输出完毕
    }
}

/**
 *  处理cJSON的数组
 *
 *  @param objs <#objs description#>
 *  @param json <#json description#>
 *
 *  @return 数组obj类型
 */
PropertyType handle_array(link_m *objs, cJSON *json) {
    cJSON *j = json;
    json = json->child;
    PropertyType type;
    if(json) {
        switch (json->type) {
            case cJSON_True:
            case cJSON_False:
                type = PropertyTypeBool;
                break;
            case cJSON_Number:
                type = PropertyTypeNumber;
                break;
            case cJSON_NULL:
            case cJSON_String:
                type = PropertyTypeString;
                break;
            case cJSON_Array:
                type = PropertyTypeArray;
                printf("waring: array : %s, unsupport array include array\n", j->string);
                break;
            case cJSON_Object:
                type = PropertyTypeObject;
                char *str = (char*)malloc(sizeof(char)*((size_t)strlen(j->string)+1));
                strcpy(str, j->string);
                tail_wipe_char(str, 's');
                handle_object(objs, json, str);
                free(str);
                break;
            default:
                printf("waring: array : %s, unkonw include type\n", j->string);
                break;
        }
    } else {
        type = PropertyTypeString;
    }
    return type;
}

void handle_object(link_m *objs, cJSON *json, char *name) {
    object_m *obj = object_m_create_name(name);
    json = json->child;
    while (json) {
        PropertyType type;
        switch (json->type) {
            case cJSON_True:
            case cJSON_False:
                type = PropertyTypeBool;
                break;
            case cJSON_Number:
                type = PropertyTypeNumber;
                break;
            case cJSON_NULL:
            case cJSON_String:
                type = PropertyTypeString;
                break;
            case cJSON_Array:
                type = PropertyTypeArray;
                PropertyType t = handle_array(objs, json);
                object_m_add_property(obj, json->string, type, t);
                json = json->next;
                continue;
            case cJSON_Object:
                type = PropertyTypeObject;
                handle_object(objs, json, json->string);
                break;
            default:
                printf("waring: class : %s, key : %s, unkonw type\n", object_m_get_name(obj), json->string);
                break;
        }
        object_m_add_property_base(obj, json->string, type);
        json = json->next;
    }
    link_node *node = link_node_create(obj);
    link_insert_head(objs, node);
}
