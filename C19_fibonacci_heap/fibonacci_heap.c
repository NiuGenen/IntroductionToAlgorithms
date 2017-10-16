#include "fibonacci_heap.h"
#include <stdlib.h>

void fibonacci_heap_insert_node( struct fibonacci_heap * fh, struct fibonacci_heap_node * node );
struct fibonacci_heap_node * fibonacci_heap_get_min_node( struct fibonacci_heap * fh );
void fibonacci_heap_decrease_key_by_node( struct fibonacci_heap * fh, struct fibonacci_heap_node * node, void * key );
struct fibonacci_heap_node * fibonacci_heap_search_key( struct fibonacci_heap * fh, void * key );
void  fibonacci_heap_delete_node( struct fibonacci_heap * fh, struct fibonacci_heap_node * node );
struct fibonacci_heap_node * fibonacci_heap_exact_min_node( struct fibonacci_heap * fh );

// insert node into list_node->right
void fibonacci_heap_node_list_insert( struct fibonacci_heap_node * list_node, struct fibonacci_heap_node * node )
{
    node->right       = list_node->right;
    list_node->right  = node;
    node->left        = list_node;
    node->right->left = node;
}

//  remove node from list , return node->right
struct fibonacci_heap_node * fibonacci_heap_node_list_remove( struct fibonacci_heap_node * node )
{
    if( node->left == node ) return NULL;
    struct fibonacci_heap_node * right = node->right;
    struct fibonacci_heap_node * left  = node->left ;
    left->right = right;
    right->left = left;
    return right;
}

#define _MALLOC_(t,n) ( (t*)malloc(sizeof(t)*(n)) )
#define _FREE_(p) do{ \
    if( p != NULL ){ \
        free(p); \
        p = NULL; \
    } \
}while(0);

struct fibonacci_heap * fibonacci_heap_alloc( fibonacci_key_cmp_t key_cmp, fibonacci_key_min_t key_min   )
{
    struct fibonacci_heap * fh = _MALLOC_( struct fibonacci_heap , 1 );
    fh->n = 0;
    fh->min = 0;
    fh->key_cmp = key_cmp;
    fh->key_min = key_min;
    // operations
    fh->insert_node          = fibonacci_heap_insert_node;
    fh->delete_node          = fibonacci_heap_delete_node;
    fh->get_min_node         = fibonacci_heap_get_min_node;
    fh->exact_min_node       = fibonacci_heap_exact_min_node;
    fh->decrease_key_by_node = fibonacci_heap_decrease_key_by_node;
    // return
    return fh;
}

void fibonacci_heap_free_node_list( struct fibonacci_heap_node * node )
{
    if( node == NULL ) return;
    struct fibonacci_heap_node * _node = node->right;
    while( _node != node ){
        if( node->degree > 0 ) fibonacci_heap_free_node_list( node->child );
        _node = _node->right;
        _FREE_( _node->left );
    }
    if( node->degree > 0 ) fibonacci_heap_free_node_list( node->child );
    _FREE_( node );
}

void fibonacci_heap_free( struct fibonacci_heap * fh )
{
    if( fh == NULL ) return;
    fibonacci_heap_free_node_list( fh->min );
    _FREE_( fh );
}

