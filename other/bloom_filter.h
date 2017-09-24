#ifndef _BLOOM_FILTER_H_
#define _BLOOM_FILTER_H_

#define BLM_DBG

#include "hash_func.h"
#include <stdint.h>
#include <stddef.h>

/*
 * user defination
 */

struct bloom_filter_entry;

typedef hash_func_ret_t (*blm_hash_t)(
        struct bloom_filter_entry* entry,
        hash_fn_t hash_func );

#ifdef BLM_DBG
#include <stdio.h>
typedef void (*blm_entry_pr_t)(
        struct bloom_filter_entry* entry );
#endif

/*
 * bloom filter
 *
 */
struct bloom_filter{
    size_t     bits;
    uint64_t*  bb;
    size_t     bb_nr;
    hash_fn_t* hash_func_s;
    size_t     hash_func_nr;
    blm_hash_t blm_hash;

    void (*add)(
            struct bloom_filter* blm,
            struct bloom_filter_entry* entry);
    int (*not_exist)(
            struct bloom_filter* blm,
            struct bloom_filter_entry* entry);
    void (*pr)(
            struct bloom_filter* blm);

#ifdef BLM_DBG
    blm_entry_pr_t entry_pr;
#endif

};

struct bloom_filter* blm_alloc(
        size_t bits,
        hash_fn_t* hash_func_s,
        size_t hash_fn_nr,
        blm_hash_t blm_hash
#ifdef BLM_DBG
        ,blm_entry_pr_t entry_pr
#endif
        );

void blm_free(
        struct bloom_filter* blm );


#endif
