#ifndef _DYNAMIC_TABLE_H_
#define _DYNAMIC_TABLE_H_ 

#include <stddef.h>

struct dynamic_table_entry{
    void * value;
};

struct dynamic_table{
    struct dynamic_table_entry * entry;
    size_t size;
    size_t length;
    // operations 
    void (*put)(  struct dynamic_table * dnmc , void * value );
    void *(*at)(  struct dynamic_table * dnmc , size_t index );
    void *(*get)( struct dynamic_table * dnmc , size_t index );
};

struct dynamic_table * dnmc_table_alloc( size_t size );
void dnmc_table_free( struct dynamic_table * dnmc );

#endif