struct fibonacci_heap * fibonacci_heap_union( struct fibonacci_heap * fh_1, struct fibonacci_heap * fh_2 )
{
    // alloc new heap
    struct fibonacci_heap * fh = NULL;
    // deal with NULL
    if( fh_1 == NULL || fh_2 == NULL ){
        if( fh_1 == NULL ) fh_1 = fh_2;
        if( fh_1 != NULL ){
            fh = fibonacci_heap_alloc( fh_1->key_cmp , fh_1->key_min );
            fh->n   = fh_1->n;
            fh->min = fh_1->min;
        }
        _FREE_( fh_1 );
        _FREE_( fh_2 );
        return fh;
    }
    // deal with 0
    if( fh_1->n == 0 || fh_2->n == 0 )
    {
        fh = fibonacci_heap_alloc( fh_1->key_cmp , fh_1->key_min );
        if( fh_1->n == 0 ) { _FREE_(fh_1); fh_1 = fh_2; }
        if( fh_1->n != 0 ){
            fh->n   = fh_1->n;
            fh->min = fh_1->min;
        }
        _FREE_( fh_1 );
        _FREE_( fh_2 );
        return fh;
    }
    // set n
    fh->n = fh_1->n + fh_2->n;
    // set min
    fh->min = fh_1->min;
    if( fh->key_cmp( fh_1->min, fh_2->min ) > 0 ) fh->min = fh_2->min;
    // link two list
    struct fibonacci_heap_node * node1 = fh_1->min;
    struct fibonacci_heap_node * node2 = fh_2->min;
    struct fibonacci_heap_node * node1_left = node1->left;
    struct fibonacci_heap_node * node2_left = node2->left;
    node1->left = node2_left;
    node2_left->right = node1;
    node2->left = node1_left;
    node1_left->right = node2;
    _FREE_( fh_1 );
    _FREE_( fh_2 );
    return fh;
}

struct fibonacci_heap_node * fibonacci_heap_node_alloc()
{
    struct fibonacci_heap_node * node = _MALLOC_( struct fibonacci_heap_node , 1 );
    node->if_lost_child = 0;
    node->degree        = 0;
    node->parent = NULL;
    node->left   = NULL;
    node->right  = NULL;
    node->child  = NULL;
    node->key    = NULL;
    return node;
}

void fibonacci_heap_insert_node( struct fibonacci_heap * fh, struct fibonacci_heap_node * node )
{
    if( fh == NULL || node == NULL ) return;
    node->parent = NULL;
    node->child  = NULL;
    node->degree = 0;
    if( fh->n == 0 ){
        fh->n = 1;
        fh->min = node;
        node->left  = node;
        node->right = node;
        return ;
    }
    fibonacci_heap_node_list_insert( fh->min, node );
    if( fh->key_cmp( node->key, fh->min->key ) < 0 ) fh->min = node;
    fh->n += 1;
}

struct fibonacci_heap_node * fibonacci_heap_get_min_node( struct fibonacci_heap * fh )
{
    if( fh == NULL ) return NULL;
    return fh->min;
}

struct fibonacci_heap_node * fibonacci_heap_exact_min_node( struct fibonacci_heap * fh )
{
    if( fh == NULL ) return NULL;
    struct fibonacci_heap_node * min_node = fh->min;
    // exact
    return min_node;
}

void fibonacci_heap_decrease_key_by_node( struct fibonacci_heap * fh, struct fibonacci_heap_node * node, void * key )
{
    if( fh == NULL || node == NULL || key == NULL ) return;
}

void  fibonacci_heap_delete_node( struct fibonacci_heap * fh, struct fibonacci_heap_node * node )
{
    if( fh == NULL || node == NULL ) return;
}

struct fibonacci_heap_node * fibonacci_heap_node_list_search(
        struct fibonacci_heap * fh,
        struct fibonacci_heap_node * node,
        void * key )
{
    if( node == NULL ) return NULL;
    struct fibonacci_heap_node * _node = node->right;
    // search node->right ....
    while( _node != node ){
        if( _node->degree > 0 ){ // search _node's child
            struct fibonacci_heap_node * ret = fibonacci_heap_node_list_search( fh, _node->child, key );
            if( ret != NULL ) return ret;
        }
        if( fh->key_cmp( _node->key, key ) == 0 ) // search _node self
            return _node;
        _node = _node->right;
    }
    // search node : _node == node
    if( _node->degree > 0 ){
        struct fibonacci_heap_node * ret = fibonacci_heap_node_list_search( fh, _node->child, key );
        if( ret != NULL ) return ret;
    }
    if( fh->key_cmp( _node->key, key ) == 0 ) return _node;
    return NULL;
}

struct fibonacci_heap_node * fibonacci_heap_search_key( struct fibonacci_heap * fh, void * key )
{
    if( fh == NULL || key == NULL ) return NULL;
    return fibonacci_heap_node_list_search( fh, fh->min , key );
}
