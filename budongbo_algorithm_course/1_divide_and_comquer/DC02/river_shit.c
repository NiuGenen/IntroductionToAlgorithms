#include "river.h"
#include <stdio.h>

// Shit! I misunderstood the problem!

void river( int *num, int hi, int m, md_and_cnt **hsty, int is_n )
{
    if( m > hi ) return;
    // hi == 1 ( n == 0 ) : m == 0
    // hi == 2 ( n == 1 ) : m == 0/1
    // hi == 3 ( n == 2 ) : m == 0/1/2
    int *md   = &(hsty[hi][m].md);
    int *cnt  = &(hsty[hi][m].cnt);
    int *idx  = &(hsty[hi][m].idx);
    int *md2  = &(hsty[hi][m].md);
    int *cnt2 = &(hsty[hi][m].cnt);
    int *idx2 = &(hsty[hi][m].idx2);

    int i     = 0;
    int md_1  = 0, cnt_1 = 0, idx_1 = 0;
    int md_2  = 0, cnt_2 = 0, idx_2 = 0;
    int tmp   = 0;
    int md_t1  = 0, cnt_t1 = 0, idx_t1 = 0;
    int md_t2  = 0, cnt_t2 = 0, idx_t2 = 0;

    if( m == hi ){
        *md  = 0; *md2  = 0;
        *cnt = 0; *cnt2 = 0;
        *idx = 0; *idx2 = 0;
        goto _OUT_PRINT_;
    }
    // require : num is sorted and start from 0 to hi
    if( m == 0 ){
        // m == 0, to calculate MinimunDistance
        md_1  = num[1] - num[0];
        cnt_1 = 1;
        idx_1 = hi;
        for( i = 1; i <= hi; ++i ){
            tmp = num[ i + 1 ] - num[ i ];
            if( tmp < md_1 ){
                md_1  = tmp;
                cnt_1 = 1;
            }else if( tmp == md_1 ){
                cnt_1 += 1;
            }
        }
        md_2 = 0; cnt_2 = 0; idx_2 = 0;
        goto _OUT_;
    }
    // 0 < m < hi, to enumerate all possiable way
    // 
    // for num[hi]
    // 
    // 1. remove : goto river( num, hi - 1, m - 1 )
    //             if is_n, then compare with num[n+1] - num[idx] when hi == n
    md_1  = hsty[hi-1][m-1].md;
    cnt_1 = hsty[hi-1][m-1].cnt;
    idx_1 = hsty[hi-1][m-1].idx;
    if( is_n ){
        tmp = num[hi+1] - num[idx_1];
        if( md_1 == tmp ){
            cnt_1 += 1;
        }else if( md_1 > tmp ){
            md_1  = tmp;
            cnt_1 = 1;
        }
        md_t1 = hsty[hi-1][m].md2;
        if( md_t1 != 0 && md_t1 >= tmp ){
            cnt_t1 = hsty[hi-1][m].cnt2;
            idx_t1 = hsty[hi-1][m].idx2;
            if( md_t1 == tmp ){

            }
        }
    }
    // 2. stay : goto river( num, hi - 1, m )
    //           then compare its MinimunDistance with num[hi] - num[hi -1]
    //           if is_n , then compare with num[n+1] - num[hi] when hi == n
    md_2  = hsty[hi-1][m].md;
    cnt_2 = hsty[hi-1][m].cnt;
    idx_2 = hsty[hi-1][m].idx;
    tmp   = num[hi] - num[idx_2];
    if( md_2 == 0 ){
        md_2  = tmp;
        cnt_2 = 1;
    }else if( md_2 == tmp ){
        cnt_2 += 1;
    }else if( md_2 > tmp ){
        md_2  = tmp;
        cnt_2 = 1;
    }
    idx_2 = hi;
    if( is_n ){
        tmp = num[hi+1] - num[hi];
        if( md_2 == tmp ){
            cnt_2 += 1;
        }else if( md_2 > tmp ){
            md_2  = tmp;
            cnt_2 = 1;
        }
    }
    // compare cnt_1 with cnt_2
    if( cnt_2 > cnt_1 ){
        cnt_1 = cnt_2;
        md_1  = md_2;
        idx_1 = idx_2;
    }else if( cnt_2 == cnt_1 ){
        if( md_2 < md_1 ){
            cnt_1 = cnt_2;
            md_1  = md_2;
            idx_1 = idx_2;
        }
    }
_OUT_:
    *md  = md_1 ; *md2  = md_2;
    *cnt = cnt_1; *cnt2 = cnt_2;
    *idx = idx_1; *idx2 = idx_2;
_OUT_PRINT_:
    printf("[%2d][%2d] = { %d,%d,%d} , {%d,%d,%d} , is_n = %d\n",
            hi  ,   m    ,
            *md ,   *cnt ,   *idx ,
            *md2,   *cnt2,   *idx2,
            is_n
            );
}
