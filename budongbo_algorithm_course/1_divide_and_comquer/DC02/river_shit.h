#ifndef _RIVER_H_
#define _RIVER_H_

#ifndef NULL 
#define NULL 0
#endif

#define MAX(x,y) ( ((x)>(y)) ? (x) : (y) )

typedef struct md_and_cnt{
    int md;
    int cnt;
    int idx;
    int md2;
    int cnt2;
    int idx2;
} md_and_cnt;

void river( int *num, int hi, int m, md_and_cnt **his, int is_n );

#endif
