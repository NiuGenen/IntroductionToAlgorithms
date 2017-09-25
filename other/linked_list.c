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

    if( ll->length == 0 ){
        ll->head = node;
        ll->tail = node;
        node->next = node;
        node->prev = node;

        ll->length += 1;
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

void add_tail_entry( struct llist* ll, struct llist_entry* entry )
{
    if( ll == NULL ) return;
    if( ll->length == 0 ){
        add_head_entry( ll , entry );
        return;
    }

    struct llist_node* node = _MALLOC_( struct llist_node , 1 );
    node->entry = entry;
    node->prev = NULL;
    node->next = NULL;

    struct llist_node* tail = ll->tail;
    ll->tail = node;
    node->next = ll->head;
    node->prev = tail;
    if( tail != NULL ) tail->next = node;
    if( ll->head != NULL ) ll->head->prev = node;

    ll->length += 1;
    return;
}

void rmv_head_entry( struct llist* ll )
{
    if( ll == NULL ) return;
    if( ll->length == 0 ) return;

    if( ll->length == 1 ){
        struct llist_node* node = ll->head;
        ll->head = NULL;
        ll->tail = NULL;
        ll->length = 0;
        _FREE_( node );
        return;
    }

    struct llist_node* head = ll->head;
    ll->head = head->next;
    ll->head->prev = ll->tail;
    ll->tail->next = ll->head;
    _FREE_( head );

    ll->length -= 1;
    return;
}

void rmv_tail_entry( struct llist* ll )
{
    if( ll == NULL ) return;
    if( ll->length == 0 ) return;

    if( ll->length == 1 ){
        rmv_head_entry( ll );
        return;
    }

    struct llist_node* tail = ll->tail;
    ll->tail = ll->tail->prev;
    ll->tail->next = ll->head;
    ll->head->prev = ll->tail;
    _FREE_( tail );

    ll->length -= 1;
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
