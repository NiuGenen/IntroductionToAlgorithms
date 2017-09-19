#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "interval_tree.h"

/*
 * alloc an empty rb_node
 *
 * free one rb_node pointer
 *
 */

struct rb_node* rb_node_alloc()
{
    struct rb_node* ret =
        (struct rb_node*)malloc( sizeof(struct rb_node) );
    /*ret->key    = RB_KEY_NONE;*/
    ret->key    = (struct interval_key* )malloc( sizeof(struct interval_key) );
    ret->left   = NULL;
    ret->right  = NULL;
    ret->parent = NULL;
    ret->color  = RB_COLOR_NONE;

    return ret;
}

void rb_node_free( struct rb_node** node , int if_free_child )
{
    if( node != NULL && *node != NULL ){
        if( if_free_child ){
            rb_node_free( &((*node)->left),  1 );
            rb_node_free( &((*node)->right), 1 );
        }
        if( (*node)->key != NULL ){
            free((*node)->key);
        }
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

void rb_node_recover_key( struct rb_node* node )
{
    if( node == NULL ) return;

    node->key->max = node->key->entry.high;
    if( rb_node_has_left( node ) == 1 && node->left->key != RB_KEY_NONE ){
        node->key->max = MAX( node->key->max, node->left->key->max  );
    }
    if( rb_node_has_right( node ) == 1 && node->right->key != RB_KEY_NONE ){
        node->key->max = MAX( node->key->max, node->right->key->max );
    }
}

int rb_key_overlap(
    RB_NODE_KEY_TYPE k1,
    RB_NODE_KEY_TYPE k2 )
{
    return k1->entry.low <= k2->entry.high && k2->entry.low <= k1->entry.high;
}

int rb_key_cmp( RB_NODE_KEY_TYPE k1, RB_NODE_KEY_TYPE k2 )
{
    ssize_t x = k1->entry.low;
    ssize_t y = k2->entry.low;

    if( x < y ){
        return -1;
    }
    else if( x > y ){
        return 1;
    }
    else{ // low == low
        x = k1->entry.high;
        y = k2->entry.high;
        if( x < y ){
            return -1;
        }
        else if( x > y ){
            return 1;
        }
        return 0;
    }
}

void rb_node_set_key(
        struct rb_node* node,
        RB_NODE_KEY_TYPE key )
{
    if( node != NULL ){
        if( node->key != NULL ){
            free( node->key );
            node->key = NULL;
        }
        node->key = key;
    }
}

/*
 * alloc an empty rb_tree
 *
 * free one rb_tree pointer
 *
 */

struct rb_tree* rb_tree_alloc()
{
    struct rb_tree* ret = (struct rb_tree*)malloc( sizeof(struct rb_tree) );
    ret->node_nr = 0;
    ret->root = NULL;

    return ret;
}

void rb_tree_free( struct rb_tree** T , int if_free_child )
{
    if( T != NULL && *T != NULL ){
        if( if_free_child ){
            rb_node_free( &( (*T)->root->left  ), 1 );
            rb_node_free( &( (*T)->root->right ), 1 );
        }
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

int rb_node_parent_red( struct rb_node* node )
{
    if( node == NULL ) return -1;
    if( node->parent == NULL ) return 0;

    return node->parent->color == RB_COLOR_RED;
}

int rb_node_parent_black( struct rb_node* node )
{
    if( node == NULL ) return -1;
    if( node->parent == NULL ) return 0;

    return node->parent->color == RB_COLOR_BLACK;
}

int rb_node_is_left( struct rb_node* node )
{
    if( node == NULL ) return -1;
    if( node->parent == NULL ) return 0;

    return node == node->parent->left;
}

int rb_node_is_right( struct rb_node* node )
{
    if( node == NULL ) return -1;
    if( node->parent == NULL ) return 0;

    return node == node->parent->right;
}

int rb_node_has_grand_parent( struct rb_node* node )
{
    if( node == NULL ) return -1;
    if( node->parent == NULL ) return 0;
    if( node->parent->parent == NULL ) return 0;
    return 1;
}

int rb_node_parent_is_left( struct rb_node* node )
{
    if( node == NULL ) return -1;
    if( node->parent == NULL ) return 0;
    if( node->parent->parent == NULL ) return 0;

    return node->parent == node->parent->parent->left;
}

int rb_node_parent_is_right( struct rb_node* node )
{
    if( node == NULL ) return -1;
    if( node->parent == NULL ) return 0;
    if( node->parent->parent == NULL ) return 0;

    return node->parent == node->parent->parent->right;
}

int rb_node_is_root( struct rb_tree* T, struct rb_node* node )
{
    if( T->root == NULL ) return 0;
    if( node == NULL ) return -1;

    return node == T->root;
}

void rb_node_left_rotate(
        struct rb_tree* T,
        struct rb_node* node )
{
    if( rb_node_has_right(node) > 0 ){
        struct rb_node* node_right = node->right;
        if( rb_node_has_left(node_right) > 0 ){
            node->right = node_right->left;
            node_right->left->parent = node;
        }else{
            node->right = NULL;
        }
        node_right->left = node;
        if( rb_node_has_parent(node) > 0 ){
            struct rb_node* node_parent = node->parent;
            if( rb_node_is_left(node) > 0){
                node_parent->left = node_right;
            }
            else{
                node_parent->right = node_right;
            }
            node_right->parent = node_parent;
        }
        else{
            node_right->parent = NULL;
        }
        node->parent = node_right;
        if( rb_node_is_root( T, node ) > 0 ){
            T->root = node_right;
        }
        // change max
        node_right->key->max = node->key->max;
        node->key->max = node->key->entry.high;
        if( rb_node_has_left(  node ) == 1 && node->left->key != RB_KEY_NONE ){
            node->key->max = MAX( node->key->max, node->left->key->max  );
        }
        if( rb_node_has_right( node ) == 1 && node->right->key != RB_KEY_NONE ){
            node->key->max = MAX( node->key->max, node->right->key->max );
        }
    }
}

void rb_node_right_rotate(
        struct rb_tree* T,
        struct rb_node* node )
{
    if( rb_node_has_left(node) > 0 ){
        struct rb_node* node_left= node->left;
        if( rb_node_has_right(node_left) > 0 ){
            node->left = node_left->right;
            node_left->right->parent = node;
        }else{
            node->left = NULL;
        }
        node_left->right = node;
        if( rb_node_has_parent(node) > 0 ){
            struct rb_node* node_parent = node->parent;
            if( rb_node_is_left(node) > 0){
                node_parent->left = node_left;
            }
            else{
                node_parent->right = node_left;
            }
            node_left->parent = node_parent;
        }
        else{
            node_left->parent = NULL;
        }
        node->parent = node_left;
        if( rb_node_is_root( T, node ) > 0 ){
            T->root = node_left;
        }
        // change max
        node_left->key->max = node->key->max;
        node->key->max = node->key->entry.high;
        if( rb_node_has_left(  node ) == 1 && node->left->key != RB_KEY_NONE ){
            node->key->max = MAX( node->key->max, node->left->key->max  );
        }
        if( rb_node_has_right( node ) == 1 && node->right->key != RB_KEY_NONE ){
            node->key->max = MAX( node->key->max, node->right->key->max );
        }
    }
}

void rb_insert_fixup(
        struct rb_tree * T,
        struct rb_node* node )
{
    while( rb_node_parent_red(node) > 0 ){
        if( rb_node_parent_is_left(node) > 0 ){
            struct rb_node* uncle = node->parent->parent->right;
            if( rb_node_is_red( uncle ) > 0 ){
                node->parent->color = RB_COLOR_BLACK;
                uncle->color = RB_COLOR_BLACK;
                node->parent->parent->color = RB_COLOR_RED;
                node = node->parent->parent;
            }
            else if( rb_node_is_right(node) > 0 ){
                node = node->parent;
                rb_node_left_rotate( T,  node );
            }
            else if( rb_node_has_parent(node) ){
                node->parent->color = RB_COLOR_BLACK;
                if( rb_node_has_grand_parent(node) ){
                    node->parent->parent->color = RB_COLOR_RED;
                    rb_node_right_rotate( T,  node->parent->parent );
                }
            }
        }
        else if( rb_node_parent_is_right(node) > 0) {
            struct rb_node* uncle = node->parent->parent->left;
            if( rb_node_is_red( uncle ) > 0 ){
                node->parent->color = RB_COLOR_BLACK;
                uncle->color = RB_COLOR_BLACK;
                node->parent->parent->color = RB_COLOR_RED;
                node = node->parent->parent;
            }
            else if( rb_node_is_left(node) > 0 ){
                node = node->parent;
                rb_node_right_rotate( T, node );
            }
            else if( rb_node_has_parent(node) ){
                node->parent->color = RB_COLOR_BLACK;
                if( rb_node_has_grand_parent(node) ){
                    node->parent->parent->color = RB_COLOR_RED;
                    rb_node_left_rotate( T, node->parent->parent );
                }
            }
        }
    }
    T->root->color = RB_COLOR_BLACK;
}

int rb_insert( struct rb_tree* T, struct rb_node* node )
{
    if( T == NULL ) return 0;
    if( node == NULL ) return 0;

    struct rb_node* root = T->root;

    // insert
    if( root == NULL ){
        T->root = node;
        T->node_nr += 1;

        node->parent = NULL;
        node->left   = NULL;
        node->right  = NULL;
        node->color  = RB_COLOR_BLACK;

        node->key->max = node->key->entry.high;
    }else{
        struct rb_node* cur  = root;
        int cmp = rb_key_cmp( node->key, cur->key );
        while( !rb_node_is_leaf(cur) ){
            if( cmp == 0 ){
                printf("[WARNING] insert key [%ld,%ld] already exists\n", node->key->entry.low, node->key->entry.high );
                return 0;
            }
            if( cmp < 0 ){
                if( rb_node_has_left(cur) )
                    cur = cur->left;
                else  break;
                cmp = rb_key_cmp( node->key, cur->key );
            }
            else{
                if( rb_node_has_right(cur) )
                    cur = cur->right;
                else break;
                cmp = rb_key_cmp( node->key, cur->key );
            }
        }
        if( cmp < 0 ) cur->left = node;
        else cur->right = node;
        T->node_nr += 1;
        node->left = NULL;
        node->right = NULL;
        node->parent = cur;
        node->color = RB_COLOR_RED;
        // fixup
        node->key->max = node->key->entry.high;
        struct rb_node* node_ = node;
        while( !rb_node_is_root( T, cur ) ){ // cur == node_->parent
            cur->key->max = MAX( cur->key->max, node_->key->max );
            node_ = cur;
            cur = cur->parent;
        }
        cur->key->max = MAX( cur->key->max, node_->key->max );
    }

    // fixup
    rb_insert_fixup( T, node );

    return 1;
}

/*
 * replace old_ with new_
 *
 * new_ maybe NULL
 *
 */
void rb_transplant(
        struct rb_tree* T,
        struct rb_node* old_,
        struct rb_node* new_)
{
    if( rb_node_has_parent(old_) != 1 ){
        T->root = new_;
    }
    else if( rb_node_is_left(old_) ){
        old_->parent->left = new_;
    }else if( rb_node_is_right(old_) ){
        old_->parent->right = new_;
    }
    if( new_ != NULL ){
        new_->parent = old_->parent;
    }
}

void rb_delete_fixup(
        struct rb_tree* T,
        struct rb_node* fix )
{
    while( fix != T->root && fix->color == RB_COLOR_BLACK ){
        if( rb_node_is_left(fix) > 0 ){
            struct rb_node* bro = fix->parent->right;
            // case 1 : bro is red
            if( rb_node_is_red( bro ) ){// case 1 : goto case 2\3\4
                bro->color = RB_COLOR_BLACK;
                fix->parent->color = RB_COLOR_RED;
                rb_node_left_rotate( T, fix->parent );
                bro = fix->parent->right;
            }
            //case 2\3\4 : bro is black
            if( ( (rb_node_has_left(bro) &&rb_node_is_black(bro->left) ) || !rb_node_has_left(bro)  ) &&
                    ( (rb_node_has_right(bro)&&rb_node_is_black(bro->right)) || !rb_node_has_right(bro) ) ){
                // case 2 : bro's left & right are both BLACK
                bro->color = RB_COLOR_RED;
                fix = fix->parent;
            }
            else{
                if( (rb_node_has_right(bro)&&rb_node_is_black(bro->right)) || !rb_node_has_right(bro) ){
                    // case 3 : bro's right is black . goto case 4
                    if( rb_node_has_left(bro) ){
                        bro->left->color = RB_COLOR_BLACK;
                        bro->color = RB_COLOR_RED;
                        rb_node_right_rotate( T, bro );
                        bro = fix->parent->right;
                    }
                }
                // case 4 : bro's right is red
                bro->color = fix->parent->color;
                fix->parent->color = RB_COLOR_BLACK;
                if( rb_node_has_right(bro) ){
                    bro->right->color = RB_COLOR_BLACK;
                }
                rb_node_left_rotate( T, fix->parent );
                fix = T->root;
            }
        }
        else if( rb_node_is_right(fix) > 0 ){
            struct rb_node* bro = fix->parent->left;
            // case 1 : bro is red
            if( rb_node_is_red( bro ) ){// case 1 : goto case 2\3\4
                bro->color = RB_COLOR_BLACK;
                fix->parent->color = RB_COLOR_RED;
                rb_node_right_rotate( T, fix->parent );
                bro = fix->parent->left;
            }
            //case 2\3\4 : bro is black
            if( ( (rb_node_has_left(bro) &&rb_node_is_black(bro->left) ) || !rb_node_has_left(bro)  ) &&
                    ( (rb_node_has_right(bro)&&rb_node_is_black(bro->right)) || !rb_node_has_right(bro) ) ){
                // case 2 : bro's left & right are both BLACK
                bro->color = RB_COLOR_RED;
                fix = fix->parent;
            }
            else{
                if( (rb_node_has_left(bro)&&rb_node_is_black(bro->left)) || !rb_node_has_left(bro) ){
                    // case 3 : bro's left is black . goto case 4
                    if( rb_node_has_right(bro) ){
                        bro->right->color = RB_COLOR_BLACK;
                        bro->color = RB_COLOR_RED;
                        rb_node_left_rotate( T, bro );
                        bro = fix->parent->left;
                    }
                }
                // case 4 : bro's left is red
                bro->color = fix->parent->color;
                fix->parent->color = RB_COLOR_BLACK;
                if( rb_node_has_left(bro) ){
                    bro->left->color = RB_COLOR_BLACK;
                }
                rb_node_right_rotate( T, fix->parent );
                fix = T->root;
            }
        }
    }
    fix->color = RB_COLOR_BLACK;
}

int rb_delete( struct rb_tree* T, RB_NODE_KEY_TYPE key )
{
    if( T == NULL ) return 0;
    if( T->root == NULL ) return 0;

    struct rb_node* node = rb_find_node( T, key );
    if( node == NULL ){
        printf("[WARNING] delele key [%ld,%ld] not exist\n", key->entry.low, key->entry.high );
        return 0;
    }

    struct rb_node* _node = node;
    int _node_color = _node->color;

    struct rb_node* node_fix = NULL;
    if( !rb_node_has_left(node) ){
        node_fix = node->right;
        if( node_fix == NULL ){
            node_fix = rb_node_alloc();
            rb_node_set_key( node_fix, RB_KEY_NONE );
            node_fix->color = RB_COLOR_BLACK;
            node_fix->parent = node->parent;

            node->right = node_fix;
        }
        rb_transplant( T, node, node->right );
    }
    else if( !rb_node_has_right(node) ){
        node_fix = node->left;
        if( node_fix == NULL ){
            node_fix = rb_node_alloc();
            rb_node_set_key( node_fix, RB_KEY_NONE );
            node_fix->color = RB_COLOR_BLACK;
            node_fix->parent = node->parent;

            node->left = node_fix;
        }
        rb_transplant( T, node, node->left );
    }else{
        _node = node->right;
        while( rb_node_has_left(_node) > 0 ){
            _node = _node->left;
        }// _node is minium of node
        _node_color = _node->color;
        node_fix = _node->right;
        if( node_fix == NULL ){
            node_fix = rb_node_alloc();
            rb_node_set_key( node_fix, RB_KEY_NONE );
            node_fix->color = RB_COLOR_BLACK;
            node_fix->parent = _node;

            _node->right = node_fix;
        }
        if( _node->parent == node ){
            node_fix->parent = _node;
        }
        else{
            rb_transplant( T, _node, _node->right );
            if( rb_node_has_right(_node) ){
                _node->right = node->right;
                _node->right->parent = _node;
            }
        }
        rb_transplant( T, node, _node );
        _node->left = node->left;
        if( rb_node_has_left(_node) ){
            _node->left->parent = _node;
        }
        _node->color = node->color;
        _node->key->max = node->key->max;
    }

    // fixup max
    struct rb_node* node_fix_max = node_fix->parent;
    while( !rb_node_is_root( T, node_fix_max ) ){
        rb_node_recover_key( node_fix_max );
        node_fix_max = node_fix_max->parent;
    }
    rb_node_recover_key( node_fix_max );

    // fixup rb tree
    if( _node_color == RB_COLOR_BLACK ){
        rb_delete_fixup( T, node_fix );
    }

    if( node_fix->key == RB_KEY_NONE ){
        if( rb_node_has_parent(node_fix) ){
            if( rb_node_is_left(node_fix) ){
                node_fix->parent->left = NULL;
            }
            else if( rb_node_is_right(node_fix) ){
                node_fix->parent->right = NULL;
            }
        }
        rb_node_free( &node_fix , 0 );
    }

    return 1;
}

struct rb_node* interval_search(
    struct rb_tree* T,
    RB_NODE_KEY_TYPE key )
{
    if( T == NULL ) return NULL;

    struct rb_node* node = T->root;
    if( node == NULL ) return NULL;

    while( node != NULL && !rb_key_overlap( key, node->key ) ){
        if( rb_node_has_left(node) == 1 &&
            node->left->key->max >= key->entry.low ){
            node = node->left;
        }
        else{
            node = node->right;
        }
    }

    return node;
}

struct rb_node* rb_find_node( struct rb_tree* T,  RB_NODE_KEY_TYPE key )
{
    if( T == NULL ) return NULL;
    if( T->root == NULL ) return NULL;

    struct rb_node* ret = NULL;
    struct rb_node* current = T->root;
    while( ret == NULL ){
        if( current == NULL ) break;

        int cmp = rb_key_cmp( key, current->key );
        if( cmp == 0 ){
            ret = current;
        }
        else{
            if( cmp < 0 ){
                current = current->left;
            }
            else{ // cmp > 0
                current = current->right;
            }
        }
    }

    return ret;
}

/*
 * print rb tree
 */

void rb_color_pr( struct rb_node* node )
{
    if( node == NULL ) return;

    switch( node->color ){
        case RB_COLOR_NONE:  printf("NONE");    break;
        case RB_COLOR_BLACK: printf("BLACK");   break;
        case RB_COLOR_RED:   printf("RED");     break;
        default:             printf("WHAT???"); break;
    }

}

void rb_node_pr( struct rb_node* node , int if_pr_child )
{
    if( node == NULL ) return;

    printf("----- NODE -----\n");
    printf("Address = %lx\n",(size_t)node );
    printf("Key = ");   rb_key_pr( node->key ); printf("\n");
    printf("Color = "); rb_color_pr( node );    printf("\n");
    printf("Left   = %lx\n", (size_t)node->left   );
    printf("Right  = %lx\n", (size_t)node->right  );
    printf("Parent = %lx\n", (size_t)node->parent );
    printf("----------------\n");

    if( if_pr_child ){
        rb_node_pr( node->left  , if_pr_child );
        rb_node_pr( node->right , if_pr_child );
    }
}

void rb_key_pr( RB_NODE_KEY_TYPE key )
{
    printf("[ %ld, %ld; %ld ]", key->entry.low, key->entry.high, key->max );
}

void rb_tree_pr( struct rb_tree* T )
{
    if( T == NULL ){
        printf("There is no rb_tree\n");
        return;
    }
    if( T->root == NULL ){
        printf("There is no root\n");
        return;
    }

    printf("====== RB TREE =====\n");
    printf("Node nr = %lu\n", T->node_nr );
    rb_node_pr( T->root , 1 );
    printf("====================\n");
}

/*
 * check if rb_tree is valid
 *
 */

int rb_node_is_leaf( struct rb_node* node )
{
    if( node == NULL ) return -1;
    if( node->left == NULL && node->right == NULL ) return 1;
    return 0;
}

int rb_node_has_left( struct rb_node* node )
{
    if( node == NULL ) return -1;
    if( node->left != NULL) return 1;
    else return 0;
}

int rb_node_has_right( struct rb_node* node )
{
    if( node == NULL ) return -1;
    if( node->right != NULL ) return 1;
    else return 0;
}

int rb_node_has_parent( struct rb_node* node )
{
    if( node == NULL ) return -1;
    if( node->parent != NULL ) return 1;
    else return 0;
}

int rb_node_is_black( struct rb_node* node )
{
    if( node == NULL ) return -1;
    if( node->color == RB_COLOR_BLACK ) return 1;
    else return 0;
}

int rb_node_is_red( struct rb_node* node )
{
    if( node == NULL ) return -1;
    if( node->color == RB_COLOR_RED ) return 1;
    else return 0;
}

int rb_node_check_max(
        struct rb_node* node,
        int if_check_child )
{
    if( node == NULL ) return 1;

    int ret = 1;

    ssize_t max = node->key->max;
    if( rb_node_has_left(node) == 1 ){
        max = MAX( max, node->left->key->max );
        if( if_check_child )
            ret &= rb_node_check_max( node->left  , 1 );
    }
    if( rb_node_has_right(node) == 1 ){
        max = MAX( max, node->right->key->max );
        if( if_check_child )
            ret &= rb_node_check_max( node->right , 1 );
    }
    if( max != node->key->max ){
        return 0;
    }
    return 1;
}

int rb_node_check_color(
        struct rb_node* node,
        int if_check_child )
{
    if( node == NULL ) return 1;

    int ret = 1;

    if( rb_node_is_red(node) > 0 ){
        if( rb_node_has_left( node) > 0  )
            ret &= rb_node_is_black(node->left );
        if( rb_node_has_right(node) > 0 )
            ret &= rb_node_is_black(node->right);
    }

    if( if_check_child ){
        if( rb_node_has_left( node) > 0 )
            ret &= rb_node_check_color( node->left , 1 );
        if( rb_node_has_right(node) > 0 )
            ret &= rb_node_check_color( node->right, 1);
    }

    return ret;
}

int rb_tree_check_black_hight_get_hight(
        struct rb_tree* T,
        struct rb_node* node )
{
    size_t hight = 0;
    while( rb_node_is_root(T,node) != 1 ){
        if( node->color == RB_COLOR_BLACK ){
            hight += 1;
        }
        node = node->parent;
    }
    return hight + 1;
}

size_t saved_hight = 0;

int rb_tree_check_black_hight_help(
        struct rb_tree* T,
        struct rb_node* node )
{
    if( !rb_node_is_leaf(node) ){
        int ret = 1;
        if( rb_node_has_left(node) > 0 ){
            ret &= rb_tree_check_black_hight_help(T,node->left);
        }
        if( rb_node_has_right(node) > 0 ){
            ret &= rb_tree_check_black_hight_help(T,node->right);
        }
        return ret;
    }
    else{
        size_t hight = rb_tree_check_black_hight_get_hight(T,node);
        if( saved_hight == 0 ){
            saved_hight = hight;
            return 1;
        }
        else{
            if( saved_hight != hight ){
                return 0;
            }
            else{
                return 1;
            }
        }
    }
}

int rb_tree_check_black_hight( struct rb_tree* T )
{
    if( T != NULL && T->root != NULL ){
        saved_hight  = 0;
        int ret = rb_tree_check_black_hight_help(T,T->root);
        return ret;
    }
    return 1;
}

int rb_tree_check( struct rb_tree* T )
{
    if( T == NULL ) return 1;
    if( T->root == NULL ){
        if( T->node_nr == 0 ){
            return 2;
        }
        else{
            return 3;
        }
    }

    struct rb_node* node = T->root;

    int check_color = rb_node_check_color( node , 1 );
    check_color &= T->root->color == RB_COLOR_BLACK;
    if( check_color != 1 ){
        printf("[Check] wrong color\n");
        rb_tree_pr( T );
        return 4;
    }else{
        printf("[OK] color is good\n");
    }

    int check_hight = rb_tree_check_black_hight( T );
    if( check_hight != 1 ){
        printf("[Check] wrong hight\n");
        rb_tree_pr( T );
        return 5;
    }else{
        printf("[OK] hight = %lu\n", saved_hight );
        saved_hight = 0;
    }

    int check_max = rb_node_check_max( node, 1 );
    if( check_max != 1 ){
        printf("[Check] wrong max\n");
        rb_tree_pr( T );
        return 6;
    }
    else{
        printf("[OK] good max\n");
    }

    return 0;
}
