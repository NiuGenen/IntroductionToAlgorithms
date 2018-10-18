#include <stdlib.h>
#include <stdio.h>

/* no greedy for this problem */
/* no DP for this problem too */

int river( int *num, int n, int m )
{
    // num.length == n + 2
    // [0]    is 0
    // [n+1]  is L
    // [1..n] is the stone
    int *step = (int*)malloc( sizeof(int) * ( n + 1 ) );
    int i = 0;
    int nr = n + 1;
    int min = 0;
    int min_i = 0;
    int merge = 0;

    // 0, 1, ... , n
    for( i = 0; i < nr ; ++i ){
        step[i] = num[i+1] - num[i];
    }
    for( i = 0; i < nr; ++i ){
        printf("%2d ", step[i]);
    }
    printf("\n");

    // 1. [0][1][...][n][n+1]
    // 2. [0][1][...][n]
    // ...
    while( m > 0 ){
        min   = step[0];
        min_i = 0;
        for( i = 1; i < nr; ++i ){
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
        // move [i][...][n+1] one left
        for( ;i < nr; ++i ){
            step[i] = step[i+1];
        }
        // next loop
        m -= 1;
        nr -= 1;
        // print
        for( i = 0; i < nr; ++i ){
            printf("%2d ", step[i]);
        }
        printf("\n");
    }

    // find min
    min   = step[0];
    for( i = 1; i < nr; ++i )
        if( step[i] < min )
            min = step[i];
    return min;
}
