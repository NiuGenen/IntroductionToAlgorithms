#include "huffman.h"

uint8_t data[] = "weifgshdjgjdhwiuqfqwgjkshdvasiudjhansvfcgweyujhdsvuwjkgveqydusxhand";

int main()
{
    struct huffman_dynamic * dy = huffman_dynamic_alloc();
    
    size_t size = sizeof(data);
    for( size_t i = 0; i < size - 1; ++i ){
        dy->add( dy, data[i] );
    }
    dy->pr( dy );
}
