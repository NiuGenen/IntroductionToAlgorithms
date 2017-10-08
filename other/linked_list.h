#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_ 

#include <stddef.h>

typedef void (*llist_for_each_entry_t)( void * entry );

/*
 * linked list 
 */
struct llist_node{
    void * entry;
    struct llist_node *prev;
    struct llist_node *next;
};

struct llist{
    struct llist_node* head;
    struct llist_node* tail;
    size_t length;

    void (*add_head)( struct llist* ll, void * entry );
    void (*add_tail)( struct llist* ll, void * entry );
    void (*rmv_head)( struct llist* ll );
    void (*rmv_tail)( struct llist* ll );
    void * (*at)( struct llist* ll, size_t index );
    void (*for_each)( struct llist* ll, llist_for_each_entry_t for_each_func );

    void (*node_add_head)( struct llist*ll, struct llist_node* node );
    void (*node_add_tail)( struct llist*ll, struct llist_node* node );
    void (*node_rmv)( struct llist*ll, struct llist_node* node );
};

struct llist* llist_alloc();
void llist_free( struct llist* ll );

#endif
