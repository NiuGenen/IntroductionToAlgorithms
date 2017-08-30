#ifndef _NG_RB_TREE_H_
#define _NG_RB_TREE_H_

#include <stdint.h>
#include <stdlib.h>

#ifndef NULL
#define NULL 0
#endif

#define RB_COLOR_NONE  0
#define RB_COLOR_BLACK 1
#define RB_COLOR_RED   3

typedef size_t RB_NODE_KEY_TYPE;

#define RB_KEY_NONE    0
#define RB_KEY_LOWEST  1
#define RB_KEY_HIGHEST 100000000
#define RB_KEY_RANGE   ( RB_KEY_HIGHEST - RB_KEY_LOWEST + 1 )

struct rb_node{
    RB_NODE_KEY_TYPE key;
    int color;
    struct rb_node* left;
    struct rb_node* right;
    struct rb_node* parent;
};

struct rb_node* rb_node_alloc();
void rb_node_free(
    struct rb_node** node, 
    int if_free_child );

int rb_key_cmp(
    RB_NODE_KEY_TYPE x,
    RB_NODE_KEY_TYPE y );
void rb_node_set_key(
    struct rb_node* node,
    RB_NODE_KEY_TYPE key );

struct rb_tree{
    struct rb_node* root;
    size_t node_nr;
};

struct rb_tree* rb_tree_alloc();
void rb_tree_free(
    struct rb_tree** T,
    int if_free_child );

int rb_insert(
    struct rb_tree* T,
    struct rb_node* node );
int rb_delete(
    struct rb_tree* T,
    RB_NODE_KEY_TYPE key );
struct rb_node* rb_find_node(
    struct rb_tree* T,
    RB_NODE_KEY_TYPE key );

void rb_tree_pr( struct rb_tree* T );
void rb_node_pr( struct rb_node* node, int if_pr_child );
void rb_key_pr( RB_NODE_KEY_TYPE key );

int rb_node_is_leaf( struct rb_node* node );
int rb_node_has_left( struct rb_node* node );
int rb_node_has_right( struct rb_node* node );
int rb_node_has_parent( struct rb_node* node );
int rb_node_is_black( struct rb_node* node );
int rb_node_is_red( struct rb_node* node );
int rb_node_check_color(
    struct rb_node* node,
    int if_check_child );
int rb_tree_check( struct rb_tree* T );

#endif
