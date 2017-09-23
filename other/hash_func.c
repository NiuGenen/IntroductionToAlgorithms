#include "hash_func.h"

hash_func_ret_t hash_rs( char*str, size_t n )
{
    // hash = ( hash * a + 'c' ) * b
    int b = 378551;
    int a = 63689;
    hash_func_ret_t hash = 0;
    for(size_t i = 0; i < n; ++i){
         hash  = hash * a + str[i];
         hash *= b;
    }
    return hash;
}

hash_func_ret_t hash_js( char*str, size_t n )
{
    // hash = hash ^ ( hash << a    + 'c' + hash >> b    )
    // hash = hash ^ ( hash*2*2*2.. + 'c' + hash/2/2/2.. )
    hash_func_ret_t hash = 141592653;
    for(size_t i = 0; i < n; ++i)
        hash ^= ( (hash<<5) + str[i] + (hash>>2) );
    return hash;
}

hash_func_ret_t hash_pjw( char*str, size_t n )
{
    hash_func_ret_t bits_       = 32;
    hash_func_ret_t bits_3_in_4 = (hash_func_ret_t)( ( bits_ * 3 ) / 4);
    hash_func_ret_t bits_1_in_8 = (hash_func_ret_t)( ( bits_     ) / 8);
    hash_func_ret_t bits_high   = (hash_func_ret_t)0xFFFFFFFF << ( bits_ - bits_1_in_8 );

    hash_func_ret_t hash = 0;
    hash_func_ret_t test = 0;

    for(size_t i = 0; i < n; ++i){
        hash = ( hash << bits_1_in_8 ) + str[i];
        test = hash& bits_high;
        if( test ){
            hash= ( hash^ ( test >> bits_3_in_4 ) ) & (~bits_high) ;
        }
    }

    return hash;
}

hash_func_ret_t hash_elf( char* str, size_t n )
{
    hash_func_ret_t hash = 0;
    hash_func_ret_t x = 0;

    for(size_t i = 0; i < n; ++i){
        hash = ( hash << 4 ) + str[i];
        x = hash & 0xF0000000L;
        if( x ){
            hash ^= ( x >> 24 );
        }
        hash &= ~x;
    }

    return hash;
}

hash_func_ret_t hash_bkdr( char*str, size_t n )
{
    // hash = hash * seed + 'c'
    
    hash_func_ret_t seed = 131;
    hash_func_ret_t hash = 0;

    for(size_t i = 0; i < n; ++i){
        hash = ( hash * seed ) + str[i];
    }

    return hash;
}

hash_func_ret_t hash_sdbm( char*str, size_t n )
{
    hash_func_ret_t hash = 0;
    for(size_t i = 0; i < n; ++i){
        hash = str[i]  + ( hash << 6 ) + ( hash << 16 ) - hash;
    }
    return hash;
}

hash_func_ret_t hash_djb( char*str, size_t n )
{ // best ?
    hash_func_ret_t hash = 5381;
    for(size_t i = 0; i < n; ++i){
        hash = ( ( hash << 5 ) + hash ) + str[i];
    }
    return hash;
}

hash_func_ret_t hash_dek( char*str, size_t n )
{
    hash_func_ret_t hash = n;
    for(size_t i = 0; i < n; ++i){
        hash = (( hash << 5 ) ^ ( hash >> 27 ) ) ^ str[i];
    }
    return hash;
}

hash_func_ret_t hash_ap( char*str, size_t n )
{
    hash_func_ret_t hash = 0xAAAAAAAA;
    for(size_t i = 0; i < n; ++i){
        if( ( i & 1 ) == 0 )
            hash ^= (  ( ( hash <<  7 ) ^ str[i] ) * ( hash >> 3 ) );
        else
            hash ^= ( -( ( hash << 11 ) + str[i] ^ ( hash >> 5 ) ) );
    }
    return hash;
}
