//
//  property.h
//  SkyJsonToModule
//
//  Created by skytoup on 15/7/31.
//  Copyright (c) 2015年 skytoup. All rights reserved.
//

#ifndef __SkyJsonToModule__property__
#define __SkyJsonToModule__property__

#include <stdio.h>

typedef enum PropertyType {
    PropertyTypeBool = 0,
    PropertyTypeNumber,
    PropertyTypeString,
    PropertyTypeArray,
    PropertyTypeObject
} PropertyType;

typedef struct property property;

property* property_node_create_name(const char *name);
property* property_node_create();
void property_node_destory(property *p);

void property_node_set_name(property *p, const char *name);
const char* property_node_get_name(property *p);

void property_node_set_type(property *p, PropertyType type);
const PropertyType property_node_get_type(property *p);

void property_node_set_arr_obj_type(property *p, PropertyType arr_obj_type);
PropertyType property_node_get_arr_obj_type(property *p);

#endif /* defined(__SkyJsonToModule__property__) */
