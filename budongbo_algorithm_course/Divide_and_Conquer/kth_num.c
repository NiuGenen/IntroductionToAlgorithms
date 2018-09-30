#include <stdio.h>
#include <stdlib.h>

#define MALLOC(t,n) ((t*)malloc(sizeof(t)*(n)))

int N;

void trace_print(int *num, int n, int pivot_i)
{
    int i = 0;
    for( i = 0; i < n; ++i ){
        if( i != pivot_i ) printf(" %5d ",num[i]);
        else printf("   <%1d> ",num[i]);
    }
    printf("\n");
}

int kth_num(int *num, int lo, int hi, int k)
{
    int pivot = num[lo];
    int i = lo, j = hi;
    int len = 0;
    while( i < j ){
        while( i < j && pivot >= num[j] ) j -= 1;
        num[i] = num[j];
        while( i < j && pivot <= num[i] ) i += 1;
        num[j] = num[i];
    }
    num[i] = pivot;
    trace_print(num,N,i);
    len = i - lo + 1; // [ lo, pivot ] bigger than pivot
    if( len == k ) return pivot;
    else{
        if( len > k ) return kth_num( num, lo, i, k );
        else return kth_num( num, i+1, hi, k - len );
    }
}

int main(int argc, char*argv[])
{
    int n,k;
    int *num;
    int i;

    if( argc < 3 ) return 1;

    n = atoi( argv[1] );
    k = atoi( argv[2] );
    N = n;

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
