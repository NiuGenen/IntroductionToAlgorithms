#include <stdio.h>
#include <stdlib.h>

int cr( int *w, int n, int l );

int main()
{
    int n = 0; // the number of person, 1 <= n <= 50000
    int l = 0; // the limit of boat, 1 <= l <= 30000
    int w[50000];
    int *p = w;
    int i;

    scanf("%d", &n);
    scanf("%d", &l);

    for( i = 0; i < n; ++i ){
        scanf("%d", p++);
    }

    printf("%d\n", cr(w,n,l) );

    return 0;
}

int cmp_int( const void *a, const void *b )
{
    const int *p1 = (int*)a;
    const int *p2 = (int*)b;
    return *p1 < *p2;
}

int cr( int *w, int n, int l )
{
    int cnt = 0;
    int lo = 0;
    int hi = n - 1;

    qsort( w, n, sizeof(int), cmp_int );
    
    int i = 0;
    for( ; i < n; ++i ){
        printf("%d ", w[i]);
    }
    printf("\n");

    while( lo <= hi ){
        if( hi - lo < 2 )
            break;
        if( l - w[lo] >= w[hi] )
            hi -= 1;
        lo += 1;
        cnt += 1;
    }
    if( lo == hi ) cnt += 1;
    else{
        if( l - w[lo] >= w[hi] )
            cnt += 1;
        else 
            cnt += 2;
    }

    return cnt;
}
