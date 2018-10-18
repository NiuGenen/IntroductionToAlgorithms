#include "river.h"
#include <stdio.h>
#include <stdlib.h>

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
    int *num;

    scanf("%d", &L);
    scanf("%d", &n);
    scanf("%d", &m);

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

    printf("result = %d\n", river(num,n,m) );
}
