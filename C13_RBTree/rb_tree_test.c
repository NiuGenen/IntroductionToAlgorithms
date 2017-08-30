#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rb_tree.h"

#define TEST_SIZE 10000

struct rb_tree* T;

int main()
{
    T = rb_tree_alloc();

    srand( (unsigned int)time(0) );

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

    int bad = rb_tree_check( T );
    if( !bad ){
        printf("Good rb_tree\n");
    }
    else{
        printf("Bad rb_tree : %d\n", bad );
    }

    return 0;
}
