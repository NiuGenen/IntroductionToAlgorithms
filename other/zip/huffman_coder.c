#include "huffman_tree.h"
#include "bb.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdlib.h>

#ifndef NULL 
#define NULL 0
#endif

#define _MALLOC_(t,n) ((t*)malloc(sizeof(t)*(n)))
#define _FREE_(p) do{\
    if( p != NULL ){ \
        free(p);     \
        p = NULL;    \
    }                \
}while(0);

struct huffman_coder_head{
    size_t char_nr;
    uint32_t encode_type;
};

#define OK 0
#define ERROR_OPEN_FILE_IN  1
#define ERROR_OPEN_FILE_OUT 2
#define WARING_DECODE_END   3

char error_info[256];

const char options_short[] = "hevdi:o:t:";

const struct option options_long[] = {
    {"encode" ,  0,  NULL,  'e' },
    {"decode" ,  0,  NULL,  'd' },
    {"help"   ,  0,  NULL,  'h' },
    {"input"  ,  1,  NULL,  'i' },
    {"output" ,  1,  NULL,  'o' },
    {"type"   ,  1,  NULL,  't' },
    {"verbose",  0,  NULL,  'v' }
};

void help()
{
    printf( "Usage: coder [OPTION...] [PARAMETER...]\n"                    );
    printf( "Huffman Coder v1.0 (un)compress one file\n"                   );
    printf( "\n"                                                           );
    printf( "Examples:\n"                                                  );
    printf( "  coder -e -i ~/inputfile.txt -o outputfilename -t dynamic\n" );
    printf( "\n"                                                           );
    printf( " Options:\n"                                                  );
    printf( "\n"                                                           );
    printf( "   -h      get help information\n"                            );
    printf( "   -v      whether print verbose info\n"                      );
    printf( "   -e      compress file_in into file_out\n"                  );
    printf( "   -d      uncompress file_in into file_out\n"                );
    printf( "   -i      input file to compress\n"                          );
    printf( "   -o      output file after compress\n"                      );
    printf( "   -t      huffman code type, 'dynamic' or 'static'\n"        );
    printf( "           default = 'dynamic'\n"                             );
    printf( "\n"                                                           );
    printf( "Report Bug to <602131568@qq.com>\n"                           );
}

int op_dynamic = 1;

int check_type( char * type )
{
    if( type == NULL ) return 0;
    // "dynamic" or "static"
    int len = strlen( type );
    if( len != 7 && len != 6 ) return 2;
    if( len == 7 ){
        const char dy[] = "dynamic";
        for(int i = 0; i < 7; ++i){
            if( type[i] != dy[i] ) return 3;
        }
    }else{
        const char st[] = "static";
        for(int i = 0; i < 6; ++i){
            if( type[i] != st[i] ) return 4;
        }
        op_dynamic = 0;
    }
    return 0;
}

int verbose = 0;

