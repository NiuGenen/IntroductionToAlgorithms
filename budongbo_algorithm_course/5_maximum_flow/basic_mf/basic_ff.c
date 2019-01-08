#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 100

int MF = 0;
int n,m,s,t;

typedef struct edg{
    int st;
    int ed;
    int w;
    int f;
    int ne;
} edg;

typedef struct vct{
    int e;
    int visited;
} vct;

vct a[MAXSIZE];
edg e[2*MAXSIZE];
vct bfsa[MAXSIZE];
edg bfse[2*MAXSIZE];
int tmpidx[MAXSIZE];
int tmpi;
int tmpj;
struct stackentry{
    int e;
    int idx;
}stack[MAXSIZE];
int stp = 0;

void print_gf()
{
    int vi, ei;
    for( vi = 0; vi < n; ++vi ){
        printf("(%d) : ", vi);
        ei = a[vi].e;
        while( ei != -1 ){
            printf("(%d)-E%d-%d-%c-(%d) ", e[ei].st, ei, e[ei].w, 'X' + e[ei].f, e[ei].ed);
            ei = e[ei].ne;
        }
        printf("\n");
    }
}

void print_bfs()
{
    int vi,ei;
    for( vi = 0; vi < n; ++vi ){
        printf("(%d) : ", vi);
        ei = bfsa[vi].e;
        while( ei != -1 ){
            printf("(%d)-E%d-%d-%d-(%d) ", bfse[ei].st, ei, bfse[ei].w, bfse[ei].f, bfse[ei].ed);
            ei = bfse[ei].ne;
        }
        printf("\n");
    }
}

int bfs()
{
    printf("/=== BFS to Find Shoeters Path ===/\n");
    int i, vi, ei, eiv, vie;
    int tmpnj = 0;
    int reach_t = 0;
    int level = 1;

    printf("/--- GF ---/\n");
    print_gf();
    printf("/--- GF ---/\n");

    for( i = 0; i < MAXSIZE; ++i ){
        bfsa[i].e = -1;
        bfsa[i].visited = 0;
        bfse[i].ne = -1;
        bfse[i].f  = 0;
    }

    bfsa[s].visited = 1;
    tmpi = 0;
    tmpj = 1;
    tmpnj = 1;
    tmpidx[0] = s;

    do{
        level++;
        for( i = tmpi; i != tmpj; i=(i+1)%MAXSIZE ){
            vi = tmpidx[i];
            ei = a[vi].e;
            while( ei != -1 ){
                eiv = e[ei].ed;
                if( eiv == t ){
                    reach_t = 1;
                    break;
                }
                ei = e[ei].ne;
            }
        }
        if( reach_t )
            printf("//-- t Level %d :", level);
        else
            printf("//-- Level %d :", level);
        for( i = tmpi; i != tmpj; i=(i+1)%MAXSIZE )
            printf("(%d)", tmpidx[i]);
        printf(" ---/\n");
        for( i = tmpi; i != tmpj; i=(i+1)%MAXSIZE ){
            vi = tmpidx[i];
            printf("(%d)\n",vi);
            ei = a[vi].e;
            while( ei != -1 ){
                eiv = e[ei].ed;
                if( reach_t && eiv != t ){
                    ei = e[ei].ne;
                    continue;
                }
                printf("-%d-(%d) : level %d => ",ei,eiv, bfsa[eiv].visited );
                if( bfsa[eiv].visited == 0 ){
                    bfsa[eiv].visited = level;
                    printf(" newly   visited in this level => ");
                    tmpidx[ tmpnj ] = e[ei].ed;
                    tmpnj = (tmpnj + 1) % MAXSIZE;
                }else if( bfsa[eiv].visited < level ){
                    ei = e[ei].ne;
                    printf(" jump over\n");
                    continue;
                }else
                    printf(" already visited in this level => ");
                bfse[ei].st = e[ei].st;
                bfse[ei].ed = e[ei].ed;
                bfse[ei].w  = e[ei].w;
                bfse[ei].ne = -1;
                printf(" add edge (%d)-%d-(%d)\n", e[ei].st, ei, e[ei].ed);
                if( bfsa[vi].e == -1 )
                    bfsa[vi].e = ei;
                else{
                    vie = bfsa[vi].e;
                    while( bfse[vie].ne != -1 )
                        vie = bfse[vie].ne;
                    bfse[vie].ne = ei;
                }
                ei = e[ei].ne;
            }
        }
        tmpi = tmpj;
        tmpj = tmpnj;
    }while( reach_t == 0 && tmpi != tmpj );

    if( reach_t ){
        printf("/--- BFS ---/\n");
        print_bfs();
        printf("/--- BFS ---/\n");
        return 1;
    }

    printf("/--- BFS ---/\n");
    printf("Ending\n");
    printf("/--- BFS ---/\n");

    return 0;
}

