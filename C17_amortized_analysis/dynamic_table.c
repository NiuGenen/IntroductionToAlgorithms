#include "dynamic_table.h"
#include <stdlib.h>

#ifndef NULL
#define NULL 0
#endif

#define _MALLOC_(t,n) ( (t*)malloc(sizeof(t)*(n)) )
#define _FREE_(p) do{ \
    if( p != NULL ){  \
        free(p);      \
        p = NULL;     \
    }                 \
}while(0);

void dnmc_put( struct dynamic_table * dnmc , void * value )
{
    if( dnmc == NULL ) return;
    if( dnmc->length == dnmc->size ){
        struct dynamic_table_entry * _entry = _MALLOC_( struct dynamic_table_entry, dnmc->size * 2 );
        for( size_t i = 0; i < dnmc->size; ++i ){
            _entry[i].value = dnmc->entry[i].value;
        }
        _FREE_( dnmc->entry );
        dnmc->entry = _entry;
        dnmc->size *= 2;
    }
    dnmc->entry[ dnmc->length ].value = value;
    dnmc->length += 1;
}

void *dnmc_at(  struct dynamic_table * dnmc , size_t index )
{
    if( dnmc == NULL ) return NULL;
    if( index >= dnmc->length ) return NULL;
    return dnmc->entry[ index ].value;
}

void *dnmc_get( struct dynamic_table * dnmc , size_t index )
{
    if( dnmc == NULL ) return NULL;
    if( index >= dnmc->length ) return NULL;
    void * ret = dnmc->entry[ index ].value;
    for( size_t i = index; i < dnmc->length - 1; ++i ){
        dnmc->entry[ i ].value = dnmc->entry[ i+1 ].value;
    }
    dnmc->length -= 1;
    if( dnmc->length <= (dnmc->size / 4) ){
        struct dynamic_table_entry * _entry = _MALLOC_( struct dynamic_table_entry , dnmc->size / 2 );
        for( size_t i = 0; i < dnmc->length; ++i ){
            _entry[i].value = dnmc->entry[i].value;
        }
        _FREE_( dnmc->entry );
        dnmc->entry = _entry;
    }
    return ret;
}

struct dynamic_table * dnmc_table_alloc( size_t size )
{
    struct dynamic_table * dnmc = _MALLOC_( struct dynamic_table, 1 );
    dnmc->size   = size;
    dnmc->entry  = _MALLOC_( struct dynamic_table_entry, size );
    dnmc->length = 0;
    // operations
    dnmc->put = dnmc_put ;
    dnmc->get = dnmc_get ;
    dnmc->at  = dnmc_at  ;
    return dnmc;
}

void dnmc_table_free( struct dynamic_table * dnmc )
{
    if( dnmc == NULL ) return;
    _FREE_( dnmc->entry );
    _FREE_( dnmc );
}