int huffman_decode( char * file_in, char * file_out )
{
    printf("Huffman Decodeing...\n");
    // open input file
    int fd_in = open( file_in, O_RDONLY, 0666 );
    if( fd_in < 0 ){
        sprintf( error_info, "[ERROR] fail to open input file : %s\n", file_in );
        return ERROR_OPEN_FILE_IN;
    }
    struct huffman_coder_head * head = _MALLOC_( struct huffman_coder_head, 1 );
    read( fd_in, head, sizeof(struct huffman_coder_head) );
    printf("The number of char is %lu\n", head->char_nr );
    struct bb_rw * bb_in  = bb_rw_alloc( fd_in, 1, 1, 4096 ); // read bit
    // open output file
    int fd_out = open( file_out, O_CREAT | O_WRONLY, 0666 );
    if( fd_out < 0 ){
        sprintf( error_info, "[ERROR] fail to open output file : %s\n", file_out );
        return ERROR_OPEN_FILE_OUT;
    }
    struct bb_rw * bb_out = bb_rw_alloc( fd_out, 0, 0, 4096 ); // write byte
    // alloc huffman tree
    struct huffman_tree * h = huffman_tree_alloc( head->encode_type );
    // vars to use
    uint8_t code[256];
    int     code_l = 0;
    uint8_t c = 0;
    size_t  count = 0;
    // dynamic huffman tree
    if( head->encode_type == 1 ){
        // first to read : 'empry code' and 'char bits'
        code_l = 0;
        code[ code_l++ ] = bb_in->rd_bit( bb_in ) + '0';
        code[ code_l ] = '\0';
        if( verbose ) printf("EM:%s\n", code);
        // char
        code_l = 0;
        for(int i = 0; i < 8; ++i){
            code[ code_l ] = bb_in->rd_bit( bb_in ) + '0';
            c = c | (((code[code_l]-'0')&1)<<i);
            code_l += 1;
        }
        code[ code_l ] = '\0';
        if( verbose ){
            for(int i = 0; i < 8; ++i)
                printf("%d", ( c >> i ) & 1 );
            printf(" '%3d' :%s\n", c, code);
        }
        h->add(h,c);
        bb_out->wr_byte( bb_out, c );
        count += 1;
        // loop to decode
        struct huffman_tree_node * node = h->root;
        while( !(bb_in->rd_end) && count < head->char_nr ){
            // read bit
            int bit = bb_in->rd_bit( bb_in );
            // move through huffman_tree
            if( bit == 0 ){ // go left
                node = node->left;
            }else{ // go right
                node = node->right;
            }
            // reach NULL node
            if( node == NULL ){
                sprintf( error_info, "[WARING] can not decode this file OR reach the end\n");
                return WARING_DECODE_END;
            }
            if( node->is_empty_node ){
                count += 1;
                if( verbose ) printf("New char found!\n");
                c = 0;
                code_l = 0;
                for(int i = 0; i < 8; ++i){
                    int bit = bb_in->rd_bit( bb_in );
                    code[ code_l ] = bit + '0';
                    c = c | (((code[code_l]-'0')&1)<<i);
                    code_l += 1;
                }
                code[ code_l ] = '\0';
                if( verbose ){
                    for(int i = 0; i < 8; ++i)
                        printf("%d", ( c >> i ) & 1 );
                    printf(" '%3d' :%s\n", c, code);
                }
                bb_out->wr_byte( bb_out, c );
                // restart
                h->add(h,c);
                node = h->root;
            }else if( node->is_leaf ){ // decode one char
                count += 1;
                if( verbose ) printf("Decode char!\n");
                bb_out->wr_byte( bb_out, node->c );
                c = node->c;
                if( verbose ){
                    printf("'%3d' : ", c);
                    for(int i = 0; i < 8; ++i)
                        printf("%d", ( c >> i ) & 1 );
                    printf("\n");
                }
                h->add(h,c);
                node = h->root;
            }
        }
        printf("Decode over!\n");
    }
    else{
        printf("Static is Not Implement!\n");
    }
    // flush output file
    bb_out->flush( bb_out );
    // free resources
    bb_rw_free( bb_in );
    bb_rw_free( bb_out );
    close( fd_in );
    close( fd_out );
    return OK;
}

