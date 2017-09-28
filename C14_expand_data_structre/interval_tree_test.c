#define ITA_DBG_ENABLE

#include "interval_tree.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../util/ask_time.h"
#include "../util/dbg_info.h"

#define RANGE 10000

char DEFAULT_INPUT_FILE[100] = "./interval_tree.random\0";

int main(int argc, char* argv[])
{
    // get input file name
    char* INPUT_FILE = DEFAULT_INPUT_FILE;
    if( argc > 1 ){
        INPUT_FILE = argv[1];
    }else{
        printf("[WRONG] not enough arguments\n");
        printf("using default input file : %s\n", INPUT_FILE );
    }
    printf("Read data from file : %s\n", INPUT_FILE );

    // try to open input file
    int fd = open( INPUT_FILE, O_RDONLY, 0x0777 );
    if( fd < 0 ){
        ITA_DBG_INFO_( ITA_DBG_T_STR, "Fail to open file : " , INPUT_FILE );
        return 1;
    }

    // read test_size
    size_t test_size = 0;
    read( fd, &test_size, sizeof( size_t ) );
ITA_DBG_INFO_( ITA_DBG_T_UINT ,"test_size = " , &test_size );
    size_t* low_s = (size_t*)malloc( sizeof(size_t) * test_size );
    size_t* hig_s = (size_t*)malloc( sizeof(size_t) * test_size );

    // alloc rb_tree
    struct rb_tree* T = rb_tree_alloc();
    // alloc time asker
    time_asker_t asker = asker_alloc( ASKER_TYPE_CLOCK );

    // read data from input file
    for( size_t i = 0; i < test_size; ++i ){
        read( fd, low_s + i, sizeof( ssize_t ) );
        read( fd, hig_s + i, sizeof( ssize_t ) );
    }
    // insert test 
    ITA_DBG_INFO( ITA_DBG_T_STR, "[TEST] insert start..." );
start_ask_time(asker);
    for( size_t i = 0; i < test_size ; ++i ){
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
    for( size_t i = 0; i < test_size / 10 ; ++i ){
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
    }
    else{
        ITA_DBG_INFO( ITA_DBG_T_STR, "Overlap!");
        rb_node_pr( node , 0 );
    }
    ITA_DBG_INFO( ITA_DBG_T_STR, "[TEST] search end ......" );

    return 0;
}
