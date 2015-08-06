//
//  object_m.h
//  SkyJsonToModule
//
//  Created by skytoup on 15/7/31.
//  Copyright (c) 2015年 skytoup. All rights reserved.
//

#ifndef __SkyJsonToModule__object_m__
#define __SkyJsonToModule__object_m__

#include <stdio.h>
#include "property.h"
#include "link_m.h"

typedef struct object_m object_m;

object_m* object_m_create_name(const char *name);
object_m* object_m_create();
void object_m_destory(object_m *obj);

void object_m_set_name(object_m *obj, const char *name);
const char* object_m_get_name(object_m *obj);

const link_m* object_m_get_property_list(object_m *obj);

void object_m_add_property_base(object_m *obj, const char *name, PropertyType type);

void object_m_add_property(object_m *obj, const char *name, PropertyType type, PropertyType obj_type);

void obj_free(link_node *node);

#endif /* defined(__SkyJsonToModule__object_m__) */
