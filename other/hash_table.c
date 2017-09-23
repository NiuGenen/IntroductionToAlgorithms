#include "hash_table.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef NULL 
#define NULL 0
#endif

struct hash_table* hash_table_alloc(
    size_t size,
    hash_fn_t hash_func,
    ht_entry_pr_t   hash_entry_pr   ,
    ht_entry_cmp_t  hash_entry_cmp  ,
    ht_entry_hash_t hash_entry_hash ,
    ht_entry_free_t hash_entry_free )
{
    struct hash_table* ht = (struct hash_table*)malloc( sizeof(struct hash_table) );
    if( !ht ) return NULL;

    ht->size = size;
    ht->hash_func       = hash_func      ;
    ht->hash_entry_pr   = hash_entry_pr  ;
    ht->hash_entry_cmp  = hash_entry_cmp ;
    ht->hash_entry_hash = hash_entry_hash;
    ht->hash_entry_free = hash_entry_free;

    ht->block = (struct hash_table_block**)malloc( sizeof( struct hash_table_block* ) * ht->size );
    for(size_t i = 0; i < ht->size; ++i){
        ht->block[i] = NULL;
    }
    /*memset( ht->block, 0, sizeof( struct hash_table_block*) * ht->size );*/

    ht->insert_entry  = hash_table_insert_entry;
    ht->exist_entry   = hash_table_exist_entry;
    ht->delete_entry  = hash_table_delete_entry;
    ht->hash_table_pr = hash_table_pr;

    return ht;
}

void hash_table_free_block( ht_entry_free_t entry_free, struct hash_table_block* block )
{
    if( block == NULL ) return;
    if( block->block_entry == NULL ) return;

    struct hash_table_block_entry* block_entry  = block->block_entry;
    while( block_entry != NULL ){
        struct hash_table_block_entry* to_free = block_entry;
        block_entry = block_entry->next;
        entry_free( to_free->entry );
        free( to_free );
    }
    free(block);
}

void hash_table_free( struct hash_table* ht )
{
    for(size_t i = 0; i < ht->size; ++i){
        if( ht->block[i] != NULL ){
            hash_table_free_block( ht->hash_entry_free, ht->block[i] );
            ht->block[i] = NULL;
        }
    }
    free( ht->block ); ht->block = NULL;
}

void hash_table_insert_entry(
    struct hash_table* ht,
    struct hash_table_entry* entry)
{
    hash_func_ret_t hash = ht->hash_entry_hash( entry , ht->hash_func );
    hash = hash % ht->size;

    struct hash_table_block_entry* block_entry = (struct hash_table_block_entry*)malloc( sizeof( struct hash_table_block_entry ) );
    block_entry->entry = entry;
    block_entry->next  = NULL;

    if( ht->block[ hash ] == NULL ){
        ht->block[ hash ] = (struct hash_table_block*)malloc( sizeof( struct hash_table_block ) );
        ht->block[ hash ]->block_entry = block_entry;
        ht->block[ hash ]->nr = 1;
    }else{
        struct hash_table_block_entry* block_entry_ = ht->block[ hash ]->block_entry;
        block_entry->next = block_entry_;
        ht->block[ hash ]->block_entry = block_entry;
        ht->block[ hash ]->nr += 1;
    }
}

int  hash_table_exist_entry(
    struct hash_table* ht,
    struct hash_table_entry* entry)
{
    hash_func_ret_t hash = ht->hash_entry_hash( entry, ht->hash_func );
    hash = hash % ht->size;
    
    if( ht->block[ hash ] == NULL ) return 0;

    struct hash_table_block_entry* block_entry = ht->block[ hash ]->block_entry;
    while( block_entry != NULL ){
        if( ht->hash_entry_cmp( entry, block_entry->entry ) == 0 ){
            return 1;
        }
    }

    return 0;
}

void hash_table_delete_entry(
    struct hash_table* ht,
    struct hash_table_entry* entry )
{
    hash_func_ret_t hash = ht->hash_entry_hash( entry, ht->hash_func );
    hash = hash % ht->size;

    if( ht->block[ hash ] == NULL )  return;
    if( ht->block[ hash ]->block_entry == NULL ) return;
    if( ht->block[ hash ]->nr == 0 ) return;

    struct hash_table_block_entry* head = ht->block[ hash ]->block_entry;
    struct hash_table_block_entry* block_entry = head;
    struct hash_table_block_entry* block_entry_prev = NULL;
    while( block_entry != NULL ){
        /*if( block_entry->entry == entry ){ // same entry */
        if( ht->hash_entry_cmp( entry, block_entry->entry ) == 0 ){ // same entry 
            if( block_entry == head ) ht->block[ hash ]->block_entry = head->next;
            else block_entry_prev->next = block_entry->next;
            ht->block[ hash ]->nr -= 1;
            free( block_entry );
            break;
        }
        block_entry_prev = block_entry;
        block_entry = block_entry->next;
    }
    
}

void hash_table_pr( struct hash_table* ht )
{
    if( ht == NULL ) return;

    printf("--- HASH TABLE ---\n");
    printf("Size = %lu\n", ht->size );
    for( size_t i = 0; i < ht->size; ++i ){
        if( ht->block[ i ] == NULL ){
            printf("Block[%5lu] = NULL\n", i );
        }else{
            printf("Block[%5lu] = %lu\n", i, ht->block[i]->nr );
            struct hash_table_block_entry* block_entry = ht->block[i]->block_entry;
            size_t count = 0;
            while( block_entry != NULL ){
                printf("    entry[%5lu] = ", count++ );
                ht->hash_entry_pr( block_entry->entry );
                block_entry = block_entry->next;
                printf("\n");
            }
        }
    }
}
