#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rb_tree.h"

#define TEST_SIZE 20000

struct rb_tree* T;

int main()
{
    T = rb_tree_alloc();

    srand( (unsigned int)time(0) );

    printf("\n=== test for rb_tree ===\n");
    printf("Key none    = %d\n", RB_KEY_NONE    );
    printf("Key lowest  = %d\n", RB_KEY_LOWEST  );
    printf("Key highest = %d\n", RB_KEY_HIGHEST );

    printf("\n=== insert test ===\n");
    printf("insert %lu random keys\n", (size_t)TEST_SIZE );

    struct rb_node* nodes[ TEST_SIZE ];
    for(size_t i = 0; i < TEST_SIZE; ++i){
        size_t key = rand() % RB_KEY_RANGE + RB_KEY_LOWEST;
        nodes[i] = rb_node_alloc();
        rb_node_set_key( nodes[i], key );
        nodes[i]->parent = NULL;
        nodes[i]->left = NULL;
        nodes[i]->right = NULL;
        nodes[i]->color = RB_COLOR_RED;

        rb_insert( T, nodes[i] );
    }

    printf("\n=== check if rb_tree is good ===\n");
    int bad = rb_tree_check( T );
    if( !bad ){
        printf("Good rb_tree\n");
    }
    else{
        printf("Bad rb_tree : %d\n", bad );
    }

    printf("\n=== delete test ===\n");
    printf("delete random %lu keys\n", (size_t)( TEST_SIZE / 10 ) );
    for(size_t i = 0; i < TEST_SIZE / 10; ++i){
        rb_delete( T, nodes[i]->key );
    }

    printf("\n=== check if rb_tree is good ===\n");
    bad = 0;
    bad = rb_tree_check( T );
    if( !bad ){
        printf("Good rb_tree\n");
    }
    else{
        printf("Bad rb_tree : %d\n", bad );
    }

    return 0;
}
