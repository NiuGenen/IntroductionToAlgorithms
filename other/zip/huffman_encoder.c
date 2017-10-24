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
    struct bb_rw * bb_in  = bb_rw_alloc( fd_in, 1, 0, 4096 ); // read byte

    int fd_out = open( file_out, O_CREAT | O_WRONLY, 0666 );
    if( fd_out < 0 ){
        printf("[ERROR] fail to open output file : %s\n", file_out );
        return -3;
    }
    lseek( fd_out, sizeof(size_t), SEEK_SET );
    size_t char_nr = 0;
    struct bb_rw * bb_out = bb_rw_alloc( fd_out, 0, 1, 4096 ); // write bit

    struct huffman_tree * h = huffman_tree_alloc( 1 );

    // first : empry + char
    uint8_t code[256];
    uint16_t c = bb_in->rd_byte( bb_in );
    h->add( h, c );
    char_nr += 1;

    int code_l = h->empty_code( h, code );
    printf("EM:");
    for( int i = 0; i < code_l; ++i ){
        bb_out->wr_bit( bb_out, code[i] );
        printf("%c", code[i] );
    }
    printf("\n");

    printf("New Char : '%3d' . Bits = ", c);
    for( int i = 0; i < 8; ++i ){
        int bit = ( c >> i ) & 1;
        bb_out->wr_bit( bb_out, bit );
        printf("%c", bit + '0' );
    }
    printf("\n");

    while( !( bb_in->rd_end ) ){
        c = bb_in->rd_byte( bb_in );
        char_nr += 1;
        if( !h->exist( h, c ) ){
            // empty + char
            code_l = h->empty_code( h, code );
            printf("EM:");
            for( int i = 0; i < code_l; ++i ){
                bb_out->wr_bit( bb_out, code[i] );
                printf("%c", code[i] );
            }
            printf("\n");
            printf("New Char : '%3d' . Bits = ", c);
            for( int i = 0; i < 8; ++i ){
                int bit = ( c >> i ) & 1;
                bb_out->wr_bit( bb_out, bit );
                printf("%c", bit + '0' );
            }
        }
        else{
            // encode
            printf("Encode Char : ");
            code_l = h->encode( h, c, code );
            printf("%c . code = ", c);
            for( int i = 0; i < code_l; ++i ){
                bb_out->wr_bit( bb_out, code[i] );
                printf("%c", code[i] );
            }
        }
        // then add
        h->add( h , c );
        printf("\n");
    }

    bb_out->flush( bb_out );

    lseek( fd_out, 0, SEEK_SET );
    write( fd_out, &char_nr, sizeof(size_t) );

    bb_rw_free( bb_in );
    bb_rw_free( bb_out );
    close( fd_in );
    close( fd_out );
    return 0;
}
