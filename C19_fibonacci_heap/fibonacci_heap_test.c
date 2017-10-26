#include "fibonacci_heap.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../util/ask_time.h"

#define _MALLOC_(t,n) ( (t*)malloc(sizeof(t)*(n)) )
#define _FREE_(p) do{ \
    if( p != NULL ){ \
        free(p); \
        p = NULL; \
    } \
}while(0);

int key_cmp( void * k1, void * k2 )
{
    size_t v1 = *((size_t*)k1);
    size_t v2 = *((size_t*)k2);
    if( v1 > v2 ) return  1;
    if( v1 < v2 ) return -1;
    return 0;
}

void * key_min()
{
    size_t * v = ( size_t* )malloc( sizeof(size_t) );
    *v = 0;
    return v;
}

void key_pr( void * key )
{
    printf("%lu", *((size_t*)key));
}

int main( int argc, char * argv[] )
{
    printf("--- test fibonacci_heap ---\n");

    if( argc < 2 ){
        printf("[ERROR] please input data file\n");
        return -1;
    }

    const char * filename = argv[1];
    printf("input file : %s\n", filename );
    int fd = open( filename, O_RDONLY, 0666 );
    if( fd < 0 ){
        printf("[ERROR] fail to open input file\n");
        return -2;
    }

    struct fibonacci_heap * fh = fibonacci_heap_alloc( key_cmp, key_min , key_pr);

    size_t min = 0, max = 0, total = 0;
    read( fd, &min, sizeof(size_t) );
    read( fd, &max, sizeof(size_t) );
    read( fd, &total, sizeof(size_t) );

    time_asker_t asker = asker_alloc( ASKER_TYPE_CLOCK );

    printf("[TEST] insert\n");
    printf("total number : %lu\n", total);
    struct fibonacci_heap_node ** nodes = _MALLOC_( struct fibonacci_heap_node *, total );
    for( size_t i = 0; i < total; ++i ){
        nodes[i] = fibonacci_heap_node_alloc();
        nodes[i]->key = malloc( sizeof( size_t ) );
        read( fd, nodes[i]->key, sizeof(size_t) );
    }
    start_ask_time( asker );
    for( size_t i = 0; i < total; ++i ){
        printf("insert %lu\n", *((size_t*)(nodes[i]->key)) );
        fh->insert_node( fh, nodes[i] );
    }
    end_ask_time( asker );
    ask_time( asker );
    reset_asker( asker );
    fh->pr( fh );

    printf("[TEST] extract_min_node\n");
    start_ask_time( asker );
    for( size_t i = 0; i < total / 10; ++i ){
        struct fibonacci_heap_node * min = fh->extract_min_node( fh );
        printf("extract min : %lu\n", *((size_t*)(min->key)) );
    }
    end_ask_time( asker );
    ask_time( asker );
    reset_asker( asker );
    fh->pr( fh );

    printf("[TEST] decrease key\n");
    srand( (unsigned int)time(0) );
    size_t * _key = ( size_t* )malloc( sizeof(size_t) );
    *_key = rand()%total + min;
    struct fibonacci_heap_node * node = fh->search_key( fh, _key );
    while( node == NULL ) {
        *_key = rand()%total + min;
        node = fh->search_key( fh, _key );
    }
    *_key = 0;
    printf("decrease %lu to 0\n", *((size_t*)(node->key)));
    start_ask_time( asker );
    fh->decrease_key_by_node( fh, node, _key );
    end_ask_time( asker );
    ask_time( asker );
    reset_asker( asker );
    fh->pr( fh );
    // return
    asker_free( asker );
    fibonacci_heap_free( fh );
    return 0;
}
