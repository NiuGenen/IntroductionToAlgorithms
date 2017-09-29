#ifndef _LRU_CACHE_H_
#define _LRU_CACHE_H_ 

#include <stddef.h>

struct lru_cache_node{
    void * key; 
    void * value; 
    struct lru_cache_node * prev;
    struct lru_cache_node * next;
};

typedef int (*lru_key_cmp_t)( void * k1, void * k2 );

struct lru_cache{
    size_t cache_size;
    size_t size;
    struct lru_cache_node * head;
    struct lru_cache_node * tail;
    lru_key_cmp_t key_cmp;

    void (*put)(
        struct lru_cache * lru,
        void * key,
        void * value);
    void* (*get)(
        struct lru_cache * lru,
        void * key);
    void (*pr)(
        struct lru_cache * lru,
        void (*lru_cache_KV_pr)(void * k, void * v) );
};

struct lru_cache * lru_alloc(
    size_t cache_size, lru_key_cmp_t key_cmp);
void lru_free( struct lru_cache* lru , int if_free_KV );

#endif
