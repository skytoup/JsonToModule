//
//  object_m.c
//  SkyJsonToModule
//
//  Created by skytoup on 15/7/31.
//  Copyright (c) 2015年 skytoup. All rights reserved.
//

#include "object_m.h"
#include <stdlib.h>
#include <string.h>

typedef struct object_m {
    char *name;
    link_m *property_m;
} object_m;

void property_node_free(link_node *node) {
    void *p = link_node_get_val(node);
    property_node_destory(p);
}

object_m* object_m_create_name(const char *name) {
    object_m *o = malloc(sizeof(object_m));
    memset(o, 0, sizeof(object_m));
    o->property_m = link_m_create();
    link_m_set_func_free(o->property_m, property_node_free);
    object_m_set_name(o, name);
    return o;
}

object_m* object_m_create() {
    return object_m_create_name(NULL);
}

void object_m_destory(object_m *obj) {
    if(obj->property_m) {
        link_m_destory(obj->property_m);
    }
    if(obj->name) {
        free(obj->name);
    }
    free(obj);
}

void object_m_set_name(object_m *obj, const char *name) {
    if(name) {
        char *n = malloc(sizeof(char)*(strlen(name)+1));
        strcpy(n, name);
        obj->name = n;
    } else if(obj->name) {
        free(obj->name);
    }
}

const char* object_m_get_name(object_m *obj) {
    return obj->name;
}

const link_m* object_m_get_property_list(object_m *obj) {
    return obj->property_m;
}

void object_m_add_property_base(object_m *obj, const char *name, PropertyType type) {
    object_m_add_property(obj, name, type, -1);
}

void object_m_add_property(object_m *obj, const char *name, PropertyType type, PropertyType obj_type) {
    property *p = property_node_create_name(name);
    property_node_set_type(p, type);
    property_node_set_arr_obj_type(p, obj_type);
    link_node *node = link_node_create(p);
    link_insert_tail(obj->property_m, node);
}


void obj_free(link_node *node) {
    void *obj = link_node_get_val(node);
    if(obj) {
        object_m_destory(obj);
    }
}