/*
 * basic kNN alg
 *
 * Author: niugen
 *
 * GitHub: NiuGenen
 *
 * 2017/12/16
 *
 */

#include "kNN.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef NULL
#define NULL 0
#endif

#define _MALLOC_(t,n) ( (t*)malloc(sizeof(t)*(n)) )
#define _FREE_(p) do{ \
    if( p != NULL ){  \
        free(p);      \
        p = NULL;     \
    }                 \
}while(0);            \

struct knn * knn_alloc()
{
    struct knn * knn = _MALLOC_( struct knn, 1 );
    return knn;
}

void knn_free( struct knn * knn , int free_data )
{
    if( free_data ){
        for( size_t i = 0; i < knn->data_training->size; ++i ){
            _FREE_( knn->data_training->data[i].elem );
        }
        _FREE_( knn->data_training->data );
        _FREE_( knn->data_training->type );
    }
    _FREE_( knn );
}


