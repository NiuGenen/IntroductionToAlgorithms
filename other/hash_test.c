#include <stdio.h>
#include <stdlib.h>
#define HASH_FUNC_GLOBAL_VAL
#include "hash_func.h"
#include <stdint.h>

/*
 * hash_func( char*str, size_t n )
 * 
 * data before hash : 8 bit , 0 to 255
 * data after  hash : 8 bit , 0 to 255
 * 
 * let A belongs to [ 0, 255 ]
 * let B belongs to [ 0, 255 ]
 *
 * for 256 pairs of (A,B) in testing
 * let set SA as all the A in 256 pairs, that will be 0 to 255
 * let set SB as all the B in 256 pairs, that depends on hash_func
 * 
 * for all B in SB,
 * let n(k) as the number of B
 * that has k pairs of (A,B)
 * 
 * the bigger n(1) is,
 * which means the more B has only one pair of (A,B)
 * the better this hash_func is
 * 
 */ 

void hash_test( hash_fn_t hash_func );

int main()
{
    for( size_t i = 0; i < HASH_FUNC_NR; ++i ){
        printf("[TEST] %s\n", hash_func_name[i] );
        hash_test( hash_func_s[ i ] );
    }

    return 0;
}

void hash_test( hash_fn_t hash_func )
{
    uint8_t A[256], B[256];
    uint16_t B_nr[256] = {0}, n[256] = {0};
    for(int i = 0; i < 256; ++i){
        A[i] = i;
        B[i] = hash_func( (char*)A + i, 1 );
        B_nr[ B[i] ] += 1;
    }
    for(int i = 0; i < 256; ++i){
        n[ B_nr[i] ] += 1;
    }
    // print 
    printf("Value: before,after\n");
    for(int l = 0; l < 32; ++l){
        for(int c = 0; c < 8; ++c){
            printf("%3d,%3d | ", A[l*8+c], B[l*8+c]);
        }
        printf("\n");
    }
    printf("The number of same hash: hash,number\n");
    for(int l = 0; l < 32; ++l){
        int pr_line = 0;
        for(int c = 0; c < 8; ++c){
            if( B_nr[ l*8+c ] ){
                pr_line = 1;
                printf("%3d,%3d | ", l*8+c, B_nr[ B[l*8+c] ]);
            }
        }
       if( pr_line ) printf("\n");
    }
    printf("The number of the number of same hash: hash,number:number\n");
    for(int l = 0; l < 32; ++l){
        int pr_line = 0;
        for(int c = 0; c < 8; ++c){
            if( n[ l*8+c ] ){
                pr_line = 1;
                printf("%3d,%3d:%3d | ", l*8+c, B_nr[l*8+c], n[ l*8+c ]);
            }
        }
        if( pr_line ) printf("\n");
    }

}
