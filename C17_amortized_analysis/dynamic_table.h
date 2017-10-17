#ifndef _DYNAMIC_TABLE_H_
#define _DYNAMIC_TABLE_H_ 

#include <stddef.h>

#define DYNAMIC_COUNTER_ENABLED

void __dynamic_counter_pr();

typedef void (*dynamic_entry_value_pr_t)( void * value );

struct dynamic_table_entry{
    void * value;
};

struct dynamic_table{
    struct dynamic_table_entry * entry;
    size_t size;
    size_t length;
    dynamic_entry_value_pr_t value_pr;
    // operations 
    void (*put)(  struct dynamic_table * dnmc , void * value );
    void *(*at)(  struct dynamic_table * dnmc , size_t index );
    void *(*get)( struct dynamic_table * dnmc , size_t index );
    void *(*pop)(  struct dynamic_table * dnmc );
    void (*push)(  struct dynamic_table * dnmc, void * value );
    void (*pr)(   struct dynamic_table * dnmc );
};

struct dynamic_table * dnmc_table_alloc( size_t size , dynamic_entry_value_pr_t value_pr );
void dnmc_table_free( struct dynamic_table * dnmc );

#endif
