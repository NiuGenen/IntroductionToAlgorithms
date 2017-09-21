#define ITA_DBG_ENABLE

#include "interval_tree.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "../util/ask_time.h"

#define TEST_SIZE 10000
ssize_t low_s[ TEST_SIZE ];
ssize_t hig_s[ TEST_SIZE ];

#define RANGE 10000

int main()
{
    srand((unsigned int)time(0));

    // alloc rb_tree
    struct rb_tree* T = rb_tree_alloc();
    // get random elements
    for( size_t i = 0; i < TEST_SIZE; ++i ){
        low_s[i] = rand()%RANGE + 1;
        hig_s[i] = rand()%RANGE + low_s[i]+ 1;
    }
    // time asker
    time_asker_t asker = asker_alloc( ASKER_TYPE_CLOCK );
    // insert test 
    ITA_DBG_INFO( ITA_DBG_T_STR, "[TEST] insert start..." );
    start_ask_time(asker);
    for( size_t i = 0; i < TEST_SIZE; ++i ){
        struct rb_node* node  = rb_node_alloc();
        node->key->entry.low  = low_s[i];
        node->key->entry.high = hig_s[i];
        node->key->max        = hig_s[i];
        rb_insert( T, node );
    }
    end_ask_time(asker);
    ITA_DBG_INFO( ITA_DBG_T_STR, "[TEST] insert end....." );
    // get running time
    ask_time(asker);
    // check rb_tree
    ITA_DBG_INFO( ITA_DBG_T_STR, "rb_tree check....." );
    rb_tree_check( T );

    // delete
    RB_NODE_KEY_TYPE key = (RB_NODE_KEY_TYPE)malloc( sizeof(struct interval_key) );
    ITA_DBG_INFO( ITA_DBG_T_STR, "[TEST] delete start......" );
    reset_asker( asker );
    start_ask_time( asker );
    for( size_t i = 0; i < TEST_SIZE / 10 ; ++i ){
        key->entry.low  = low_s[i];
        key->entry.high = hig_s[i];
        key->max        = hig_s[i];

        rb_delete( T, key );
    }
    end_ask_time( asker );
    ITA_DBG_INFO( ITA_DBG_T_STR, "[TEST] delete end......" );
    // get running time
    ask_time(asker);
    // check rb_tree
    ITA_DBG_INFO( ITA_DBG_T_STR, "rb_tree check....." );
    rb_tree_check( T );

    ITA_DBG_INFO( ITA_DBG_T_STR, "[TEST] search start ......" );
    key->entry.low  = 100;
    key->entry.high = 200;
    struct rb_node* node = interval_search( T, key );
    if( node == NULL ){
        ITA_DBG_INFO( ITA_DBG_T_STR, "no overlap [100,200]");
        /*rb_tree_pr( T );*/
    }
    else{
        ITA_DBG_INFO( ITA_DBG_T_STR, "Overlap!");
        rb_node_pr( node , 0 );
    }
    ITA_DBG_INFO( ITA_DBG_T_STR, "[TEST] search start ......" );

    return 0;
}
