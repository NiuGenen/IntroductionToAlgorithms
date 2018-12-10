#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define ng_t int
ng_t abm(ng_t *b, ng_t *m, int n );

int main()
{
    char buf[20000000];
    ng_t m[1000000];
    ng_t b[1000000];
    char *p = strtok( buf, " " );
    int i = 0;
    int n = 0;

    fgets( buf, 10000, stdin );
    /*printf("%s\n", buf);*/

    p = strtok( buf, " " );
    while(p){
        b[n++] = atol(p);
        p = strtok( NULL, " " );
    }

    fgets( buf, 10000, stdin );
    /*printf("%s\n", buf);*/

    p = strtok( buf, " " );
    while(p){
        m[i++] = atol(p);
        p = strtok( NULL, " " );
    }

    if( n != i ){
        p = 0;
        *p = 1; // segment falut
    }
    else{
        printf("%ld\n", abm(b,m,n));
    }

    return 0;
}

int cmp_ng_t( const void *a, const void *b )
{
    const ng_t *p1 = (ng_t*)a;
    const ng_t *p2 = (ng_t*)b;
    return *p1 < *p2;
}

ng_t abs_ng_t( ng_t a )
{
    if( a < 0 ) return -a;
    else return a;
}

ng_t abm(ng_t *b, ng_t *m, int n )
{
    int i   = 0;
    ng_t max = 0;
    ng_t tmp = 0;

    qsort( b, n, sizeof(ng_t), cmp_ng_t );
    qsort( m, n, sizeof(ng_t), cmp_ng_t );

    max = abs_ng_t( b[0] - m[0] );
    for( i = 1; i < n; ++i ){
        tmp = abs_ng_t( b[i] - m[i] );
        if( tmp > max )
            max = tmp;
    }

    return max;
}
