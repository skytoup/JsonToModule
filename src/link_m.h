//
//  link.h
//  Link
//
//  Created by skytoup on 15/6/20.
//  Copyright © 2015年 skytoup. All rights reserved.
//

#ifndef link_c
#define link_c

#include <stdio.h>

/**
 链表返回状态
 */
typedef enum link_status {
    link_fail =0, // 失败
    link_success, // 成功
    link_exist, // 存在
    link_not_exist // 不存在
} link_status;

/**
 *  链表节点
 */
typedef struct link_node link_node;

typedef void(*node_free_func)(link_node *node); // 释放节点值内存函数
typedef int(*node_equal_fuc)(link_node *node_1, link_node *node_2); // 判断两个节点是否相等
typedef int(*node_val_equal_fuc)(void *val_1, void *val_2); // 判断两个节点值是否相等

/**
 *  链表
 */
typedef struct link_m link_m;

link_node* link_node_create(void *val);

void* link_node_get_val(link_node *node);

void link_node_set_val(link_node *node, void *val);

link_node* link_node_get_next(link_node *node);

link_node* link_node_get_pre(link_node *node);


link_m* link_m_create();

void link_m_destory(link_m *m);

void link_m_set_func_free(link_m* m, node_free_func free);

void link_m_set_func_equal(link_m* m, node_equal_fuc node_equal);

void link_m_set_func_val_equal(link_m* m, node_val_equal_fuc val_equal);

link_node* link_m_get_head(link_m *m);

link_node* link_m_get_tail(link_m *m);

link_status link_insert_head(link_m *m, link_node *node);

link_status link_insert_tail(link_m *m, link_node *node);

link_status link_remove_head(link_m *m);

link_status link_remove_tail(link_m *m);

link_status link_remove_node(link_m *m, link_node *node);

link_status link_remove_val(link_m *m, void *val, int reverse);

link_status link_remove_all(link_m *m);

link_status link_replace_head_val(link_m *m, void *val);

link_status link_replace_tail_val(link_m *m, void *val);

link_node* link_search_val(link_m *m, void *val, int reverse);

link_status link_search_node(link_m *m, link_node *node, int reverse);

#endif /* link_c */
