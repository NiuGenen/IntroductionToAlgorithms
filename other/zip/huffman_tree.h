#ifndef _HUFFMAN_TREE_H_
#define _HUFFMAN_TREE_H_

#include <stddef.h>
#include <stdint.h>

struct huffman_tree_counter{
    struct huffman_tree_node * node;
    size_t count;
};

struct huffman_tree_node{
    struct huffman_tree_node * parent;
    struct huffman_tree_node * left;
    struct huffman_tree_node * right;
    size_t count;
    int index; // index in sort[]
    uint8_t c;
    uint8_t code_in_byte[256]; // 8 * 32 = 256
    int code_length;
    uint8_t is_leaf;
    uint8_t is_empty_node; // dynamic huffman code
};

struct huffman_tree{
    struct huffman_tree_node * root;
    struct huffman_tree_node ** sort; // 256 * 2 - 1
    struct huffman_tree_counter ** counter; // [ char ]
    int dynamic;
    struct huffman_tree_node * empty_node;
    int n;
    // operations 
    int (*exist)( struct huffman_tree * h, uint8_t c );
    int (*empty_code)( struct huffman_tree * h, uint8_t * code_in_byte );
    int (*encode)( struct huffman_tree * h, uint8_t c, uint8_t * code_in_byte );
    uint16_t (*decode)( struct huffman_tree * h, uint8_t * code, int length );
    void (*add)(   struct huffman_tree * h, uint8_t c );
    void (*pr)( struct huffman_tree * h );
};

struct huffman_tree * huffman_tree_alloc( int dynamic );
void huffman_tree_free( struct huffman_tree * h );

#endif
