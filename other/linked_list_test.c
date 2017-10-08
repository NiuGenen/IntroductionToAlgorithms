#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef _MALLOC_ 
#define _MALLOC_(type,n) ( (type*)malloc( sizeof(type) * 1 ) )
#endif

struct llist_entry{
    int a;
    int b;
};

void llist_for_each_entry( void * entry )
{
    struct llist_entry * _entry = ( struct llist_entry* )entry;
    printf("[%5d,%5d]\n", _entry->a, _entry->b );
}

int main()
{
    struct llist* ll = llist_alloc();
    
    for(size_t i = 0; i < 23; ++i){
        struct llist_entry* entry = _MALLOC_( struct llist_entry, 1 );
        entry->a = i ;
        entry->b = i << 5 ;
        if( i & 1 ) ll->add_head( ll, entry );
        else        ll->add_tail( ll, entry );
    }

    ll->rmv_head( ll );
    ll->rmv_tail( ll );

    ll->for_each( ll, llist_for_each_entry );

    return 0;
}
