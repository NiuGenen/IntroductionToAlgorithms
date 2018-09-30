#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef NULL
#define NULL 0
#endif

#define _MALLOC_(t,n) ( (t*)malloc(sizeof(t)*(n)) )
#define _FREE_(p) do{\
    if( p != NULL ){\
        free(p); \
        p = NULL; \
    }\
}while(0);

#define _MIN_(x,y) ( ((x)>(y))?(y):(x) )
#define _MAX_(x,y) ( ((x)>(y))?(x):(y) )

const char options_short[] = "hevdi:o:a:b:";

const struct option options_long[] = {
    {"encode"    ,  0,  NULL,  'e' },
    {"decode"    ,  0,  NULL,  'd' },
    {"help"      ,  0,  NULL,  'h' },
    {"input"     ,  1,  NULL,  'i' },
    {"output"    ,  1,  NULL,  'o' },
    {"verbose"   ,  0,  NULL,  'v' },
    {"ahead"     ,  1,  NULL,  'a' },
    {"back"      ,  1,  NULL,  'b' }
};

void help()
{
    printf( "Usage: lz77 [OPTION...] [PARAMETER...]\n"                     );
    printf( "Lz77 v1.0 (un)compress one file\n"                            );
    printf( "\n"                                                           );
    printf( "Examples:\n"                                                  );
    printf( "  lz77 -e -i ~/inputfile.txt -o outputfilename\n"             );
    printf( "\n"                                                           );
    printf( " Options:\n"                                                  );
    printf( "\n"                                                           );
    printf( "   -h      get help information\n"                            );
    printf( "   -v      whether print verbose info\n"                      );
    printf( "   -e      compress   file_in into file_out\n"                );
    printf( "   -d      uncompress file_in into file_out\n"                );
    printf( "   -i      input file to compress\n"                          );
    printf( "   -o      output file after compress\n"                      );
    printf( "   -a      look ahead window size. default = 256\n"           );
    printf( "   -b      look back  window size. default = 256\n"           );
    printf( "\n"                                                           );
    printf( "Report Bug to <602131568@qq.com>\n"                           );
}

int verbose    = 0 ;
size_t ahead_s = 0 ;
size_t back_s  = 0 ;

uint8_t lz_back_buf[256];
uint8_t lz_back_idx  = 0;
uint8_t lz_back_size = 0;

void _lz77_read_back_buf( int fd_in )
{
    lz_back_size = read( fd_in, lz_back_buf, 256 );
    lz_back_idx = 0;
}

void _lz77( int fd_in, int fd_out )
{
    size_t buf_size  = ( ahead_s + back_s ) * 2 ;
    uint8_t * buf    = _MALLOC_( uint8_t, buf_size );
    size_t rd_size   = read( fd_in, buf, buf_size );

    size_t w_st = 0;
    size_t w_ed = w_st + ahead_s + 1;
    size_t d_st = 0;
    size_t d_ed = 0;
    //
    // dict   = [0][1][2]
    // window = [3][4][5][6]
    //
    // [0]  [1]  [2]  [3]  [4]  [5]  [6]  [7] [8] [9]
    //  |-----dict-----|------window-------|
    // d_st           d_ed                 |
    //                w_st                w_ed
    //
    
    size_t offset;
    size_t length;
    uint8_t chara;
    while( rd_size > 0 ){
        if( d_st == d_ed ){
            offset = 0;
            length = 0;
            chara = buf[ w_st ];
        }else{
            size_t t_idx = d_st;
            while( t_idx < d_ed ){
                if( buf[t_idx] == buf[w_st] ){
                    //search
                }
                t_idx += 1;
            }
        }
        d_ed += length + 1;
        w_st += length + 1;
        w_ed += length + 1;
        if( d_ed - d_st > back_s ) d_st = d_ed - back_s;
        if( w_ed > buf_size ){
            // move data and read
        }
    }
}

void _77zl( int fd_in, int fd_out )
{

}

int main( int argc, char * argv[] )
{
    int op           = 0    ;
    int  need_help   = 0    ;
    char * file_in   = NULL ;
    char * file_out  = NULL ;
    int  encode      = 0    ;
    int  decode      = 0    ;
    while( ( op = getopt_long( argc, argv, options_short, options_long, NULL ) ) != -1 ){
        switch( op ){
            default:
            case 'h': need_help = 1;       break;
            case 'e': encode = 1;          break;
            case 'd': decode = 1;          break;
            case 'i': file_in   = optarg;  break;
            case 'o': file_out  = optarg;  break;
            case 'v': verbose   = 1;       break;
            case 'a': ahead_s = atoi(optarg); break;
            case 'b': back_s  = atoi(optarg); break;
        }
    }

    if( need_help           ||
            argc < 3         ||
            file_in  == NULL  ||
            file_out == NULL   ||
            encode == decode    ){
        help();
        return 0;
    }

    if( verbose ) printf("[INFO] verbose on\n");

    printf("Input  File : %s\n", file_in );
    printf("Output File : %s\n", file_out);

    int lz_fd_in   = -1 ;
    int lz_fd_out  = -1 ;

    lz_fd_in = open( file_in, O_RDONLY , 0666 );
    if( lz_fd_in < 0 ){
        perror("[ERROR] fail to open input file\n");
        goto _LZ77_OUT_;
    }

    lz_fd_out = open( file_out, O_WRONLY | O_CREAT, 0666 );
    if( lz_fd_out < 0 ){
        perror("[ERROR] fail to open output file\n");
        goto _LZ77_OUT_;
    }

    if( ahead_s == 0 ){
        printf("Using default look ahead window size : 256\n");
        ahead_s = 256;
    }
    else printf("Set Look Ahead Window Size = %lu\n", ahead_s );

    if( back_s == 0 ){
        printf("Using default look back window size : 256\n");
        back_s = 256;
    }
    else printf("Set Look Back  Window Size = %lu\n", back_s );

    if( encode )      _lz77( lz_fd_in, lz_fd_out );
    else if( decode ) _77zl( lz_fd_in, lz_fd_out );

_LZ77_OUT_:
    if( lz_fd_in  > 0 ) close( lz_fd_in  );
    if( lz_fd_out > 0 ) close( lz_fd_out );
    return 0;
}
