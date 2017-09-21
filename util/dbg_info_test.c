#include <stdio.h>
#include <stdlib.h>
#define ITA_DBG_ENABLE
#include "dbg_info.h"

#define FUNC_NR 3

void func_one()
{
    ITA_DBG_INFO( ITA_DBG_T_STR, "to dbg 1" );
    printf("func_one\n");
}

void func_two()
{
    int a = 100;
    ITA_DBG_INFO( ITA_DBG_T_INT, &a );
    printf("func_two\n");
}

void func_three()
{
    double a = 1.1;
    ITA_DBG_INFO( ITA_DBG_T_FLO, &a );
    printf("func_three\n");
}

typedef void (*func_void_t)(void);

int main()
{
    func_void_t* func;
    func = (func_void_t * )malloc( sizeof(func_void_t) * FUNC_NR );

    func[0] = func_one   ;
    func[1] = func_two   ;
    func[2] = func_three ;

    for(size_t i = 0; i < FUNC_NR; ++i){
        func[i]();
    }

    return 0;
}
