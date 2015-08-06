//
//  json_to_module.h
//  SkyJsonToModule
//
//  Created by skytoup on 15/8/1.
//  Copyright (c) 2015年 skytoup. All rights reserved.
//

#ifndef __SkyJsonToModule__json_to_module__
#define __SkyJsonToModule__json_to_module__

#include <stdio.h>
#include "cJSON.h"
#include "link_m.h"
#include "object_m.h"

#define OUT_RESULT_FUNCS struct out_result_funcs

typedef struct out_result_funcs {
    int (*ready_out)(OUT_RESULT_FUNCS *funcs);
    int (*out_class)(OUT_RESULT_FUNCS *funcs, const char* class_name);
    void (*will_out_property)(OUT_RESULT_FUNCS *funcs);
    void (*out_property)(OUT_RESULT_FUNCS *funcs, const char* property_name, PropertyType property_type, PropertyType obj_type);
    void (*to_next_property)(OUT_RESULT_FUNCS *funcs);
    void (*to_next_class)(OUT_RESULT_FUNCS *funcs);
    void (*out_end)(OUT_RESULT_FUNCS *funcs);
    void *data;
} out_result_funcs;

void handle_object(link_m *objs, cJSON *json, char *name);
void out_result(link_m *m, out_result_funcs *funcs);

#endif /* defined(__SkyJsonToModule__json_to_module__) */
