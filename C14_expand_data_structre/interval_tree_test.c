#include "interval_tree.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define TEST_SIZE 100
ssize_t low_s[ TEST_SIZE ];
ssize_t hig_s[ TEST_SIZE ];

#define RANGE 10000

int main()
{
    srand((unsigned int)time(0));

    struct rb_tree* T = rb_tree_alloc();

    for( size_t i = 0; i < TEST_SIZE; ++i ){
        low_s[i] = rand()%RANGE + 1;
        hig_s[i] = rand()%RANGE + low_s[i]+ 1;

        struct rb_node* node  = rb_node_alloc();
        node->key->entry.low  = low_s[i];
        node->key->entry.high = hig_s[i];
        node->key->max        = hig_s[i];

        rb_insert( T, node );
    }

    rb_tree_check( T );
    /*rb_tree_pr( T );*/

    RB_NODE_KEY_TYPE key = (RB_NODE_KEY_TYPE)malloc( sizeof(struct interval_key) );
    for( size_t i = 0; i < TEST_SIZE / 10 ; ++i ){
        key->entry.low  = low_s[i];
        key->entry.high = hig_s[i];
        key->max        = hig_s[i];

        rb_delete( T, key );
    }

    rb_tree_check( T );

    key->entry.low  = 100;
    key->entry.high = 200;
    struct rb_node* node = interval_search( T, key );
    if( node == NULL ){
        printf("no overlap [100,200]\n");
        /*rb_tree_pr( T );*/
    }
    else{
        printf("Overlap!\n");
        rb_node_pr( node , 0 );
    }


    return 0;
}
