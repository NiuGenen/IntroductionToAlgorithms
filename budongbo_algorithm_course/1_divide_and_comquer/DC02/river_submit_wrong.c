#include <stdio.h>
#include <stdlib.h>

// 
// [0] [1] [...] [n]
// 
// nr = n
// 
long river( long *step, int nr, int m )
{
    int i = 0;
    int min = 0;
    int min_i = 0;
    int merge = 0;

    while( m > 0 ){
        min   = step[0];
        min_i = 0;
        for( i = 1; i <= nr; ++i ){
            if( step[i] < min ){
                min = step[i];
                min_i = i;
            }
        }
        // merge = 0 : with left
        // merge = 1 : with right
        if( min_i == 0 ) merge = 1;
        else if( min_i == nr ) merge = 0;
        else{
            if( step[min_i - 1] < step[min_i + 1] ) merge = 0;
            else merge = 1;
        }
        // merge with left
        if( !merge ){
            step[min_i - 1] += step[min_i];
            i = min_i;
        }
        // merge with right
        else{
            step[min_i] += step[min_i + 1];
            i = min_i + 1;
        }
        // move [i][...][n] one left
        for( ;i < nr; ++i ){
            step[i] = step[i+1];
        }
        // next loop
        m  -= 1;
        nr -= 1;
    }
    // find min
    min   = step[0];
    for( i = 1; i <= nr; ++i )
        if( step[i] < min )
            min = step[i];
    return min;
}

int cmpint( const void *a, const void *b )
{
    return *((long*)a) > *((long*)b);
}

int main()
{
    int L,n,m,i;
    long num[50010];

    scanf("%d", &L);
    scanf("%d", &n);
    scanf("%d", &m);

    for( i = 0; i < n; ++i ){
        scanf("%ld", num + i + 1);
    }

    num[0]   = 0;
    num[n+1] = L;

    // [0] [1] [...] [n+1]
    qsort( num, n + 2, sizeof(long), cmpint );
    for( i = 0; i < n + 1; ++i ){
        num[i] = num[i+1] - num[i];
    }

    // [0] [1] [...] [n]
    printf("%ld\n", river(num,n,m) );
}
