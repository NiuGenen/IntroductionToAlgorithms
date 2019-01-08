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

long long C;
vct a[MAXSIZEA];
edg e[MAXSIZEE];
long long levels[MAXSIZEA];
long long tmpidx[MAXSIZEA];
long long tmpi;
long long tmpj;

long long bfs()
{
    memset(levels, 0, sizeof(long long) * MAXSIZEA );

    long long i, vi, ei;
    long long tmpnj = 0;

    tmpi = 0;
    tmpj = 1;
    tmpnj = 1;
    tmpidx[0] = s;
    levels[s] = 1;

    do{
        for( i = tmpi; i < tmpj; ++i ){
            vi = tmpidx[i];
            ei = a[vi].e;
            while( ei > 0 ){
                /*if( levels[e[ei].ed] == 0 && e[ei].w > 0 ){*/
                if( levels[e[ei].ed] == 0 && e[ei].w >= C ){
                    levels[e[ei].ed] = levels[vi] + 1;
                    tmpidx[ tmpnj++ ] = e[ei].ed;
                }
                ei = e[ei].ne;
            }
        }
        tmpi = tmpj;
        tmpj = tmpnj;
    }while( tmpi < tmpj );

    return levels[t];
}

long long apply( long long vi, long long vif )
{
    if( vi == t || vif <= 0 ) return vif;

    long long myf = 0;
    long long nxtf = 0;
    long long ei;
    
    ei = a[vi].e;
    while( ei > 0 ){
        if( levels[e[ei].ed] == levels[vi] + 1 && e[ei].w > 0 ){
            nxtf = apply( e[ei].ed, MIN(vif, e[ei].w) );

            e[ei].w   -= nxtf;
            e[ei^1].w += nxtf;

            myf += nxtf;
            vif -= nxtf;
            if( vif <= 0 )
                break;
        }
        ei = e[ei].ne;
    }

    return myf;
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
    long long i,ei;
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
                    if( ax >= 1 && ax <= n && ay >= 1 && ay <= n )
                        add_edge( chess_code[x][y], chess_code[ax][ay], MAXINT );
                }
            }
            else
                add_edge( chess_code[x][y], t, 1 );
        }

    C = 0;
    ei = a[s].e;
    while( ei > 0 ){
        C += e[ei].w;
        ei = e[ei].ne;
    }

    while( C ){
        while( bfs() ){
            MF += apply(s, MAXINT);
        }
        C = C / 2;
    }

    printf("%lld\n", n * n - m - MF);

    return 0;
}