int huffman_encode( char * file_in, char * file_out , int dynamic )
{
    printf("Huffman Encoding......\n");
    // open input file
    int fd_in = open( file_in, O_RDONLY, 0666 );
    if( fd_in < 0 ){
        sprintf( error_info, "[ERROR] fail to open input file : %s\n", file_in );
        return ERROR_OPEN_FILE_IN;
    }
    struct bb_rw * bb_in  = bb_rw_alloc( fd_in, 1, 0, 4096 ); // read byte
    // open output file
    int fd_out = open( file_out, O_CREAT | O_WRONLY, 0666 );
    if( fd_out < 0 ){
        sprintf( error_info, "[ERROR] fail to open output file : %s\n", file_out );
        return ERROR_OPEN_FILE_OUT;
    }
    lseek( fd_out, sizeof(struct huffman_coder_head), SEEK_SET );// leave space to set the number of char
    struct huffman_coder_head * head = _MALLOC_( struct huffman_coder_head, 1 );
    struct bb_rw * bb_out = bb_rw_alloc( fd_out, 0, 1, 4096 ); // write bit
    // alloc huffman tree
    struct huffman_tree * h = huffman_tree_alloc( dynamic );
    // vars that will be used
    uint8_t code[256];
    uint16_t c = 0;
    int code_l = 0;
    // dynamic huffman encode
    if( dynamic ){
        // for first char : output 'empty code' + 'char bits'
        c = bb_in->rd_byte( bb_in );
        h->add( h, c );
        head->char_nr += 1;
        // output 'empty code'
        code_l = h->empty_code( h, code );
        if( verbose ) printf("EM:");
        for( int i = 0; i < code_l; ++i ){
            bb_out->wr_bit( bb_out, code[i] );
            if( verbose ) printf("%c", code[i] );
        }
        if( verbose ) printf("\n");
        // output 'char bits'
        if( verbose ) printf("New Char : '%3d' . Bits = ", c);
        for( int i = 0; i < 8; ++i ){
            int bit = ( c >> i ) & 1;
            bb_out->wr_bit( bb_out, bit );
            if( verbose ) printf("%c", bit + '0' );
        }
        if( verbose ) printf("\n");
        // for each char input
        while( !( bb_in->rd_end ) ){
            c = bb_in->rd_byte( bb_in );
            head->char_nr += 1;
            // if this char NOT exist in huffman_tree
            if( !h->exist( h, c ) ){
                // 'empty code'
                code_l = h->empty_code( h, code );
                if( verbose ) printf("EM:");
                for( int i = 0; i < code_l; ++i ){
                    bb_out->wr_bit( bb_out, code[i] );
                    if( verbose ) printf("%c", code[i] );
                }
                if( verbose ) printf("\n");
                // char bits
                if( verbose ) printf("New Char : '%3d' . Bits = ", c);
                for( int i = 0; i < 8; ++i ){
                    int bit = ( c >> i ) & 1;
                    bb_out->wr_bit( bb_out, bit );
                    if( verbose ) printf("%c", bit + '0' );
                }
            }
            // if this char exist in huffman_tree
            else{
                // 'encode'
                if( verbose ) printf("Encode Char : ");
                code_l = h->encode( h, c, code );
                if( verbose ) printf("%c . code = ", c);
                for( int i = 0; i < code_l; ++i ){
                    bb_out->wr_bit( bb_out, code[i] );
                    if( verbose ) printf("%c", code[i] );
                }
            }
            // add this char into huffman_tree
            h->add( h , c );
            if( verbose ) printf("\n");
        }
        // flush output file
        bb_out->flush( bb_out );
        // output the number of char at the first of output file
        lseek( fd_out, 0, SEEK_SET );
        head->encode_type = 1; // dynamic huffman encode
        write( fd_out, head, sizeof(struct huffman_coder_head) );
    }
    else{
        printf("Static is Not Implement!\n");
    }
    printf("Encode over!\n");
    printf("The number of char is %lu\n", head->char_nr );
    // free resources
    bb_rw_free( bb_in );
    bb_rw_free( bb_out );
    close( fd_in );
    close( fd_out );
    return OK;
}

int main( int argc, char *argv[] )
{
    int  op          = 0;
    int  need_help   = 0;
    char * file_in   = NULL;
    char * file_out  = NULL;
    char * type      = NULL;
    int  encode      = 0;
    int  decode      = 0;
    while( ( op = getopt_long( argc, argv, options_short, options_long, NULL ) ) != -1 ){
        switch( op ){
            default:
            case 'h': need_help = 1;       break;
            case 'e': encode = 1;          break;
            case 'd': decode = 1;          break;
            case 'i': file_in   = optarg;  break;
            case 'o': file_out  = optarg;  break;
            case 't': type      = optarg;  break;
            case 'v': verbose   = 1;       break;
        }
    }

    if( need_help            ||
            argc < 3          ||
            file_in  == NULL   ||
            file_out == NULL    ||
            encode == decode     ||
            check_type(type) != 0 ){
        help();
        return 0;
    }

    printf("Encode File : %s\n", file_in );
    printf("Output File : %s\n", file_out);

    int ret = 0;
    if( encode ){
        ret = huffman_encode( file_in, file_out , op_dynamic );
    }
    else{
        ret = huffman_decode( file_in, file_out );
    }
    if( ret != 0 ){
        perror( error_info );
    }

    return ret;
}
