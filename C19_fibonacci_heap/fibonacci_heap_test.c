#include "fibonacci_heap.h"
#include <stdio.h>
#include <stdlib.h>

int key_cmp( void * k1, void * k2 )
{
    int v1 = *((int*)k1);
    int v2 = *((int*)k2);
    if( v1 > v2 ) return  1;
    if( v1 < v2 ) return -1;
    return 0;
}

void * key_min()
{
    int * v = ( int* )malloc( sizeof(int) );
    *v = 0;
    return v;
}

void key_pr( void * key )
{
    printf("%d", *((int*)key));
}

int main()
{
    printf("--- test fibonacci_heap ---\n");

    struct fibonacci_heap * fh = fibonacci_heap_alloc( key_cmp, key_min , key_pr);
    // insert / min / extact_min / union
    // decrease_key / delete

    printf("[TEST] insert\n");
    for( size_t i = 0; i < 32; ++i ){
        struct fibonacci_heap_node * node = fibonacci_heap_node_alloc();
        node->key = malloc( sizeof( int ) );
        *((int*)(node->key)) = i;
        fh->insert_node( fh, node );
        fh->pr( fh );
    }

    printf("[TEST] extract_min_node\n");
    for( size_t i = 0; i < 16; ++i ){
        fh->extract_min_node( fh );
        fh->pr( fh );
    }

    printf("[TEST] decrease key\n");
    int * _key = ( int* )malloc( sizeof(int) );
    *_key = 19;
    struct fibonacci_heap_node * node = fh->search_key( fh, _key );
    *_key = 2;
    fh->decrease_key_by_node( fh, node, _key );
    fh->pr( fh );

    printf("[TEST] delete_node\n");
    int *__key = ( int* )malloc( sizeof(int) );
    *__key = 31;
    struct fibonacci_heap_node * _node = fh->search_key( fh, __key );
    fh->delete_node( fh, _node );
    fh->pr( fh );

    fibonacci_heap_free( fh );
    return 0;
}
