#include <stdio.h>
/*
 * The first line of input are m and n,which denote the size
 * of the grid. The second line of input is the kk. The next
 * m lines are the score matrix. (1≤m,n,k≤1001≤m,n,k≤100,and
 * 0≤score≤1000≤score≤100)
 */
#define MAXM 100
#define MAXN 100
#define MAXK 100

int g[MAXM][MAXN]; // [m][n]
void topk( int m, int n, int k );

int main()
{
    int k;
    int m,n;
    int i,j;

    scanf("%d %d", &m, &n);
    scanf("%d", &k);

    for( i = 0; i < m; ++i ){
        for( j = 0; j < n; ++j ){
            scanf("%d", &(g[i][j]) );
        }
    }

    topk(m,n,k);

    return 0;
}

void mqsort( int *p, int lo, int hi )
{
    if( lo < hi ){
        int pivot   = p[ lo ];
        int i = lo, j = hi;
        while( i < j )
        {
            while( i < j && pivot >= p[ j ] ) j -= 1;
            p[ i ] = p[ j ];
            while( i < j && pivot <= p[ i ] ) i += 1;
            p[ j ] = p[ i ];
        }
        p[ i ] = pivot;
        mqsort(p, lo, i-1);
        mqsort(p, i+1, hi);
    }
}

int s[MAXM][MAXN][MAXK]; // s[m][n][k]

void topk( int m, int n, int k )
{
    int mi,ni,ki;
    int v[2*MAXK],vi,vg1,vg2;

    s[0][0][0] = g[0][0];
    for( ki = 1; ki < k; ++ki )
        s[0][0][ki] = -1;

    for( ni = 1; ni < n; ++ni ){
        s[0][ni][0] = s[0][ni-1][0] + g[0][ni];
        for( ki = 1; ki < k; ++ki )
            s[0][ni][ki] = -1;
    }

    for( mi = 1; mi < m; ++mi ){
        s[mi][0][0] = s[mi-1][0][0] + g[mi][0];
        for( ki = 1; ki < k; ++ki )
            s[mi][0][ki] = -1;
    }

    for( mi = 1; mi < m; ++mi ){
        for( ni = 1; ni < n; ++ni ){
            vi = 0;
            for( ki = 0; ki < k; ++ki ){
                vg1 = s[mi-1][ni][ki];
                vg2 = s[mi][ni-1][ki];
                if( vg1 >= 0 ) v[vi++] = vg1 + g[mi][ni];
                if( vg2 >= 0 ) v[vi++] = vg2 + g[mi][ni];
            }
            for( ; vi < k; ++vi )
                v[vi] = -1;
            mqsort( v, 0, vi - 1 );
            for( ki = 0; ki < k; ++ki ){
                s[mi][ni][ki] = v[ki];
            }
        }
    }

    /*for( mi = 0; mi < m; ++mi ){*/
        /*for( ni = 0; ni < n; ++ni ){*/
            /*printf("%d",g[mi][ni]);*/
            /*printf("{");*/
            /*for( ki = 0; ki < k - 1; ++ki ){*/
                /*printf("%d,",s[mi][ni][ki]);*/
            /*}*/
            /*printf("%d} ",s[mi][ni][ki]);*/
        /*}*/
        /*printf("\n");*/
    /*}*/

    for( ki = 0; ki < k - 1; ++ki )
        printf("%d ", s[m-1][n-1][ki] );
    printf("%d", s[m-1][n-1][ki] );
    printf("\n");
}
