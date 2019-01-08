#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZEA 2000
#define MAXSIZEE 300000

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

void print_graph()
{
    long long vi, ei;
    for( vi = 0; vi <= n; ++vi ){
        printf("[%lld] (%lld) : ", d[vi], vi);
        ei = a[vi].e;
        while( ei > 0 ){
            printf(" %2lld(%lld)-%lld-(%lld) ",ei, e[ei].st, e[ei].w, e[ei].ed);
            ei = e[ei].ne;
        }
        printf("\n");
    }
    printf("/------------------------------/\n");
}

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
    printf("/-------- Before --------------/\n");
    print_graph();

    int i;
    for( i = 0; i <= n; ++i )
        printf("p[%d] = %lld  ",i, p[i]);
    printf("\n");

    for( i = 0; i <= n; ++i )
        printf("n[%d] = %lld  ",i, num[i]);
    printf("\n");

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

    printf("/-------- After  --------------/\n");
    print_graph();

    return f;
}

long long isap()
{
    long long MF = 0;
    int i;

    bfs();
    print_graph();

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
        printf("u = (%d)\n",u);

        ei = cur[u];
        while( ei > 0 ){
            printf("    (%lld)-%lld-(%lld)\n", e[ei].st, e[ei].w, e[ei].ed);
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
            printf("/=== adjust ===/\n");
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

            print_graph();
            
            for( i = 0; i <= n; ++i )
                printf("p[%d] = %lld  ",i, p[i]);
            printf("\n");

            for( i = 0; i <= n; ++i )
                printf("n[%d] = %lld  ",i, num[i]);
            printf("\n");

            for( i = 0; i <= n; ++i )
                printf("r[%d] = %lld  ",i, cur[i]);
            printf("\n");
            printf("/=== finish ===/\n");
        }
    }

    return MF;
}

int main()
{
    long long i,tm,ei,ei2,eiu;
    long long u,v;
    long long c;
    long long MF = 0;
    long long old = 0;

    memset( e, 0, sizeof(edg) * MAXSIZEE );
    memset( a, 0, sizeof(vct) * MAXSIZEA );

    scanf("%lld %lld %lld %lld", &n, &m, &s, &t);
    tm = m;

    for( i = 1; i <= tm; ++i ){
        old = 0;
        ei  = 2 * i;
        ei2 = ei + 1;
        scanf("%lld %lld %lld", &u, &v, &c);

        eiu = a[u].e;
        while( eiu > 0 ){
            if( e[eiu].ed == v ){
                e[eiu].w += c;
                old = 1;
                tm--;
                i--;
                break;
            }
            eiu = e[eiu].ne;
        }

        if( old )
            continue;

        e[ei].st = u;
        e[ei].ed = v;
        e[ei].w  = c;
        e[ei].ne = a[u].e;
        a[u].e = ei;

        e[ei2].st = v;
        e[ei2].ed = u;
        e[ei2].w  = 0;
        e[ei2].ne = a[v].e;
        a[v].e = ei2;
    }

    MF = isap();

    printf("%lld\n", MF);

    return 0;
}
