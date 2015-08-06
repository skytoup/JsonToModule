//
//  link.c
//  Link
//
//  Created by skytoup on 15/6/20.
//  Copyright © 2015年 skytoup. All rights reserved.
//

#include "link_m.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct link_node {
    struct link_node *next; // 前一个节点
    struct link_node *pre; // 后一个节点
    void *val; // 节点值
//    void *data; // 附加值
};

struct link_m {
    link_node *head; // 链表头
    link_node *tail; // 链表尾
    long count; // 链表数量
    node_free_func node_free_fuc; // 释放节点值内存函数
    node_equal_fuc node_equal_func; // 判断两个节点是否相等
    node_val_equal_fuc node_val_equal_func; // 判断两个节点值是否相等
};

/*** link_node ***/

link_node* link_node_create(void *val) {
    link_node *node = (link_node*)malloc(sizeof(link_node));
    memset(node, 0, sizeof(link_node));
    node->val = val;
    return node;
};

void* link_node_get_val(link_node *node) {
    return node?node->val:NULL;
};

void link_node_set_val(link_node *node, void *val) {
    if(node) {
        node->val = val;
    }
}

link_node* link_node_get_next(link_node *node) {
    return node?node->next:NULL;
};

link_node* link_node_get_pre(link_node *node) {
    return node?node->pre:NULL;
};


link_m* link_m_create() {
    link_m *m = (link_m*)malloc(sizeof(link_m));
    memset(m, 0, sizeof(link_m));
    return m;
};

void link_m_destory(link_m *m) {
    link_remove_all(m);
    free(m);
}

void link_m_set_func(link_m* m, node_free_func free, node_equal_fuc equal) {
    if(m) {
        m->node_free_fuc = free;
        m->node_equal_func = equal;
    }
};

void link_m_set_func_free(link_m* m, node_free_func free) {
    if(m) {
        m->node_free_fuc = free;
    }
};

void link_m_set_func_equal(link_m* m, node_equal_fuc node_equal) {
    if(m) {
        m->node_equal_func = node_equal;
    }
};

void link_m_set_func_val_equal(link_m* m, node_val_equal_fuc val_equal) {
    if(m) {
        m->node_val_equal_func = val_equal;
    }
};

link_node* link_m_get_head(link_m *m) {
    return m->head;
}

link_node* link_m_get_tail(link_m *m) {
    return m->tail;
}

link_status link_insert_head(link_m *m, link_node *node) {
    if(m) {
        if(m->head) {
            node->next = m->head;
            node->pre = NULL;
            m->head->pre = node;
            m->head = node;
        } else {
            m->head = m->tail = node;
        }
        m->count++;
        return link_success;
    } else {
        return link_fail;
    }
};

link_status link_insert_tail(link_m *m, link_node *node) {
    if(m) {
        if(m->tail) {
            node->pre = m->tail;
            node->next = NULL;
            m->tail->next = node;
            m->tail = node;
        } else {
            m->head = m->tail = node;
        }
        m->count++;
        return link_success;
    } else {
        return link_fail;
    }
};

link_status link_remove_head(link_m *m) {
    if(m) {
        if(m->head) {
            link_node *head = m->head;
            if(m->head == m->tail) {
                m->head = m->tail = NULL;
            } else {
                m->head = m->head->next;
                m->head->pre = NULL;
            }
            m->count--;
            if(m->node_free_fuc) {
                m->node_free_fuc(head);
                free(head);
            }
        }
        return link_success;
    } else {
        return link_fail;
    }
};

link_status link_remove_tail(link_m *m){
    if(m) {
        if(m->tail) {
            link_node *tail = m->tail;
            if(m->head == m->tail) {
                m->head = m->tail = NULL;
            } else {
                m->tail = m->tail->pre;
                m->tail->next = NULL;
            }
            m->count--;
            if(m->node_free_fuc) {
                m->node_free_fuc(tail);
                free(tail);
            }
        }
        return link_success;
    } else {
        return link_fail;
    }
};

link_status link_remove_node(link_m *m, link_node *node) {
    if(m && link_search_node(m, node, 0) == link_exist ) {
        if(!(node->pre)) {
            link_remove_head(m);
        } else if(!(node->next)) {
            link_remove_tail(m);
        } else {
            link_node *pre = node->pre;
            link_node *next = node->next;
            pre->next = next;
            next->pre = pre;
        }
        m->count --;
        if(m->node_free_fuc) {
            m->node_free_fuc(node);
            free(node);
        }
        return link_success;
    } else {
        return link_fail;
    }
};

link_status link_remove_val(link_m *m, void *val, int reverse) {
    if(m) {
        link_node *node = link_search_val(m, val, reverse);
        if(node) {
            link_remove_node(m, node);
            return link_success;
        } else {
            return link_not_exist;
        }
    } else {
        return link_fail;
    }
};

link_status link_remove_all(link_m *m) {
    if(m) {
        link_node *node = m->head;
        if(m->node_free_fuc) {
            while(node) {
                m->node_free_fuc(node);
                free(node);
                node = node->next;
            }
        } else {
            while(node) {
                free(node);
                node = node->next;
            }
        }
        m->head = m->tail = NULL;
        m->count = 0;
        return link_success;
    } else {
        return link_fail;
    }
}

link_status link_replace_head_val(link_m *m, void *val) {
    if(m) {
        if(m->node_free_fuc) {
            m->node_free_fuc(m->head->val);
        }
        m->head->val = val;
        return link_success;
    } else {
        return link_fail;
    }
};

link_status link_replace_tail_val(link_m *m, void *val) {
    if(m) {
        if(m->node_free_fuc) {
            m->node_free_fuc(m->tail->val);
        }
        m->tail->val = val;
        return link_success;
    } else {
        return link_fail;
    }
}

link_node* link_search_val(link_m *m, void *val, int reverse) {
    if(!m) {
        return NULL;
    }
    link_node *node = NULL;
    if(reverse) {
        node =  m->tail;
        do {
            if(m->node_val_equal_func) {
                if(m->node_val_equal_func(node->val, val) ) {
                    break;
                }
            }
            node = node->pre;
        } while(node);
    } else {
        node = m->head;
        do {
            if(m->node_val_equal_func) {
                if(m->node_val_equal_func(node->val, val) ) {
                    break;
                }
            }
            node = node->next;
        } while(node);
    }
    return node;
};

link_status link_search_node(link_m *m, link_node *node, int reverse) {
    if(!m) {
        return link_not_exist;
    }
    link_node *n = NULL;
    if(reverse) {
        n =  m->tail;
        do {
            if(m->node_equal_func) {
                if(m->node_equal_func(node, n) ) {
                    break;
                }
            }
            n = n->pre;
        } while(n);
    } else {
        n = m->head;
        do {
            if(m->node_equal_func) {
                if(m->node_equal_func(node, n) ) {
                    break;
                }
            }
            n = n->next;
        } while(n);
    }
    return n?link_exist:link_not_exist;
};
