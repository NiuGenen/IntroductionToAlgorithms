#include "lru_cache.h"
#include <stdio.h>

void kv_pr( void * k, void * v )
{
    printf("%3d,%3d", *((int*)k), *((int*)v) );
}

int key_cmp( void * k1, void * k2 )
{
    int v1 = *((int*)k1);
    int v2 = *((int*)k2);

    if( v1 < v2 ) return -1;
    if( v1 > v2 ) return 1;
    return 0;
}

int main()
{
    struct lru_cache * lru = lru_alloc( 8, key_cmp );

    int key[ 32 ]   = {
        1 ,  2,  3,  4,  5,  6,  7,  8,
        9 ,  1,  2,  5, 11,  6,  8,  0,
        12, 13, 14, 15,  3,  4,  5,  6,
        7 ,  8,  3,  4,  5,  6,  7,  8
    };

    int value[ 32 ];
    for(size_t i = 0; i < 32; ++i) value[i] = key[i] + 100;

    for(size_t i = 0; i < 32; ++i){
        void * v = lru->get( lru, &(key[i]) );
        if( v == NULL ){ // not cache 
            printf("Not Cached : %3d  >> ", value[i] );
            lru->put( lru, &(key[i]), &(value[i]) );
        } else printf("Cached     : %3d  >> ", *((int*)v) );
        lru->pr( lru, kv_pr );
        printf("\n");
    }

    lru_free( lru, 0 );

    return 0;
}
