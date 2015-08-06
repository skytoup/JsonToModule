//
//  out_data.h
//  SkyJsonToModule
//
//  Created by skytoup on 15/8/3.
//  Copyright (c) 2015年 skytoup. All rights reserved.
//

#ifndef __SkyJsonToModule__out_data__
#define __SkyJsonToModule__out_data__

#include <stdio.h>

typedef struct out_data {
    FILE *file;
    FILE *tmp_file;
    char *path;
    char *tmp_path;
    void *data;
} out_data;

#endif /* defined(__SkyJsonToModule__out_data__) */
