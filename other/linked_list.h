#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_ 

#include <stddef.h>

struct llist_entry;

typedef void (*llist_for_each_entry_t)( struct llist_entry* entry );

/*
 * linked list 
 */
struct llist_node{
    struct llist_entry* entry;
    struct llist_node *prev;
    struct llist_node *next;
};

struct llist{
    struct llist_node* head;
    struct llist_node* tail;
    size_t length;

    void (*add_head)( struct llist* ll, struct llist_entry* entry );
    void (*add_tail)( struct llist* ll, struct llist_entry* entry );
    void (*rmv_head)( struct llist* ll );
    void (*rmv_tail)( struct llist* ll );
    struct llist_entry* (*at)( struct llist* ll, size_t index );
    void (*for_each)( struct llist* ll, llist_for_each_entry_t for_each_func );

    void (*node_add_head)( struct llist*ll, struct llist_node* node );
    void (*node_add_tail)( struct llist*ll, struct llist_node* node );
    void (*node_rmv)( struct llist*ll, struct llist_node* node );
};

struct llist* llist_alloc();
void llist_free( struct llist* ll );

#endif
