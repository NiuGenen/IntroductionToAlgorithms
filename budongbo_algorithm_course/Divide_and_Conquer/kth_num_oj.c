#include <stdio.h>
#include <stdlib.h>
#include "kth_num.h"

/*
 * int kth_num(int *num, int lo, int hi, int h);

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
    len = i - lo + 1; // [ lo, pivot ] bigger than pivot
    if( len == k ) return pivot;
    else{
        if( len > k ) return kth_num( num, lo, i, k );
        else return kth_num( num, i+1, hi, k - len );
    }
}

 */

int main()
{
    int n,k;
    int num[100];
    int i;

    scanf("%d %d",&n,&k);

    for( i = 0; i < n; ++i ){
        scanf("%d", num + i);
    }

    printf("%d\n", kth_num(num,0,n-1,k));

    return 0;
}
