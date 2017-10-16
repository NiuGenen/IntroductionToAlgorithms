#include "big_int.h"
#include <stdlib.h>

/*
 * suppose n1 & n2 are normal
 * 
 * eg.  536735634523436534 
 *      -73478561278434232 : using [0] to store +/-
 *      
 * for number :  123456789
 * if mod == BIG_INT_0_MIN then n[0] = 1 , n[1] = 2 , ...
 * if mod == BIG_INT_0_MAX then n[0] = 9 , n[1] = 8 , ...
 * 
 * for number : -123456789
 * if mod == BIG_INT_0_MIN then n[0] = -1 , n[1] = 2 , ...
 * if mod == BIG_INT_0_MAX then n[0] = -9 , n[1] = 8 , ...
 * 
 * NOT: 000235486384
 *      ^^^
 *      
 * both using @n1 & @n2 to calculate 
 * 
 * store the result in @result 
 * 
 * return the length of @result
 * 
 */

#define _MAX_(x,y) ( ((x)>(y))?(x):(y) )
#define _MIN_(x,y) ( ((x)<(y))?(x):(y) )

#define _MALLOC_(t,n) ( (t*)malloc(sizeof(t)*(n)) )

size_t big_int_add( int * n1, size_t l1, int * n2, size_t l2, int ** result , int mod )
{
    int * ret = _MALLOC_( int, _MAX_(l1,l2) + 1 );
    size_t l = 0;

    int n = 0 , d = 0 ;
    if( mod == BIG_INT_0_MIN ){
        while( l < l1 && l < l2 ){
            n = n1[l] + n2[l] + d;    d  = 0;
            while( n > 9 ) { n -= 10; d += 1; } ret[l] = n ;
            l += 1;
        }
        while( l < l1 ){
            n = n1[l] + d;            d  = 0;
            while( n > 9 ) { n -= 10; d += 1; } ret[l] = n ;
            l += 1;
        }
        while( l < l2 ){
            n = n2[l] + d;            d  = 0;
            while( n > 9 ) { n -= 10; d += 1; } ret[l] = n ;
            l += 1;
        }
    }else{ // mod == BIG_INT_0_MAX
        while( l1 > 0 && l2 > 0 ){
            n = n1[l1-1] + n2[l2-1] + d; d = 0;
            while( n > 9 ) {  n -= 10;   d += 1; } ret[l] = n ;
            l += 1; l1 -= 1; l2 -= 1;
        }
        while( l1 > 0 ){
            n = n1[l1-1] + d;         d  = 0;
            while( n > 9 ) { n -= 10; d += 1; } ret[l] = n ;
            l += 1; l1 -= 1;
        }
        while( l2 > 0 ){
            n = n2[l2-1] + d;         d  = 0;
            while( n > 9 ) { n -= 10; d += 1; } ret[l] = n ;
            l += 1; l2 -= 1;
        }
    }

    *result = ret;
    return l;
}

size_t big_int_sub( int * n1, size_t l1, int * n2, size_t l2, int ** result , int mod );
size_t big_int_mul( int * n1, size_t l1, int * n2, size_t l2, int ** result , int mod );
size_t big_int_div( int * n1, size_t l1, int * n2, size_t l2, int ** result , int mod );

int bit_int_cmp( int * n1, size_t l1, int * n2, size_t l2 , int mod )
{
    if( l1 < l2 ) return -1; else 
    if( l1 > l2 ) return  1;
    // l1 == l2
    if( mod == BIG_INT_0_MIN ){
        size_t i = l1 -1 ;
        while( i > 0 ){
            if( n1[i] < n2[i] ) return -1; else 
            if( n1[i] > n2[i] ) return  1;
            i -= 1;
        }
    }else
    { // mod == BIG_INT_0_MAX 
        size_t i = 0;
        while( i < l1 ){
            if( n1[i] < n2[i] ) return -1; else 
            if( n1[i] > n2[i] ) return  1;
            i += 1;
        }
    }
    return 0;
}

