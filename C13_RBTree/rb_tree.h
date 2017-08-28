#ifndef _NG_RB_TREE_H_
#define _NG_RB_TREE_H_

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

struct rb_node{
    RB_NODE_KEY_TYPE key;
    int color;
    struct rb_node* left;
    struct rb_node* right;
    struct rb_node* parent;
};

struct rb_node* alloc_rb_node();
void free_rb_node( struct rb_node* node);

int rb_key_cmp_in_node( struct rb_node* x, struct rb_node* y );
int rb_key_cmp( RB_NODE_KEY_TYPE x, RB_NODE_KEY_TYPE y );
void set_rb_node_key( struct rb_node* node, RB_NODE_KEY_TYPE key );

struct rb_tree{
    struct rb_node* root;
    size_t node_nr;
};

struct rb_tree* alloc_rb_tree();

int rb_insert( struct rb_tree* T, struct rb_node* node );
int rb_delete( struct rb_tree* T, RB_NODE_KEY_TYPE key );
void pr_rb_tree( struct rb_tree* T );
void pr_rb_node( struct rb_node* node, int if_pr_child );
void pr_rb_key( RB_NODE_KEY_TYPE key );

#endif
