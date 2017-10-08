#include "bloom_filter.h"
#include <stdlib.h>
#include <string.h>

struct bloom_filter_entry{
    char* str;
    size_t n;
};

/*hash_func_ret_t blm_hash( struct bloom_filter_entry* entry, hash_fn_t hash_func )*/
hash_func_ret_t blm_hash( void * entry, hash_fn_t hash_func )
{
    struct bloom_filter_entry* _entry = ( struct bloom_filter_entry* )entry;
    return hash_func( _entry->str, _entry->n );
}

#ifdef BLM_DBG
void entry_pr( void * entry )
{
    struct bloom_filter_entry* _entry = ( struct bloom_filter_entry* )entry;
    printf("Entry = %s\n", _entry->str );
}
#endif

int main()
{
    hash_fn_t *hash_func_s = (hash_fn_t*)malloc( sizeof( hash_fn_t ) * 4 );

    hash_func_s[ 0 ] = hash_js;
    hash_func_s[ 1 ] = hash_djb;
    hash_func_s[ 2 ] = hash_pjw;
    hash_func_s[ 3 ] = hash_ap;

    struct bloom_filter* blm = blm_alloc( 256, hash_func_s, 4, blm_hash
#ifdef BLM_DBG
    , entry_pr
#endif
    );

    char str[10][11] = {
        "asdtebf\0",    // 0  7
        "qwezxcvvf\0",  // 1  9
        "zxcvvbds\0",   // 2  8
        "tsffdyh\0",    // 3  7
        "tnbgfvv\0",    // 4  7
        "bfgytyhg\0",   // 5  8
        "vzxcvdf\0",    // 6  7
        "ynbgffvx\0",   // 7  8
        "146785434\0",  // 8  9
        "asdtebf\0"     // 9  7
    };

    char str_not_exist[10] = "qweasd";

    int str_len[10] = { 7,9,8,7,7,8,7,8,9,7 };

#ifdef BLM_DBG
    printf("BLM debug on\n");
#endif

    for( size_t i = 0; i < 10; ++i ){
        struct bloom_filter_entry* entry
            = ( struct bloom_filter_entry* )malloc( sizeof( struct bloom_filter_entry ) );
        entry->str = (char* )malloc( 10 );
        memcpy( entry->str , str[i], str_len[i] );
        entry->n = str_len[i];

        blm->add( blm, entry );
        free( entry );
    }

    blm->pr( blm );

    struct bloom_filter_entry* entry
            = ( struct bloom_filter_entry* )malloc( sizeof( struct bloom_filter_entry ) );
    entry->str = (char* )malloc( 10 );
    memcpy( entry->str , str_not_exist, 6 );
    int ret = blm->not_exist( blm, entry );
    printf("if not exist = %d\n", ret);

    return 0;
}
