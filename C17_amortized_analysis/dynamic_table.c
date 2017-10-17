#include "dynamic_table.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef  DYNAMIC_COUNTER_ENABLED 
size_t __op  = 0;
size_t __put = 0;
size_t __get = 0;
size_t __mov = 0;
#endif

void __dynamic_counter_pr()
{
#ifdef  DYNAMIC_COUNTER_ENABLED 
    __op = __put + __get + __mov;
    size_t __avg = __op / ( __get + __put );
    printf("OPs = %5lu , PUTs = %5lu , GETs = %5lu , MOVs = %5lu, OP_AVGs = %5lu\n", \
            __op, __put, __get, __mov , __avg);
#endif
}

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
#ifdef DYNAMIC_COUNTER_ENABLED 
    __mov += 1;
#endif
        }
        _FREE_( dnmc->entry );
        dnmc->entry = _entry;
        dnmc->size *= 2;
    }
    dnmc->entry[ dnmc->length ].value = value;
#ifdef DYNAMIC_COUNTER_ENABLED 
    __put += 1;
#endif
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
#ifdef DYNAMIC_COUNTER_ENABLED 
    __get += 1;
#endif
    for( size_t i = index; i < dnmc->length - 1; ++i ){
        dnmc->entry[ i ].value = dnmc->entry[ i+1 ].value;
#ifdef DYNAMIC_COUNTER_ENABLED 
    __mov += 1;
#endif
    }
    dnmc->length -= 1;
    if( dnmc->length <= (dnmc->size / 4) ){
        struct dynamic_table_entry * _entry = _MALLOC_( struct dynamic_table_entry , dnmc->size / 2 );
        for( size_t i = 0; i < dnmc->length; ++i ){
            _entry[i].value = dnmc->entry[i].value;
#ifdef DYNAMIC_COUNTER_ENABLED 
    __mov += 1;
#endif
        }
        _FREE_( dnmc->entry );
        dnmc->entry = _entry;
        dnmc->size = dnmc->size / 2;
    }
    return ret;
}

void *dnmc_pop( struct dynamic_table * dnmc )
{
    if( dnmc == NULL ) return NULL;
    if( dnmc->length == 0 ) return NULL;
    return dnmc_get( dnmc, dnmc->length - 1 );
}

void dnmc_push( struct dynamic_table * dnmc , void * value )
{
    dnmc_put( dnmc, value );
}

void dnmc_pr( struct dynamic_table * dnmc )
{
    printf("--- dynamic table ---\n");
    if( dnmc == NULL ){
        printf("[WARING] NULL table pointer\n");
        goto DNMC_PR_OUT;
    }
    printf("Size   = %5lu\n", dnmc->size );
    printf("Length = %5lu\n", dnmc->length);
    if( dnmc->length == 0 ){
        printf("Empry dynamic table\n");
        goto DNMC_PR_OUT;
    }
    for( size_t i = 0; i < dnmc->length; ++i ){
        printf("Value[%3lu] = ", i );
        dnmc->value_pr( dnmc->entry[i].value );
        printf("\n");
    }
DNMC_PR_OUT:
    printf("---------------------\n");
}

struct dynamic_table * dnmc_table_alloc( size_t size , dynamic_entry_value_pr_t value_pr )
{
    struct dynamic_table * dnmc = _MALLOC_( struct dynamic_table, 1 );
    dnmc->size   = size;
    dnmc->entry  = _MALLOC_( struct dynamic_table_entry, size );
    dnmc->length = 0;
    dnmc->value_pr = value_pr;
    // operations
    dnmc->put  = dnmc_put ;
    dnmc->get  = dnmc_get ;
    dnmc->at   = dnmc_at  ;
    dnmc->pop  = dnmc_pop ;
    dnmc->push = dnmc_push;
    dnmc->pr   = dnmc_pr;
    return dnmc;
}

void dnmc_table_free( struct dynamic_table * dnmc )
{
    if( dnmc == NULL ) return;
    _FREE_( dnmc->entry );
    _FREE_( dnmc );
}


