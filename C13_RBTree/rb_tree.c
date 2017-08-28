#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "rb_tree.h"

/*
 * alloc an empty rb_node
 *
 * free one rb_node pointer
 *
 */

struct rb_node* alloc_rb_node()
{
    struct rb_node* ret = (struct rb_node)malloc( sizeof(struct rb_node) );
    ret->key    = RB_KEY_NONE;
    ret->left   = NULL;
    ret->right  = NULL;
    ret->parent = NULL;
    ret->color  = RB_COLOR_NONE;
}

void free_rb_node( struct rb_node** node )
{
    if( *node != NULL ){
        free(*node);
        *node = NULL;
    }
}

/*
 * operation of KEY
 *
 * cmp
 * set
 *
 */

int rb_key_cmp_in_node( struct rb_node* x, struct rb_node* y )
{
    if( x != NULL && y != NULL ){
        if( x->key < y->key ){
            return -1;
        }
        else if( x->key > y->key ){
            return 1;
        }
        else{
            return 0;
        }
    }
}

int rb_key_cmp( RB_NODE_KEY_TYPE x, RB_NODE_KEY_TYPE y )
{
    if( x < y ){
        return -1;
    }
    else if( x > y ){
        return 1;
    }
    else{
        return 0;
    }
}

void set_rb_node_key( struct rb_node* node, RB_NODE_KEY_TYPE key )
{
    if( node != NULL ){
        node->key = key;
    }
}

/*
 * alloc an empty rb_tree
 *
 * free one rb_tree pointer
 *
 */

struct rb_tree* alloc_rb_tree()
{
    struct rb_tree* ret = (struct rb_tree*)malloc( sizeof(struct rb_tree) );
    ret->node_nr = 0;
    ret->root = NULL;
}

void free_rb_tree( struct rb_tree** T )
{
    if( *T != NULL ){
        free(*T);
        *T = NULL;
    }
}

/*
 * core of rb_tree
 *
 * insert(T,node)
 *
 * delete(T,key)
 *
 */

int rb_insert( struct rb_tree* T, struct rb_node* node )
{
    if( T == NULL ) return 0;
    if( T->root == NULL ) return 0;
}

int rb_delete( struct rb_tree* T, RB_NODE_KEY_TYPE key )
{
    if( T == NULL ) return 0;
    if( T->root == NULL ) return 0;
}

/*
 * print rb tree
 */

void pr_rb_tree( struct rb_tree* T )
{
    if( T == NULL ) return;
    if( T->root == NULL ) return;

    printf("====== RB TREE =====\n");
    printf("Node nr = %lu\n", T->node_nr );
    pr_rb_node( T->root );
    printf("====================\n");
}

void pr_rb_node( struct rb_node* node , int if_pr_child )
{
    if( node == NULL ) return;

    printf("----- NODE -----\n");
    printf("Address = %x\n", node);
    printf("Key = ");   pr_rb_key( node->key ); printf("\n");
    printf("Color = "); pr_rb_color( node );    printf("\n");
    printf("Left   = %x\n", node->left   );
    printf("Right  = %x\n", node->right  );
    printf("Parent = %x\n", node->parent );
    printf("----------------\n");

    if( if_pr_child ){
        pr_rb_node( node->left  );
        pr_rb_node( node->right );
    }
}

void pr_rb_key( RB_NODE_KEY_TYPE key )
{
    printf("%lu", key);
}

void pr_rb_color( struct rb_node* node )
{
    if( node == NULL ) return;

    switch( node->color ){
        case RB_COLOR_NONE:  printf("NONE");   break;
        case RB_COLOR_BLACK: printf("BLACK");  break;
        case RB_COLOR_RED:   printf("RED");    break;
        default:             printf("WHAT???") break;
    }

}
