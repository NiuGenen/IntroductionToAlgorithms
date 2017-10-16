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

int main()
{
    printf("--- test fibonacci_heap ---\n");

    struct fibonacci_heap * fh = fibonacci_heap_alloc( key_cmp, key_min );

    fibonacci_heap_free( fh );

    return 0;
}
