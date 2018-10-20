#include <stdio.h>
/*
 * The first line of input is T, which means there are
 * T groups test cases. For each test case, the first
 * line contains the length of sequence N, and the
 * second line contains the elements of sequence - N
 * integers in the range from 0 to 1000000 each,
 * separated by spaces(1 <= N <= 100000).
 */

int lis(int *num, int lo, int hi);
int mmv[100000];

int main()
{
    int t;
    int i;
    int j;
    int n;
    int num[100000];

    scanf("%d",&t); // number of problems;

    for( i = 0; i < t; ++i ){
        scanf("%d",&n); // number of numbers
        for( j = 0; j < n; ++j )
            scanf("%d", num + j);
        printf("%d\n", lis(num,0,n-1) );
    }

    return 0;
}

// num[] : numbers with index @i
// mmv[] : mmv[LSI] records LSI's MinLasValue among all possible sub-sequence

int bs(int *n, int lo, int hi, int k)
{
    int mid = 0;
    while(lo <= hi){
        mid = (lo + hi) / 2;
        if( n[mid] <= k)
            lo = mid + 1;
        else 
            hi = mid - 1;
    }
    return lo;
}

int lis(int *num, int lo, int hi)
{
    int i;
    int max;
    int idx;

    if( lo > hi ) return 0;

    max     = 1;
    mmv[1]  = num[lo];
    for( i = lo + 1; i <= hi; ++i ){
        if( num[i] > mmv[max] ){
            max += 1;
            mmv[max] = num[i];
        }
        else{
            idx = bs(mmv,1,max,num[i]);
            if( idx > 0 && mmv[idx-1] != num[i] )
                mmv[idx] = num[i];
        }
    }

    return max;
}
