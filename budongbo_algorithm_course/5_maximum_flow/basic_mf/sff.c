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

long long C;
vct a[MAXSIZEA];
edg e[MAXSIZEE];
long long levels[MAXSIZEA];
long long tmpidx[MAXSIZEA];
long long tmpi;
long long tmpj;

void print_graph()
{
    long long vi, ei;
    for( vi = 0; vi <= n; ++vi ){
        printf("[%lld] (%lld) : ", levels[vi], vi);
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
    /*printf("%d-%d\n", vi, vif);*/
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

    printf("%lld\n", MF);

    return 0;
}