void apply()
{
    printf("/=== Applying ===/\n");

    int nomore = 0;
    int ei = 0, vi = -1, eit = 0;
    int f, tf, i;
    tmpj = 0;

    do{
        vi = -1;
        tmpj = 0;

        ei = bfsa[s].e;
        while( ei != -1 && bfse[ei].f == bfse[ei].w ){
            ei = bfse[ei].ne;
        }
        while( ei != -1 ){
            tmpidx[ tmpj ] = ei;

            eit = bfse[ei].ne;
            while( eit != -1 ){
                if( bfse[eit].w > bfse[eit].f ){
                    stack[ stp ].e   = eit;
                    stack[ stp ].idx = tmpj;
                    stp++;
                }
                eit = bfse[eit].ne;
            }

            vi = bfse[ei].ed;
            if( vi == t ){
                tmpj = ( tmpj + 1 ) % MAXSIZE;
                break;
            }
            ei = bfsa[vi].e;
            while( ei != -1 && bfse[ei].f == bfse[ei].w ){
                ei = bfse[ei].ne;
            }
            if( ei == -1 ){
                if( stp == 0 )
                    break;
                stp -= 1;
                ei = stack[ stp ].e;
                tmpj = stack[stp].idx;
            }else
                tmpj = ( tmpj + 1 ) % MAXSIZE;
        }
        if( vi == t ){
            printf("Shortest Path : ");
            for( i = 0; i < tmpj - 1; ++i ){
                ei = tmpidx[i];
                printf("(%d)-E%d-%d-%d-", bfse[ei].st, ei, bfse[ei].w, bfse[ei].f);
            }
            ei = tmpidx[i];
            printf("(%d)-E%d-%d-%d-(%d)", bfse[ei].st, ei, bfse[ei].w, bfse[ei].f, bfse[ei].ed);
            f = bfse[ tmpidx[0] ].w - bfse[ tmpidx[0] ].f;
            for( i = 1; i < tmpj; ++i ){
                ei = tmpidx[i];
                tf = bfse[ei].w - bfse[ei].f;
                if( tf < f )
                    f = tf;
            }
            MF += f;
            printf(" => MF += %d\n", f);
            for( i = 0; i < tmpj; ++i ){
                ei = tmpidx[i];
                bfse[ei].f += f;
            }
        }else{
            nomore = 1;
        }
    }while( nomore == 0 );

    printf("/--- BFS Applied ---/\n");
    print_bfs();
    printf("/--- BFS Applied ---/\n");
}

void update()
{
    printf("/=== Updating ===/\n");
    printf("/--- Before Update ---/\n");
    print_gf();
    printf("/--- Before Update ---/\n");

    int ei, vi, ei2, vi2, eit;

    for( vi = 0; vi < n; ++vi ){
        ei = bfsa[vi].e;
        for( ;ei != -1 ; ei = bfse[ei].ne ){
            if( bfse[ei].f == 0 )
                continue;
            e[ei].w -= bfse[ei].f;
            if( e[ei].w >= 0 ){
                if( ei & 1 ) ei2 = ei - 1;
                else         ei2 = ei + 1;
                if( e[ei2].f == 0 ){
                    e[ei2].f = 1;
                    e[ei2].w  = bfse[ei].f;
                    e[ei2].st = e[ei].ed;
                    e[ei2].ed = e[ei].st;
                    vi2 = e[ei].ed;
                    eit = a[vi2].e;
                    a[vi2].e  = ei2;
                    e[ei2].ne = eit;
                }else{
                    e[ei2].w += bfse[ei].f;
                }
                if( e[ei].w == 0 ){
                    // remove ei from e[ei].st
                    e[ei].f = 0;
                    vi2 = e[ei].st;
                    if( a[vi2].e == ei ){
                        a[vi2].e = e[ei].ne;
                    }else{
                        ei2 = a[vi2].e;
                        while( e[ei2].ne != ei ){
                            ei2 = e[ei2].ne;
                        }
                        e[ei2].ne = e[ei].ne;
                    }
                }
            }else{
                printf("[Error] edge w < 0 : (%d) | E%d | %d | %d | (%d)\n", e[ei].st, ei, e[ei].w, e[ei].f, e[ei].ed);
                exit(-1);
            }
        }
    }

    printf("/--- After  Update ---/\n");
    print_gf();
    printf("/--- After  Update ---/\n");
}

int main()
{
    int i,j;
    int u,v,c;

    for( i = 0; i < MAXSIZE; ++i ){
        a[i].e  = -1;
        e[i].ne = -1;
        e[i].f  = 0;
    }
    for( ; i < 2 * MAXSIZE; ++i ){
        e[i].ne = -1;
        e[i].f  = 0;
    }

    scanf("%d %d %d %d", &n, &m, &s, &t);

    for( i = 0; i < m; ++i ){
        scanf("%d %d %d", &u, &v, &c);
        e[2*i].st = u; e[2*i].ed = v;
        e[2*i].w  = c; e[2*i].f = 1;
        if( a[u].e == -1 ){
            a[u].e = 2*i;
            continue;
        }
        j = a[u].e;
        a[u].e = 2*i;
        e[2*i].ne = j;
    }

    while( bfs() ){
        apply();
        update();
    }

    return 0;
}
