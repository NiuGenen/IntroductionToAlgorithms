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
    // red black tree attribute
    RB_NODE_KEY_TYPE key;
    int color;
    struct rb_node* left;
    struct rb_node* right;
    struct rb_node* parent;
    // one more attribute to support order-statistic
    size_t size; // the number of nodes in this sub_tree
    /*
     * to maintain size 
     * 1. insert new node 
     *   1.1 increase by 1 when add new node 
     *   1.2 modify rotate when fixup 
     * 2. delete node
     *   2.1 decrease by 1 when delete node 
     *   2.2 modify rotate when fixup
     */ 
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

/*
 * core func
 */

int rb_insert(
    struct rb_tree* T,
    struct rb_node* node );
int rb_delete(
    struct rb_tree* T,
    RB_NODE_KEY_TYPE key );
struct rb_node* rb_find_node(
    struct rb_tree* T,
    RB_NODE_KEY_TYPE key );
void rb_node_left_rotate(
    struct rb_tree* T,
    struct rb_node* node);
void rb_node_right_rotate(
    struct rb_tree* T,
    struct rb_node* node);
void rb_insert_fixup(
    struct rb_tree* T,
    struct rb_node* node);
void rb_transplant(
    struct rb_tree* T,
    struct rb_node* old_,
    struct rb_node* new_);
void rb_delete_fixup(
    struct rb_tree* T,
    struct rb_node* fix);
// find the @index node of the acsending sorted nodes by key
struct rb_node* rb_select(
    struct rb_tree* T,
    size_t index);
struct rb_node* rb_select_help(
    struct rb_node* node,
    size_t index);
// give the @index of the node in this tree 
size_t rb_rank(
    struct rb_tree* T,
    struct rb_node* node);

/*
 * print func 
 */

void rb_tree_pr(  struct rb_tree* T );
void rb_node_pr(  struct rb_node* node, int if_pr_child );
void rb_color_pr( struct rb_node* node );
void rb_key_pr(   RB_NODE_KEY_TYPE key );
#define RB_TREE_VISIT_NLR 1
#define RB_TREE_VISIT_LNR 2
#define RB_TREE_VISIT_LRN 3
void rb_tree_pr_key( struct rb_tree* T , int pr_type);
void rb_node_pr_key( struct rb_node* node, int pr_type );

/*
 * common func 
 */

int rn_node_parent_red(   struct rb_node* node );
int rn_node_parent_black( struct rb_node* node );

int rb_node_is_root( struct rb_tree* T, struct rb_node* node );
int rb_node_is_leaf(   struct rb_node* node );
int rb_node_is_left(   struct rb_node* node );
int rb_node_is_right(  struct rb_node* node );
int rb_node_is_black(  struct rb_node* node );
int rb_node_is_red(    struct rb_node* node );
int rb_node_parent_id_left(  struct rb_node* node );
int rb_node_parent_id_right( struct rb_node* node );

int rb_node_has_left(         struct rb_node* node );
int rb_node_has_right(        struct rb_node* node );
int rb_node_has_parent(       struct rb_node* node );
int rb_node_has_grand_parent( struct rb_node* node );

/*
 * check rb_tree 
 */
int rb_node_check_color(
    struct rb_node* node,
    int if_check_child );
int rb_tree_check( struct rb_tree* T );
int rb_tree_check_black_hight_get_hight(
    struct rb_tree* T,
    struct rb_node* node_leaf);
int rb_tree_check_black_hight_help(
    struct rb_tree* T,
    struct rb_node* node );
int rb_tree_check_black_hight(
    struct rb_tree* T);
int rb_tree_check_size(
    struct rb_tree* T);
int rb_node_check_size(
    struct rb_node* node);

#endif
