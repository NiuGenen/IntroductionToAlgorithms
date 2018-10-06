#include "closest_pair.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*
void qsort_pnts( fp_t *p_x, fp_t *p_y, int lo, int hi )
{
    if( lo < hi ){
        fp_t pivot_x = p_x[ lo ];
        fp_t pivot_y = p_y[ lo ];
        int i = lo, j = hi;
        while( i < j )
        {
            while( i < j && pivot_x <= p_x[ j ] ) j -= 1;
            p_x[ i ] = p_x[ j ];
            p_y[ i ] = p_y[ j ];
            while( i < j && pivot_x >= p_x[ i ] ) i += 1;
            p_x[ j ] = p_x[ i ];
            p_y[ j ] = p_y[ i ];
        }
        p_x[ i ] = pivot_x;
        p_y[ i ] = pivot_y;
        qsort_pnts(p_x, p_y, lo, i-1);
        qsort_pnts(p_x, p_y, i+1, hi);
    }
}

fp_t closest_pair_cursion( fp_t *p_x, fp_t *p_y, int lo, int hi , fp_t *p_cp_x, fp_t *p_cp_y)
{
    if( lo + 1 == hi ){
        return DISTANCE(p_x, p_y, lo, hi);
    }

    if( lo + 2 == hi ){
        fp_t dis   = DISTANCE(p_x,p_y,lo,lo+1);
        fp_t dis_  = DISTANCE(p_x,p_y,lo+1,hi); 
        fp_t dis__ = DISTANCE(p_x,p_y,lo,hi);
        dis = dis<dis_?dis:dis_;
        dis = dis<dis__?dis:dis__;
        return dis;
    }

    int   i          =  0;
    int   j          =  0;

    int   mid        =  ( lo + hi ) / 2;
    fp_t  dis_l      =  closest_pair_cursion( p_x, p_y, lo, mid , p_cp_x, p_cp_y);
    fp_t  dis_r      =  closest_pair_cursion( p_x, p_y, mid + 1, hi , p_cp_x, p_cp_y);

    fp_t  sigema_2   =  MIN( dis_l, dis_r );
    fp_t  sigema     =  sqrt( sigema_2 );

    int   left_idx   =  mid - 1;
    int   right_idx  =  mid + 1;
    fp_t  mid_x      =  p_x[ mid ];

    while( left_idx >= lo ) {
        if( mid_x - p_x[ left_idx ] < sigema ){
            left_idx -= 1;
        }else{
            left_idx += 1;
            break;
        }
    }
    if( left_idx < lo ) left_idx = lo;

    while( right_idx <= hi ) {
        if( p_x[ right_idx ] - mid_x < sigema ){
            right_idx += 1;
        }else{
            right_idx -= 1;
            break;
        }
    }
    if( right_idx > hi ) right_idx = hi;

    // reuse mid as the length of comparing points
    mid = right_idx - left_idx + 1;

    for( i = left_idx; i <= right_idx ; ++i){
        p_cp_x[  i - left_idx  ] = p_x[ i ];
        p_cp_y[  i - left_idx  ] = p_y[ i ];
    }
    qsort_pnts( p_cp_y, p_cp_x, 0, right_idx - left_idx );

    for( i = 0; i < mid; ++i){
        for( j = i + 1; j <= i + 11 && j < mid; ++j ){
            // reuse dis_l as the distance 
            if( fabs(p_cp_x[i]-p_cp_x[j]) > sigema || fabs(p_cp_y[i]-p_cp_y[j]) > sigema ) continue;
            dis_l = DISTANCE(p_cp_x, p_cp_y, i, j);
            sigema_2 = sigema_2 < dis_l ? sigema_2 : dis_l;
        }
    }

    return sigema_2;
}

fp_t closest_pair( fp_t *p_x, fp_t *p_y, int n , fp_t *p_cp_x, fp_t *p_cp_y)
{
    qsort_pnts(p_x, p_y,0,n-1);
    return sqrt( closest_pair_cursion(p_x, p_y,0,n-1, p_cp_x, p_cp_y) );
}
*/

// 0  [0]  [1]
// 1  [2]  [3]
// 2  [4]  [5]
// 3  [6]  [7]
// .
// n  [2n] [2n+1]

int cmp_fp_t( const void *a, const void *b )
{
    const fp_t *p1 = (fp_t*)a;
    const fp_t *p2 = (fp_t*)b;
    return *p1 > *p2;
}

