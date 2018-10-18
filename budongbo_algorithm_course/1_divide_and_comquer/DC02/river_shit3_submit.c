#include <stdio.h>
#include <stdlib.h>

// [n][m]
// 
// 1 <  n  <  stone_nr
// 0 <= m  <= n
// 

// step[0:n]
// [0]   = stone[1]   - stone[0]
// [1]   = stone[2]   - stone[1]
// ...
// [n-1] = stone[n]   - stone[n-1]
// [n]   = stone[n+1] - stone[n]
int _river( int *step, int nk, int mk)
{
    int i  = 0;
    int md = 0;
    int *tmpstep;

    if( mk == 0 ){
        md = step[0];
        for( i = 1; i <= nk; ++i ){
            if( step[i] < md ) md = step[i];
        }
        /*hsty[nk][mk] = md;*/
        /*return;*/
        return md;
    }

    if( mk == nk ){
        md = step[0];
        for( i = 1; i <=nk; ++i ){
            md += step[i];
        }
        /*hsty[nk][mk] = md;*/
        /*return;*/
        return md;
    }

    // 0 < mk < nk 
    // 
    // [0] [1] [...] [nk-1] [nk]
    // 1. [nk] stay  : goto [nk-1][mk] and compare with [nk]
    md = _river(step,nk-1,mk);
    if( step[nk] < md ) md = step[nk];
    // 2. [nk] merge : goto [nk-1][mk-1] which [nk-1] = [nk-1] + [nk]
    tmpstep = (int*)malloc(sizeof(int)*nk);
    for( i = 0; i < nk; ++i )
        tmpstep[i] = step[i];
    tmpstep[nk-1] += step[nk];
    i = _river(tmpstep,nk-1,mk-1);
    if( i > md ) md = i;
    
    free(tmpstep);
    return md;
}

// stone[0:n+1]
// [0]    =  0
// [1..n] =  stones sorted
// [n+1]  =  L
int river( int *stone, int n, int m)
{
    int nk = 0;
    /*int mk = 0;*/
    for( nk = 0; nk < n+1; ++nk )
        stone[nk] = stone[nk+1] - stone[nk];
    /*for( nk = 0; nk <= n; ++nk )*/
        /*for( mk = 0; mk <=nk && mk <= m; ++mk )*/
            /*_river( stone, nk, mk );*/
    /*return hsty[n][m];*/
    return _river(stone, n, m);
}

int cmpint( const void *a, const void *b )
{
    return *((int*)a) > *((int*)b);
}

int main()
{
    int L,n,m,i;
    int num[50010];

    scanf("%d", &L);
    scanf("%d", &n);
    scanf("%d", &m);

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

    printf("%d\n", river(num,n,m) );
}
