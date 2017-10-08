#include "hash_table.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define _MALLOC_(t,n) ( (t*)malloc(sizeof(t)*(n)) )

struct hash_table_entry{
    char* str;
    size_t n;
};

void ht_entry_free( void * entry )
{
    if( entry == NULL ) return;
    struct hash_table_entry* _entry = ( struct hash_table_entry* )entry;
    free( _entry->str );
    free( _entry );
}

void ht_entry_pr( void * entry )
{
    struct hash_table_entry* hte = (struct hash_table_entry*)entry;
    char c[16] = {
        '0','1','2','3',
        '4','5','6','7',
        '8','9','a','b',
        'c','d','e','f'
    };
    /*printf("%s\n", hte->str );*/
    for(size_t i = 0; i < hte->n; ++i){
        printf("%c", c[ ((hte->str[i] & 0xF0) >> 4 ) & 0x0F ] );
        printf("%c", c[   hte->str[i] & 0x0F                ] );
    }
}

int ht_entry_cmp(
    void * entry_1,
    void * entry_2)
{
    struct hash_table_entry* hte1 = ( struct hash_table_entry* )entry_1;
    struct hash_table_entry* hte2 = ( struct hash_table_entry* )entry_2;

    if( hte1->n != hte2->n ) return 0;
    
    for(size_t i = 0; i < hte1->n; ++i){
        if( hte1->str[i] != hte2->str[i] ){
            return 0;
        }
    }

    return 1;
}

hash_func_ret_t ht_entry_hash(
    void * entry,
    hash_fn_t hash_func)
{
    struct hash_table_entry* hte = ( struct hash_table_entry* )entry;
    return hash_func( hte->str, hte->n );
}

/*
 * (size_t n)(char str[n])
 * 
 */
#define HASH_STR_FILE "./hash_str.random"

int main()
{
    int fd = open( HASH_STR_FILE, O_RDONLY, 0x777 );

    struct hash_table* ht = hash_table_alloc(
        1024, hash_djb,
        ht_entry_pr, ht_entry_cmp, ht_entry_hash, ht_entry_free );

    const int str_nr = 128;

    size_t n;
    for(size_t i = 0; i < str_nr; ++i){
        struct hash_table_entry* entry = _MALLOC_( struct hash_table_entry, 1 );
        // read str length
        read( fd, &n, sizeof(size_t) );
        printf("Read str.len = %lu ; ", n);
        // malloc str buf
        entry->str = _MALLOC_( char, n+1 );
        // read string
        read( fd, entry->str, n );
        // build entry
        entry->str[n] = '\0';
        entry->n = n;
        printf("insert into hash_table\n");
        // insert into hash_table
        ht->insert_entry( ht, entry );
    }

    ht->hash_table_pr( ht );

    printf("Now to free\n");
    hash_table_free( ht );

    return 0;
}
