//
//  json_to_module_util.h
//  SkyJsonToModule
//
//  Created by skytoup on 15/8/3.
//  Copyright (c) 2015年 skytoup. All rights reserved.
//

#ifndef __SkyJsonToModule__json_to_module_util__
#define __SkyJsonToModule__json_to_module_util__

#include <stdio.h>
void tail_wipe_char(char *str, char wipe_char);

void head_lower(char *str);
void head_capital(char *str);
int find_char_reverse(const char *str, char c);

FILE* open_file(const char *path, const char *mode);

int merge_file(FILE *file, FILE *merge_file);
int merge_file_path(const char *file_path, const char *merge_file_path);

#endif /* defined(__SkyJsonToModule__json_to_module_util__) */
