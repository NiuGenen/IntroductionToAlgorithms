#include "linked_list.h"
#include <stdlib.h>
#include <string.h>

#ifndef NULL 
#define NULL 0
#endif

#define _MALLOC_(type,n) ( (type*)malloc( sizeof(type) * n ) )
#define _FREE_(v) do{ \
    if( v != NULL ) free( v ); \
    v = NULL; \
}while(0);

void add_head_entry( struct llist* ll, struct llist_entry* entry );
void add_tail_entry( struct llist* ll, struct llist_entry* entry );
void rmv_head_entry( struct llist* ll );
void rmv_tail_entry( struct llist* ll );
void for_each_entry( struct llist* ll, llist_for_each_entry_t for_each_func );
struct llist_entry* at_entry( struct llist* ll, size_t index );

void node_add_head( struct llist* ll, struct llist_node* node );
void node_add_tail( struct llist* ll, struct llist_node* node );
void node_rmv( struct llist* ll, struct llist_node* node );

struct llist* llist_alloc()
{
    struct llist* ll = ( struct llist* )malloc( sizeof( struct llist ) );
    
    ll->head   = NULL;
    ll->tail   = NULL;
    ll->length = 0;

    ll->add_head = add_head_entry;
    ll->add_tail = add_tail_entry;
    ll->rmv_head = rmv_head_entry;
    ll->rmv_tail = rmv_tail_entry;
    ll->for_each = for_each_entry;

    ll->node_add_head = node_add_head;
    ll->node_add_tail = node_add_tail;
    ll->node_rmv = node_rmv;

    ll->at = at_entry;

    return ll;
}

void add_head_entry( struct llist* ll, struct llist_entry* entry )
{
    if( ll == NULL ) return;

    struct llist_node* node = _MALLOC_( struct llist_node , 1 );
    node->entry = entry;
    node->prev = NULL;
    node->next = NULL;

    node_add_head( ll, node );

    return;
}

void add_tail_entry( struct llist* ll, struct llist_entry* entry )
{
    if( ll == NULL ) return;

    struct llist_node* node = _MALLOC_( struct llist_node , 1 );
    node->entry = entry;
    node->prev = NULL;
    node->next = NULL;

    node_add_tail( ll, node );

    return;
}

void rmv_head_entry( struct llist* ll )
{
    if( ll == NULL ) return;
    if( ll->length == 0 ) return;

    struct llist_node* node = ll->head;
    node_rmv( ll, node );

    _FREE_( node->entry );
    _FREE_( node );

    return;
}

void rmv_tail_entry( struct llist* ll )
{
    if( ll == NULL ) return;
    if( ll->length == 0 ) return;

    struct llist_node* node = ll->tail;
    node_rmv( ll, node );

    _FREE_( node->entry );
    _FREE_( node );

    return;
}

void for_each_entry( struct llist* ll, llist_for_each_entry_t for_each_func )
{
    if( ll == NULL ) return;
    if( ll->length == 0 ) return;

    struct llist_node* node = ll->head;
    while( node != ll->tail ){
        for_each_func( node->entry );
        node = node->next;
    }
    for_each_func( node->entry );

    return;
}

struct llist_entry* at_entry( struct llist* ll, size_t index )
{   // index start from 0
    if( ll == NULL ) return 0;
    if( ll->length == 0 ) return 0;
    if( ll->length < index ) return 0;

    size_t half = ll->length / 2;

    if( index < half ){
        struct llist_node* node = ll->head;
        while( index ){
            node = node->next;
            --index;
        }
        return node->entry;
    }else{
        struct llist_node* node = ll->tail;
        index = ll->length - index;
        while( index ){
            node = node->prev;
            --index;
        }
        return node->entry;
    }  
}

void node_add_head( struct llist* ll, struct llist_node* node )
{
    if( ll == NULL ) return;

    if( ll->length == 0 ){
        ll->head = node;
        ll->tail = node;
        node->next = node;
        node->prev = node;

        ll->length = 1;
        return;
    }

    struct llist_node* head = ll->head;
    ll->head = node;
    node->next = head;
    node->prev = ll->tail;
    if( head != NULL ) head->prev = node;
    if( ll->tail != NULL ) ll->tail->next = node;

    ll->length += 1;
    return;
}

void node_add_tail( struct llist* ll, struct llist_node* node )
{
    if( ll == NULL ) return;

    if( ll->length == 0 ){
        ll->head = node;
        ll->tail = node;
        node->next = node;
        node->prev = node;

        ll->length = 1;
        return;
    }

    struct llist_node* tail = ll->tail;
    ll->tail = node;
    node->next = ll->head;
    node->prev = tail;
    if( tail != NULL ) tail->next = node;
    if( ll->head != NULL ) ll->head->prev = node;

    ll->length += 1;
    return;
}

void node_rmv( struct llist* ll, struct llist_node* node )
{
    if( ll == NULL ) return;
    if( node == NULL ) return;
    if( ll->length == 0 ) return;

    if( ll->length == 1 ){
        ll->head = NULL;
        ll->tail = NULL;
        ll->length = 0;
        node->next = NULL;
        node->prev = NULL;
        return;
    }

    if( node == ll->head ){
        ll->head = ll->head->next;
        ll->head->prev = ll->tail;
        ll->tail->next = ll->head;
    }else if( node == ll->tail ){
        ll->tail = ll->tail->prev;
        ll->tail->next = ll->head;
        ll->head->prev = ll->tail;
    }else{
        struct llist_node* prev = node->prev;
        struct llist_node* next = node->next;
        prev->next = next;
        next->prev = prev;
    }
    node->next = NULL;
    node->prev = NULL;
    ll->length -= 1;
    return;
}


