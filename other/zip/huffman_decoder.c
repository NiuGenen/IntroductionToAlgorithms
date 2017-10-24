#include "huffman_tree.h"
#include "bb.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main( int argc, char *argv[] )
{
    if( argc < 3 ) return -1;

    const char * file_in  = argv[1];
    const char * file_out = argv[2];
    printf("Encode File : %s\n", file_in );
    printf("Output File : %s\n", file_out);

    int fd_in = open( file_in, O_RDONLY, 0666 );
    if( fd_in < 0 ){
        printf("[ERROR] fail to open input file : %s\n", file_in );
        return -2;
    }
    size_t char_nr = 0;
    read( fd_in, &char_nr, sizeof(size_t) );
    struct bb_rw * bb_in  = bb_rw_alloc( fd_in, 1, 1, 4096 ); // read bit

    int fd_out = open( file_out, O_CREAT | O_WRONLY, 0666 );
    if( fd_out < 0 ){
        printf("[ERROR] fail to open output file : %s\n", file_out );
        return -3;
    }
    struct bb_rw * bb_out = bb_rw_alloc( fd_out, 0, 0, 4096 ); // write byte

    struct huffman_tree * h = huffman_tree_alloc( 1 );

    // first : empry + char
    uint8_t code[256];
    int code_l = 0;
    uint8_t c = 0;

    size_t count = 0;
    // EM
    code_l = 0;
    code[ code_l++ ] = bb_in->rd_bit( bb_in ) + '0';
    code[ code_l ] = '\0';
    printf("EM:%s\n", code);
    // char
    code_l = 0;
    for(int i = 0; i < 8; ++i){
        code[ code_l ] = bb_in->rd_bit( bb_in ) + '0';
        c = c | (((code[code_l]-'0')&1)<<i);
        code_l += 1;
    }
    code[ code_l ] = '\0';
    for(int i = 0; i < 8; ++i) printf("%d", ( c >> i ) & 1 );
    printf(" '%3d' :%s\n", c, code);
    h->add(h,c);
    bb_out->wr_byte( bb_out, c );
    count += 1;

    struct huffman_tree_node * node = h->root;
    while( !(bb_in->rd_end) && count < char_nr ){
        int bit = bb_in->rd_bit( bb_in );
        if( bit == 0 ){ // go left
            node = node->left;
        }else{ // go right
            node = node->right;
        }
        if( node == NULL ){
            printf("[WARING] can not decode this file OR reach the end\n");
            break;
        }
        if( node->is_empty_node ){
            count += 1;
            printf("New char found!\n");
            c = 0;
            code_l = 0;
            for(int i = 0; i < 8; ++i){
                int bit = bb_in->rd_bit( bb_in );
                code[ code_l ] = bit + '0';
                c = c | (((code[code_l]-'0')&1)<<i);
                code_l += 1;
            }
            code[ code_l ] = '\0';
            for(int i = 0; i < 8; ++i)
                printf("%d", ( c >> i ) & 1 );
            printf(" '%3d' :%s\n", c, code);
            bb_out->wr_byte( bb_out, c );
            // restart
            h->add(h,c);
            node = h->root;
        }else if( node->is_leaf ){ // decode one char
            count += 1;
            printf("Decode char!\n");
            bb_out->wr_byte( bb_out, node->c );
            c = node->c;
            printf("'%3d' : ", c);
            for(int i = 0; i < 8; ++i)
                printf("%d", ( c >> i ) & 1 );
            printf("\n");
            h->add(h,c);
            node = h->root;
        }
    }
    printf("End decode!\n");

    bb_out->flush( bb_out );

    bb_rw_free( bb_in );
    bb_rw_free( bb_out );
    close( fd_in );
    close( fd_out );
    return 0;
}
