#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZEA 50000
#define MAXSIZEE 400000

#define MAXINT 0x7FFFFFFF

#define MIN(x,y) ((x)<(y)?(x):(y))

long long n,m,s,t;

typedef struct edg{
    long long st;
    long long ed;
    long long w;
    long long ne;
} edg;

typedef struct vct{
    long long e;
} vct;

vct a[MAXSIZEA];
edg e[MAXSIZEE];
long long d[MAXSIZEA];
long long cur[MAXSIZEA];
long long p[MAXSIZEA];
long long num[MAXSIZEA];

long long tmpidx[MAXSIZEA];
long long tmpi;
long long tmpj;

long long bfs()
{
    memset(d, 0, sizeof(long long) * MAXSIZEA );

    long long i, vi, ei;
    long long tmpnj = 0;

    tmpi = 0;
    tmpj = 1;
    tmpnj = 1;
    tmpidx[0] = t;
    d[t] = 1;

    do{
        for( i = tmpi; i < tmpj; ++i ){
            vi = tmpidx[i];
            ei = a[vi].e;
            while( ei > 0 ){
                if( d[e[ei^1].st] == 0 && e[ei^1].w > 0 ){
                    d[e[ei^1].st] = d[vi] + 1;
                    tmpidx[ tmpnj++ ] = e[ei^1].st;
                }
                ei = e[ei].ne;
            }
        }
        tmpi = tmpj;
        tmpj = tmpnj;
    }while( tmpi < tmpj );

    return d[s];
}

long long argument()
{
    int u;
    long long f = MAXINT;
    int ei = 0;

    u = t;
    while( u != s ){
        ei = p[u];
        f = MIN( f, e[ei].w );
        u = e[ei].st;
    }

    u = t;
    while( u != s ){
        ei = p[u];
        e[ei].w -= f;
        e[ei^1].w += f;
        u = e[ei].st;
    }

    return f;
}

long long isap()
{
    long long MF = 0;
    int i;

    bfs();

    memset( cur, 0, sizeof(long long) * n );
    for( i = 0; i <= n; ++i )
        cur[i] = a[i].e;

    memset( num, 0, sizeof(long long) * n );
    for( i = 0; i <= n; ++i )
        num[ d[i] ]++;

    int u = s;
    int ei;
    int can_move = 0;

    while( d[s] <= n ){
        if( u == t ){
            MF += argument();
            u = s;
        }
        
        can_move = 0;

        ei = cur[u];
        while( ei > 0 ){
            if( e[ei].w > 0 && d[u] == d[e[ei].ed] + 1 ){
                can_move = 1;
                cur[u] = ei;
                u = e[ei].ed;
                p[u] = ei;
                break;
            }
            ei = e[ei].ne;
        }

        if( !can_move ){
            int max = n;

            ei = a[u].e;
            while( ei > 0 ){
                if( e[ei].w > 0 ){
                    max = MIN( max, d[ e[ei].ed ] );
                }
                ei = e[ei].ne;
            }

            num[ d[u] ]--;
            if( num[d[u]] == 0 )
                break;
            d[u] = max + 1;
            num[ d[u] ]++;
            cur[u] = a[u].e;
            if( u != s ){
                u = e[ p[u] ].st;
            }
        }
    }

    return MF;
}

int chess[201][201];
int chess_code[201][201];

int atk_x[8] = {-1,-2,-2,-1,+1,+2,+2,+1};
int atk_y[8] = {-2,-1,+1,+2,+2,+1,-1,-2};

int edge_nr = 2;
void add_edge( int st, int ed, int c )
{
    int ei1 = edge_nr;
    int ei2 = edge_nr^1;

    e[ei1].st = st;
    e[ei1].ed = ed;
    e[ei1].w  = c;

    e[ei1].ne = a[st].e;
    a[st].e   = ei1;

    e[ei2].st = ed;
    e[ei2].ed = st;
    e[ei2].w  = 0;

    e[ei2].ne = a[ed].e;
    a[ed].e   = ei2;

    edge_nr += 2;
}

int main()
{
    long long i;
    long long MF = 0;
    int x,y, ax, ay;

    memset( e, 0, sizeof(edg) * MAXSIZEE );
    memset( a, 0, sizeof(vct) * MAXSIZEA );
    memset( chess, 0, sizeof(int) * 200 * 200 );
    memset( chess_code, 0, sizeof(int) * 200 * 200 );

    scanf("%lld %lld", &n, &m);

    for( i = 0; i < m; ++i ){
        scanf("%d %d", &x, &y);
        chess[x][y] = 1;
    }

    s = 0;
    t = n*n+1;

    for( x = 1; x <= n; ++x )
        for( y = 1; y <= n; ++y )
            chess_code[x][y] = (x-1)*n + y;

    for( x = 1; x <= n; ++x )
        for( y = 1; y <= n; ++y ){
            if( chess[x][y] )
                continue;
            if( (x+y)&1 ){
                add_edge( s, chess_code[x][y],1 );
                for( i = 0; i < 8; ++i ){
                    ax = x + atk_x[i];
                    ay = y + atk_y[i];
                    if( ax >= 1 && ax <= n && ay >= 1 && ay <= n && chess[ax][ay] == 0)
                        add_edge( chess_code[x][y], chess_code[ax][ay], MAXINT );
                }
            }
            else
                add_edge( chess_code[x][y], t, 1 );
        }

    n = n * n + 1;
    MF = isap();

    printf("%lld\n", n - 1 - m - MF);

    return 0;
}
