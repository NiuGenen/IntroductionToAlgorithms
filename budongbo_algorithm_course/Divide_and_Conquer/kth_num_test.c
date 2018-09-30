#include <stdio.h>
#include <stdlib.h>
#include "kth_num.h"

#define MALLOC(t,n) ((t*)malloc(sizeof(t)*(n)))

void trace_print(int *num, int n, int pivot_i)
{
    int i = 0;
    for( i = 0; i < n; ++i ){
        if( i != pivot_i ) printf(" %5d ",num[i]);
        else printf("   <%1d> ",num[i]);
    }
    printf("\n");
}

int main(int argc, char*argv[])
{
    int n,k;
    int *num;
    int i;

    if( argc < 3 ) return 1;

    n = atoi( argv[1] );
    k = atoi( argv[2] );

    if( argc < 3 + n ) return 2;
    if( k <= 0 || k > n ) return 3;

    num = MALLOC( int, n );
    for( i = 0; i < n; ++i ){
        num[i] = atoi( argv[ 3 + i ] );
    }

    printf("n = %d, k = %d\n", n, k);
    trace_print(num,n,0);

    printf("%3d-th num is %4d\n", k, kth_num(num,0,n-1,k));

    return 0;
}
