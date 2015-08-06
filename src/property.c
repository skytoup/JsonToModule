//
//  property.c
//  SkyJsonToModule
//
//  Created by skytoup on 15/7/31.
//  Copyright (c) 2015年 skytoup. All rights reserved.
//

#include "property.h"
#include <stdlib.h>
#include <string.h>

typedef struct property {
    char *name; 
    PropertyType type;
    PropertyType arr_obj_type;
} property;

property* property_node_create_name(const char *name) {
    property *p = malloc(sizeof(property));
    memset(p, 0, sizeof(property));
    property_node_set_name(p, name);
    return p;
}

property* property_node_create() {
    return property_node_create_name(NULL);
}

void property_node_destory(property *p) {
    char *name = p->name;
    if(name) {
        free(name);
    }
    free(p);
}

void property_node_set_name(property *p, const char *name) {
    if(name) {
        char *n = malloc(sizeof(char)*(strlen(name)+1));
        strcpy(n, name);
        p->name = n;
    } else if(p->name) {
        free(p->name);
    }
}

const char* property_node_get_name(property *p) {
    return p->name;
}

void property_node_set_type(property *p, PropertyType type) {
    p->type = type;
}

const PropertyType property_node_get_type(property *p) {
    return p->type;
}

void property_node_set_arr_obj_type(property *p, PropertyType arr_obj_type) {
    p->arr_obj_type = arr_obj_type;
}

PropertyType property_node_get_arr_obj_type(property *p) {
    return p->arr_obj_type;
}
