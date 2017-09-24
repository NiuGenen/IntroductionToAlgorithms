#include "bloom_filter.h"
#include <stdio.h>
#include <stdlib.h>

void binary_pr( void * value, size_t n );

void blm_add(
        struct bloom_filter* blm,
        struct bloom_filter_entry* entry);
int blm_not_exist(
        struct bloom_filter* blm,
        struct bloom_filter_entry* entry);
void blm_pr(
        struct bloom_filter* blm);

/*
 * alloc bloom filter
 */
struct bloom_filter* blm_alloc(
        size_t bits,
        hash_fn_t* hash_func_s,
        size_t hash_fn_nr,
        blm_hash_t blm_hash
#ifdef BLM_DBG
        ,blm_entry_pr_t entry_pr
#endif
        )
{
    struct bloom_filter* blm = ( struct bloom_filter* )malloc( sizeof( struct bloom_filter ) );

    blm->bits  = bits;
    blm->bb_nr = ( bits >> 6 ) + ( ( bits & 0x003F ) ? 1 : 0 );
    blm->bb    = ( uint64_t* )malloc( sizeof( uint64_t ) * blm->bb_nr );

    blm->hash_func_s  = hash_func_s;
    blm->hash_func_nr = hash_fn_nr;
#ifdef BLM_DBG
    blm->entry_pr = entry_pr;
#endif
    blm->blm_hash = blm_hash;

    blm->add       = blm_add;
    blm->not_exist = blm_not_exist;
    blm->pr        = blm_pr;

    return blm;
}

void blm_free(
        struct bloom_filter* blm )
{
    if( blm == NULL ) return;

    free( blm->bb );
    free( blm );
}

void blm_add(
        struct bloom_filter* blm,
        struct bloom_filter_entry* entry)
{
    if( blm == NULL ) return;
    if( entry == NULL ) return;

#ifdef BLM_DBG
    printf("\nAdd New Entry\n");
    blm->entry_pr( entry );
#endif

    for(size_t i = 0; i< blm->hash_func_nr; ++i){
        hash_func_ret_t hash = blm->blm_hash( entry , blm->hash_func_s[i] );
        hash = hash % blm->bits;
#ifdef BLM_DBG
        printf("hash  = ");
        binary_pr( &(hash), 64  );
        printf("   %lu\n", hash );
#endif
        // [0,63][64,127]...
        size_t bb_idx    = hash >> 6 ;
        size_t bb_offset = hash % 64 ;
        uint64_t asd = 1;
        asd = asd << bb_offset;
        blm->bb[ bb_idx ] ^= asd;
#ifdef BLM_DBG
        printf("asd   = ");
        binary_pr( &(asd), 64 );
        printf("\n");
        printf("bb[%lu] = ", bb_idx);
        binary_pr( &(blm->bb[ bb_idx ]), 64 );
        printf("\n");
#endif
    }
}

int blm_not_exist(
        struct bloom_filter* blm,
        struct bloom_filter_entry* entry)
{
    if( blm == NULL ) return 0;
    if( entry == NULL ) return 0;

#ifdef BLM_DBG
    printf("\nCheck Entry\n");
    blm->entry_pr( entry );
#endif

    for(size_t i = 0; i< blm->hash_func_nr; ++i){
        hash_func_ret_t hash = blm->blm_hash( entry , blm->hash_func_s[i] );
        hash = hash % blm->bits;

#ifdef BLM_DBG
        printf("hash  = ");
        binary_pr( &(hash), 64  );
        printf("   %lu\n", hash );
#endif

        // [0,63][64,127]...
        size_t bb_idx    = hash >> 6;
        size_t bb_offset = hash & ~0x003F;
        uint64_t asd = 1;
        asd = asd << bb_offset;

#ifdef BLM_DBG
        printf("asd   = ");
        binary_pr( &(asd), 64 );
        printf("\n");
        printf("bb[%lu] = ", bb_idx);
        binary_pr( &(blm->bb[ bb_idx ]), 64 );
        printf("\n");
#endif
        //blm->bb[ bb_idx ] &= asd;
        if( !(blm->bb[ bb_idx ] & asd) ){
#ifdef BLM_DBG
            printf("Entry Not Exist!\n");
#endif
            return 1;
        }
    }

    return 0;
}

void binary_pr( void * value, size_t n )
{
    size_t v = *((size_t*)value);
    char * o = (char* )malloc( n + 1 );
    o[ n ] = '\0';
    for(size_t i = 0; i < n; ++i){
        o[ n - i - 1 ] = ( v & 1 ) ? '1' : '0';
        v = v >> 1;
    }
    printf("%s", o );
    free( o );
}

void blm_pr( struct bloom_filter* blm )
{
    printf("--- bloom filter ---\n");
    printf("Bits    = %lu\n", blm->bits  );
    printf("bb_nr   = %lu\n", blm->bb_nr );
    printf("hash_nr = %lu\n", blm->hash_func_nr );
    printf("bb = \n");
    for( size_t i = 0; i < blm->bb_nr; ++i ){
        printf("    bb[%lu] = ", i );
        binary_pr( &( blm->bb[i] ) , 64 );
        printf("\n");
    }
    printf("--------------------\n");
}
