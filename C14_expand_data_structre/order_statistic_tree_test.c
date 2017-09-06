#include <stdio.h>
#include <stdlib.h>
#include "order_statistic_tree.h"

#define INSERT_SIZE 1000

RB_NODE_KEY_TYPE DELETE_ELEMENT = 500;

#define SELECT_SIZE 5
size_t to_select[ SELECT_SIZE ] = {1, 302, 500, 999, 1000};

#define RANK_SIZE 9
RB_NODE_KEY_TYPE to_rank[ RANK_SIZE ] = {1, 2, 203, 499, 500, 501, 788, 999, 1000};

/*int main(int argc, char* argv[])*/
int main() 
{
    /*if( argc <= 1 || argv == NULL ) return 0;*/

    struct rb_tree* T =  rb_tree_alloc();

    // insert 
    printf("1. insert 1 to 1000 elements\n");
    for(size_t i = 0; i < INSERT_SIZE; ++i){
        struct rb_node* node = rb_node_alloc();
        node->key = i + 1;
        node->color = RB_COLOR_RED;

        rb_insert( T, node );
    }
    // delete 
    printf("2. delete element 500\n");
    rb_delete( T, DELETE_ELEMENT );

    // check 
    printf("3. check rb_tree\n");
    rb_tree_check( T );

    // test select 
    printf("4. test select\n");
    for(size_t i = 0; i < SELECT_SIZE; ++i){
        size_t index = to_select[i];
        printf("to select the %lu th element\n", index);
        struct rb_node* node = rb_select( T, index );
        rb_node_pr( node, 0 );
    }

    // test rank 
    printf("5. test rank\n");
    for(size_t i = 0; i < RANK_SIZE; ++i){
        RB_NODE_KEY_TYPE key = to_rank[i];
        printf("to rank key : %lu\n", key );
        struct rb_node* node = rb_find_node( T, key );
        rb_node_pr( node , 0);
        size_t rank = rb_rank( T,  node );
        printf("rank = %lu\n", rank);
    }
    return 0;
}
