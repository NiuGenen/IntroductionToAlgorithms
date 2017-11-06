#include <stdio.h>
#include <stdlib.h>

#ifndef NULL
#define NULL 0
#endif 

#define _MALLOC_(t,n) ( (t*)malloc( sizeof(t) * (n) ) )
#define _FREE_(p) do{ \
    if( p != NULL ){  \
        free(p);      \
        p = NULL;     \
    }                 \
}while(0);

int  find ( int x        );
void join ( int x, int y );

void init ( int size     );
void set  ( int size     );
void data (              );
void clear(              );
void res  (              );

int _SIZE_ = 1024;

int* prev;
int* mark;

int find( int node )
{
    // find root
    int root = node;
    while( prev[ root ] != root  ){
        root = prev[ root ];
    }
    // set all node's prev as root
    int n = node;
    int tmp;
    while( n != root ){
        tmp = prev[n];
        prev[n] = root; 
        n = tmp;
    }
    return root;
}

void join( int x, int y )
{
    int ix = find( x );
    int iy = find( y );
    if( ix != iy ){
        prev[ ix ] = iy; // connect root
    }
}

void set( int size )
{
    if( prev != NULL ){
        _FREE_( prev );
    }
    if( mark != NULL ){
        _FREE_( mark );
    }
    _SIZE_ = size;
    prev = _MALLOC_( int, _SIZE_ );
    mark = _MALLOC_( int, _SIZE_ );
    clear();
}

void clear()
{
    for( int i = 0; i < _SIZE_; ++i ){
        prev[i] = i;
        mark[i] = 0;
    }   
}

// 
//   1--2
//   |
//   3  4
//     /|
//    / |
//   5  0
// 
void data()
{
    join(0,4);
    join(1,2);
    join(1,3);
    join(4,5);
}

void res()
{
    int res = 0;
    for( int i = 0; i < _SIZE_; ++i ){
        int r = find(i);
        if( !mark[r] ){
            res += 1;
            mark[r] = 1;
        }
    }
    printf("Res = %d\n", res);
}

int main()
{
    set(6);
    data();
    res();
}
