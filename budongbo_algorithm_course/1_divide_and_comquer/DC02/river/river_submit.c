#include <stdio.h>

void mqsort( int *p, int lo, int hi )
{
    if( lo < hi ){
        int pivot   = p[ lo ];
        int i = lo, j = hi;
        while( i < j )
        {
            while( i < j && pivot <= p[ j ] ) j -= 1;
            p[ i ] = p[ j ];
            while( i < j && pivot >= p[ i ] ) i += 1;
            p[ j ] = p[ i ];
        }
        p[ i ] = pivot;
        mqsort(p, lo, i-1);
        mqsort(p, i+1, hi);
    }
}


// [0] [1] [...] [n+1]
// 
// [0]     : 0
// [1...n] : stones
// [n+1]   : L
// 
int river( int *stone, int n, int m )
{
    int il    = 1,   ih = n,            i   = 0; 
    int lo    = 0,   hi = stone[ih+1],  mid = 0;
    int i_pre = 0;
    int _m    = 0;
    while( hi >= lo ){
        mid   = ( lo + hi ) / 2;
        _m    = 0;
        i_pre = 0;
        // count the number of stones removed to satisfy MinDis = mid
        for( i = il; i <= ih + 1; ++i  ){
            if( stone[i] - stone[i_pre] < mid ){
                _m += 1;
            }
            else{
                i_pre = i;
            }
        }
        if( _m > m ){
            // if remove too much stones
            // mid is too large
            hi = mid - 1;
        }
        else{
            // if remove too less stones
            // mid is too small ; or check if it can become bigger
            lo = mid + 1;
        }
    }
    return hi;
}

int main()
{
    int L,n,m,i;
    int num[50010];

    scanf("%d", &L);
    scanf("%d", &n);
    scanf("%d", &m);

    for( i = 1; i <= n; ++i )
        scanf("%d", num + i);

    num[0]   = 0;
    num[n+1] = L;

    mqsort( num, 0, n+1);

    printf("%d\n", river(num,n,m) );
}