void qsort_pnts( fp_t *p, int lo, int hi )
{
    qsort( &(p[lo]), hi - lo + 1, sizeof(fp_t) * 2, cmp_fp_t );
    /*if( lo < hi ){*/
        /*fp_t pivot   = p[ 2 * lo ];*/
        /*fp_t pivot_y = p[ 2 * lo + 1 ];*/
        /*int i = lo, j = hi;*/
        /*while( i < j )*/
        /*{*/
            /*while( i < j && pivot <= p[ 2 * j ] ) j -= 1;*/
            /*p[ 2 * i     ] = p[ 2 * j ];*/
            /*p[ 2 * i + 1 ] = p[ 2 * j + 1 ];*/
            /*while( i < j && pivot >= p[ 2 * i ] ) i += 1;*/
            /*p[ 2 * j     ] = p[ 2 * i ];*/
            /*p[ 2 * j + 1 ] = p[ 2 * i + 1 ];*/
        /*}*/
        /*p[ 2 * i     ] = pivot;*/
        /*p[ 2 * i + 1 ] = pivot_y;*/
        /*qsort_pnts(p, lo, i-1);*/
        /*qsort_pnts(p, i+1, hi);*/
    /*}*/
}

fp_t closest_pair_cursion( fp_t *p, int lo, int hi , fp_t *p_cp)
{
    if( lo + 1 == hi ){
        return DISTANCE(p,lo,hi);
    }

    if( lo + 2 == hi ){
        fp_t dis   = DISTANCE(p,lo,lo+1);
        fp_t dis_  = DISTANCE(p,lo+1,hi); 
        fp_t dis__ = DISTANCE(p,lo,hi);
        dis = dis<dis_?dis:dis_;
        dis = dis<dis__?dis:dis__;
        return dis;
    }

    int   i          =  0;
    int   j          =  0;

    int   mid        =  ( lo + hi ) / 2;
    fp_t  dis_l      =  closest_pair_cursion( p, lo, mid , p_cp);
    fp_t  dis_r      =  closest_pair_cursion( p, mid + 1, hi , p_cp);

    fp_t  sigema_2   =  MIN( dis_l, dis_r );
    fp_t  sigema     =  sqrt( sigema_2 );

    int   left_idx   =  mid - 1;
    int   right_idx  =  mid + 1;
    fp_t  mid_x      =  p[ 2 * mid ];

    while( left_idx >= lo ) {
        if( mid_x - p[ 2 * left_idx ] < sigema ){
            left_idx -= 1;
        }else{
            left_idx += 1;
            break;
        }
    }
    if( left_idx < lo ) left_idx = lo;

    while( right_idx <= hi ) {
        if( p[ 2 * right_idx ] - mid_x < sigema ){
            right_idx += 1;
        }else{
            right_idx -= 1;
            break;
        }
    }
    if( right_idx > hi ) right_idx = hi;

    // reuse mid as the length of comparing points
    mid = right_idx - left_idx + 1;

    for( i = left_idx; i <= right_idx ; ++i){
        p_cp[ 2 *( i - left_idx )     ] = p[ 2 * i + 1 ];
        p_cp[ 2 *( i - left_idx ) + 1 ] = p[ 2 * i ];
    }
    qsort_pnts( p_cp, 0, right_idx - left_idx );

    for( i = 0; i < mid; ++i){
        for( j = i + 1; j <= i + 11 && j < mid; ++j ){
            // reuse dis_l as the distance 
            dis_l = DISTANCE(p_cp, i, j);
            sigema_2 = sigema_2 < dis_l ? sigema_2 : dis_l;
        }
    }

    return sigema_2;
}

fp_t closest_pair( fp_t *p, int n , fp_t *p_cp)
{
    qsort_pnts(p,0,n-1);
    return sqrt( closest_pair_cursion(p,0,n-1, p_cp) );
}


/*fp_t native_closest_pair( fp_t *p_x, fp_t *p_y, int n )*/
/*{*/
    /*int i = 0;*/
    /*int j = 0;*/
    /*fp_t dis = DISTANCE(p_x, p_y,0,1);*/
    /*fp_t tmp;*/
    /*for( i = 0; i < n; ++i ){*/
        /*for( j = i + 1; j < n; ++j ){*/
            /*tmp = DISTANCE(p_x, p_y,i,j);*/
            /*dis = dis < tmp ? dis : tmp;*/
        /*}*/
    /*}*/
    /*return sqrt(dis);*/
/*}*/

fp_t native_closest_pair( fp_t *p, int n )
{
    int i = 0;
    int j = 0;
    fp_t dis = DISTANCE(p,0,1);
    fp_t tmp;
    for( i = 0; i < n; ++i ){
        for( j = i + 1; j < n; ++j ){
            tmp = DISTANCE(p,i,j);
            dis = dis < tmp ? dis : tmp;
        }
    }
    return sqrt(dis);
}
