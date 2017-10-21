#include "huffman_tree.h"

uint8_t data[] = "asdasduytqwdyujvdguyughxzbvxguyiuqwkjhbfvweiudsjkhan";

int main()
{
    struct huffman_tree * h = huffman_tree_alloc( 1 );
    
    size_t size = sizeof(data) - 1;
    for( size_t i = 0; i < size; ++i ){
        h->add( h, data[i] );
    }

    h->pr( h );
    
    return 0;
}
