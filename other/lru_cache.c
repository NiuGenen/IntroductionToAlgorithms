#include "lru_cache.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef NULL
#define NULL 0
#endif

#define _MALLOC_(t,n) ( (t*)malloc(sizeof(t)*n) )

void lru_cache_put( struct lru_cache * lru, void * key, void * value );

void* lru_cache_get( struct lru_cache * lru, void * key );

void lru_cache_pr( struct lru_cache * lru, void (*lru_cache_KV_pr)(void * k, void * v) );

struct lru_cache * lru_alloc( size_t cache_size , lru_key_cmp_t key_cmp )
{
    struct lru_cache * lru = _MALLOC_( struct lru_cache, 1 );

    lru->cache_size = cache_size;
    lru->size = 0;

    lru->put = lru_cache_put;
    lru->get = lru_cache_get;
    lru->key_cmp = key_cmp;
    lru->pr  = lru_cache_pr;

    lru->head = NULL;
    lru->tail = NULL;

    return lru;
}

void lru_free( struct lru_cache* lru , int if_free_KV )
{
    if( lru == NULL ) return;
    if( lru->size == 0 ) return;

    struct lru_cache_node *node = lru->head, *to_free = lru->head;
    while( node != lru->tail ){
        node = node->next;
        if( if_free_KV ){
            free( to_free->key );
            free( to_free->value );
        }
        free( to_free );
        to_free = node;
    }
    if( if_free_KV ){
        free( to_free->key );
        free( to_free->value );
    }
    free( to_free );

    lru->head = NULL;
    lru->tail = NULL;
    return;
}

// connect & set as head
void lru_node_attach( struct lru_cache* lru, struct lru_cache_node* node )
{
    node->next = lru->head;
    node->prev = lru->tail;
    if( lru->head != NULL ) lru->head->prev = node;
    if( lru->tail != NULL ) lru->tail->next = node;

    lru->head = node;
    if( lru->tail == NULL ) lru->tail = node;
    lru->size += 1;
}

// disconnect
void lru_node_detach( struct lru_cache* lru, struct lru_cache_node* node )
{
    if( lru->size == 1 ){
        lru->size = 0;
        lru->head = NULL;
        lru->tail = NULL;
        return;
    }

    if( node == lru->head ) lru->head = lru->head->next;
    if( node == lru->tail ) lru->tail = lru->tail->prev;
    struct lru_cache_node * prev = node->prev;
    struct lru_cache_node * next = node->next;

    prev->next = node->next;
    next->prev = node->prev;
    node->next = NULL;
    node->prev = NULL;
    lru->size -= 1;
    return;
}

// remove tail
void lru_cache_pop( struct lru_cache * lru )
{
    struct lru_cache_node * tail = lru->tail;
    lru_node_detach( lru, tail );
    //free( tail->key );
    //free( tail->value );
    free( tail );
}

// add head
void lru_cache_push( struct lru_cache * lru, struct lru_cache_node * node )
{
    lru_node_attach( lru, node );
}

void lru_cache_put( struct lru_cache * lru, void * key, void * value)
{
    if( lru->size == lru->cache_size ) lru_cache_pop( lru ); // remove tail

    struct lru_cache_node * node = _MALLOC_( struct lru_cache_node , 1 );
    node->prev = NULL;
    node->next = NULL;
    node->key = key;
    node->value = value;

    lru_cache_push( lru, node ); // add to head
}

void* lru_cache_get( struct lru_cache * lru, void * key)
{
    struct lru_cache_node *node = lru->head, *to_visit = NULL ;

    while( node != NULL && node != lru->tail ){
        if( lru->key_cmp( node->key, key ) == 0 ){
            to_visit = node;
            break;
        }
        node = node->next;
    }
    if( to_visit == NULL && node != NULL){
        if( lru->key_cmp( node->key, key ) == 0 ) to_visit = node;
    }

    if( to_visit != NULL ){
        lru_node_detach( lru, to_visit );
        lru_node_attach( lru, to_visit );
        return to_visit->value;
    }
    return NULL;
}



void lru_cache_pr( struct lru_cache * lru, void (*lru_cache_KV_pr)(void * k, void * v) )
{
    printf("Cached[%3lu] = | " , lru->size );
    if( lru->size == 0 ) return;

    struct lru_cache_node * node = lru->head;
    while( node != lru->tail ){
        lru_cache_KV_pr( node->key, node->value );
        printf(" | ");
        node = node->next;
    }
    lru_cache_KV_pr( node->key, node->value );
    printf(" | ");

    return;
}
