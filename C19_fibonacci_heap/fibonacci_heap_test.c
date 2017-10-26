#include "fibonacci_heap.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

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
    // insert / min / extact_min / union
    // decrease_key / delete

    size_t min = 0, max = 0, total = 0;
    read( fd, &min, sizeof(size_t) );
    read( fd, &max, sizeof(size_t) );
    read( fd, &total, sizeof(size_t) );

    printf("[TEST] insert\n");
    printf("total number : %lu\n", total);
    for( size_t i = 0; i < total; ++i ){
        struct fibonacci_heap_node * node = fibonacci_heap_node_alloc();
        node->key = malloc( sizeof( size_t ) );
        read( fd, node->key, sizeof(size_t) );
        printf("insert %lu\n", *((size_t*)(node->key)) );
        fh->insert_node( fh, node );
    }
    fh->pr( fh );

    printf("[TEST] extract_min_node\n");
    for( size_t i = 0; i < total / 10; ++i ){
        struct fibonacci_heap_node * min = fh->extract_min_node( fh );
        printf("extract min : %lu\n", *((size_t*)(min->key)) );
    }
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
    fh->decrease_key_by_node( fh, node, _key );
    fh->pr( fh );

    fibonacci_heap_free( fh );
    return 0;
}
