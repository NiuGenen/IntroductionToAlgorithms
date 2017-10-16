#ifndef _BIG_INT_H_
#define _BIG_INT_H_

#include <stddef.h>

#define BIG_INT_0_MIN 0 // [0] is GE wei
#define BIG_INT_0_MAX 1 

size_t big_int_add( int * n1, size_t l1, int * n2, size_t l2, int ** result , int mod );
size_t big_int_sub( int * n1, size_t l1, int * n2, size_t l2, int ** result , int mod );
size_t big_int_mul( int * n1, size_t l1, int * n2, size_t l2, int ** result , int mod );
size_t big_int_div( int * n1, size_t l1, int * n2, size_t l2, int ** result , int mod );

int bit_int_cmp( int * n1, size_t l1, int * n2, size_t l2 , int mod );

#endif
