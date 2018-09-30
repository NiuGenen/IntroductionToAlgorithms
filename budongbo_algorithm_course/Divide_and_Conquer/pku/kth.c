#include <stdio.h>
#include <stdlib.h>

int kth_num(int *num, int lo, int hi, int k)
{
    int pivot = num[lo];
    int i = lo, j = hi;
    int len = 0;
    while( i < j ){
        while( i < j && pivot <= num[j] ) j -= 1;
        num[i] = num[j];
        while( i < j && pivot >= num[i] ) i += 1;
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

int main()
{
    int n;
    int qs; // qs <= 5000
    int num[100000];
    int num_cp[100000];
    int i;
    int lo,hi,k;

    scanf("%d %d",&n,&qs);

    for( i = 0; i < n; ++i ){
        scanf("%d", num + i);
    }

    while( qs > 0 )
    {
        scanf("%d %d %d", &lo, &hi, &k );
        for( i = lo - 1; i < hi; ++i ){
            num_cp[i] = num[i];
        }
        printf("%d\n", kth_num(num_cp,lo-1,hi-1,k));
        qs -= 1;
    }

    return 0;
}
