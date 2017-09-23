#ifndef _HASH_FUNC_H_
#define _HASH_FUNC_H_ 

#include "stddef.h"

typedef long hash_func_ret_t;

typedef hash_func_ret_t(*hash_fn_t)( char*str, size_t n );

#define HASH_FUNC_NAME_LEN 10

hash_func_ret_t hash_rs  ( char* str , size_t n );
hash_func_ret_t hash_js  ( char* str , size_t n );
hash_func_ret_t hash_pjw ( char* str , size_t n );
hash_func_ret_t hash_elf ( char* str , size_t n );
hash_func_ret_t hash_bkdr( char* str , size_t n );
hash_func_ret_t hash_sdbm( char* str , size_t n );
hash_func_ret_t hash_djb ( char* str , size_t n );
hash_func_ret_t hash_dek ( char* str , size_t n );
hash_func_ret_t hash_ap  ( char* str , size_t n );

#ifdef HASH_FUNC_GLOBAL_VAL
#define HASH_FUNC_NR 9

char hash_rs_name  [ HASH_FUNC_NAME_LEN ] = "rs\0"   ;
char hash_js_name  [ HASH_FUNC_NAME_LEN ] = "js\0"   ;
char hash_pjw_name [ HASH_FUNC_NAME_LEN ] = "pjw\0"  ;
char hash_elf_name [ HASH_FUNC_NAME_LEN ] = "elf\0"  ;
char hash_bkdr_name[ HASH_FUNC_NAME_LEN ] = "bkdr\0" ;
char hash_sdbm_name[ HASH_FUNC_NAME_LEN ] = "sdbm\0" ;
char hash_djb_name [ HASH_FUNC_NAME_LEN ] = "djb\0"  ;
char hash_dek_name [ HASH_FUNC_NAME_LEN ] = "dek\0"  ;
char hash_ap_name  [ HASH_FUNC_NAME_LEN ] = "ap\0"   ;

hash_fn_t hash_func_s[ HASH_FUNC_NR ] = {
    hash_rs   , // 0
    hash_js   , // 1
    hash_pjw  , // 2
    hash_elf  , // 3
    hash_bkdr , // 4
    hash_sdbm , // 5
    hash_djb  , // 6
    hash_dek  , // 7
    hash_ap     // 8
};

char* hash_func_name[ _HASH_FUNC_H_ ] = {
    hash_rs_name   , // 0
    hash_js_name   , // 1
    hash_pjw_name  , // 2
    hash_elf_name  , // 3
    hash_bkdr_name , // 4
    hash_sdbm_name , // 5
    hash_djb_name  , // 6
    hash_dek_name  , // 7
    hash_ap_name     // 8
};

#endif // HASH_GLOBAL_VAL

#endif // _HASH_FUNC_H_
