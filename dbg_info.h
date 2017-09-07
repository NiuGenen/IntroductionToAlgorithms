#ifndef _DBG_INFO_H_
#define _DBG_INFO_H_

#include <stdint.h>

#define ITA_DBG_T_STR  0
#define ITA_DBG_T_INT  1
#define ITA_DBG_T_UINT 2
#define ITA_DBG_T_FLO  3

#ifdef ITA_DBG_ENABLE

#define ITA_DBG_INFO( t , x ) do{ \
    printf("%s:%-5lu      %-20s   ", __FILE__, (size_t)__LINE__ , __FUNCTION__ ); \
    uint64_t x_uint   ; \
    int64_t  x_int    ; \
    double   x_double ; \
    switch(t){ \
        case ITA_DBG_T_STR: \
            printf("%s" , (char*)x  ); break; \
        case ITA_DBG_T_UINT: \
            x_uint = *x ; \
            printf("%lu", x_uint ); break; \
        case ITA_DBG_T_INT: \
            x_int   = *x ; \
            printf("%ld", x_int  ); break; \
        case ITA_DBG_T_FLO: \
            x_double = *x ;  \
            printf("%lf", x_double ); break; \
        default: printf("default"); break; } \
    printf("\n"); \
}while(0);

#else
#define ITA_DBG_INFO(t,x)
#endif

#endif
