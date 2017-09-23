#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#include "hash_func.h"
#include <stddef.h>

/*
 * entry represents one KEY-VALUE
 * 
 * at least two arrtibute
 * {
 *   key;
 *   value;
 * }
 * 
 * using KEY to search
 * 
 */
struct hash_table_entry;

typedef void (*ht_entry_pr_t)(
    struct hash_table_entry* hte);

typedef int (*ht_entry_cmp_t)(
    struct hash_table_entry* hte1,
    struct hash_table_entry* hte2);

typedef hash_func_ret_t (*ht_entry_hash_t)(
    struct hash_table_entry* hte,
    hash_fn_t hash_func );
typedef void (*ht_entry_free_t)(
    struct hash_table_entry* entry );

/*
 * using linked_list to solve hash collision
 * 
 * not good enough
 * 
 */
struct hash_table_block_entry{
    struct hash_table_entry* entry;
    struct hash_table_block_entry* next;
};

struct hash_table_block{
    struct hash_table_block_entry* block_entry;
    size_t nr;
};

struct hash_table{
    size_t size                     ;
    struct hash_table_block** block ;
    hash_fn_t       hash_func       ;
    ht_entry_pr_t   hash_entry_pr   ;
    ht_entry_cmp_t  hash_entry_cmp  ;
    ht_entry_hash_t hash_entry_hash ;
    ht_entry_free_t hash_entry_free ;
    void (*insert_entry)(
        struct hash_table* ht,
        struct hash_table_entry* entry);
    int (*exist_entry)(
        struct hash_table* ht,
        struct hash_table_entry* entry);
    void (*delete_entry)(
        struct hash_table* ht,
        struct hash_table_entry* entry);
    void (*hash_table_pr)(
        struct hash_table* ht );
};

struct hash_table* hash_table_alloc(
    size_t size,
    hash_fn_t hash_func,
    ht_entry_pr_t   hash_entry_pr,
    ht_entry_cmp_t  hash_entry_cmp,
    ht_entry_hash_t hash_entry_hash,
    ht_entry_free_t hash_entry_free );
void hash_table_free(
    struct hash_table* ht);

void hash_table_insert_entry(
    struct hash_table* ht,
    struct hash_table_entry* entry);
int hash_table_exist_entry(
    struct hash_table* ht,
    struct hash_table_entry* entry );
void hash_table_delete_entry(
    struct hash_table* ht,
    struct hash_table_entry* entry );
void hash_table_pr(
    struct hash_table* ht );

#endif
