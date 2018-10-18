#include <stdio.h>
#include <stdlib.h>

typedef struct rcd{
    int md;
    int idx;
}rcd;

rcd hsty[500][500];
// [n][m]
// 
// 1 <  n  <  stone_nr
// 0 <= m  <= n
// 

void _river( int *stone, int nk, int mk , int is_n)
{
    int i    =  0;
    int md   =  0;
    int idx  =  0;
    int tmp  =  0;
    int _idx =  0;
    int _md  =  0;

    if( mk == 0 ){
        md = stone[1] - stone[0];
        // [i] - [i-1]
        for( i = 1; i <=nk; ++i  ){
            tmp = stone[i] - stone[i-1];
            if( tmp < md ){
                md = tmp;
            }
        }
        hsty[nk][mk].md  = md;
        hsty[nk][mk].idx = nk;
        goto _OUT_;
    }

    if( mk == nk ){
        hsty[nk][mk].md  = 0;
        hsty[nk][mk].idx = 0;
        goto _OUT_;
    }

    // 0 < mk < nk 
    // 
    // 1. [nk] stay
    //     goto [nk-1][mk]
    //     
    _md  = hsty[nk-1][mk].md ;
    _idx = hsty[nk-1][mk].idx;
    tmp = stone[nk] - stone[_idx];
    if( tmp < _md || _md == 0 ){
        _md  = tmp;
        _idx = nk;
    }
    if( is_n ){
        tmp = stone[nk+1] - stone[nk];
        if( tmp < _md ){
            _md  = tmp;
            _idx = nk;
        }
    }
    // 
    // 2. [nk] remove
    //     goto [nk-1][mk-1]
    md  = hsty[nk-1][mk-1].md;
    idx = hsty[nk-1][mk-1].idx;
    if( is_n ){
        tmp = stone[nk+1] - stone[idx];
        if( tmp < md ){
            md = tmp;
            /*idx = idx;*/
        }
    }
    // compare and get result
    if( _md > md ){
        hsty[nk][mk].md  = _md;
        hsty[nk][mk].idx = _idx;
    }else{
        hsty[nk][mk].md  = md;
        hsty[nk][mk].idx = idx;
    }
_OUT_:
    printf("[%d][%d]={%d,%d}\n", nk, mk, hsty[nk][mk].md, hsty[nk][mk].idx );
}

// [0]    = 0
// [1..n] = stones 
// [n+1]  = L
int river( int *stone, int n, int m)
{
    int nk = 1;
    int mk = 0;
    for( nk = 1; nk < n; ++nk )
        for( mk = 0; mk <=nk && mk <= m; ++mk )
            _river( stone, nk, mk, 0 );
    for( mk = 0; mk <=m && mk <= nk; ++mk )
        _river( stone, nk, mk, 1 );
    return hsty[n][m].md;
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
