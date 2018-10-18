#include <stdio.h>
#include <stdlib.h>
#include "river.h"

// void river( int *num, int hi, int m, md_and_cnt **his );

int cmpint( const void *a, const void *b )
{
    return *((int*)a) > *((int*)b);
}

int main()
{
    int L;
    int n;
    int m;
    int i;
    int j;
    int *num;
    md_and_cnt **hsty;

    scanf("%d", &L);
    scanf("%d", &n);
    scanf("%d", &m);

    hsty = (md_and_cnt**)malloc( sizeof(md_and_cnt*) * (n + 2) );
    for( i = 0; i < n + 2; ++i ){
        hsty[i] = (md_and_cnt*)malloc( sizeof(md_and_cnt) * m );
    }

    num = (int*)malloc( sizeof(int) * (n + 2) );
    for( i = 0; i < n; ++i ){
        scanf("%d", num + i + 1);
    }

    num[0]   = 0;
    num[n+1] = L;

    qsort( num, n + 2, sizeof(int), cmpint );
    for( i = 0; i < n + 2; ++i ){
        printf("%d ", num[i]);
    }
    printf("\n");

    for( i = 1; i < n; ++i )
        for( j = 0; j <= m; ++j )
            river( num, i, j, hsty, 0 );
    for( j = 0; j <= m; ++j )
        river( num, i, j, hsty, 1 );

    printf("mid = %d\n", hsty[n][m].md );
    printf("cnt = %d\n", hsty[n][m].md ); 

    return 0;
}
