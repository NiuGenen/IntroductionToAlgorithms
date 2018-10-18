#include <stdio.h>

typedef unsigned int ct_t;

#define SWAPINT(x,y) do{\
    ct_t t = x;\
    x = y;\
    y = t;\
}while(0);

ct_t tmp[100000];

ct_t ct(ct_t *num, ct_t lo, ct_t hi)
{
    if( lo >= hi ) return 0;
    if( lo+1 == hi ){
        ct_t ret = 0;
        if(num[lo] > num[hi]){
            if(num[lo] > num[hi]*3) ret = 1;
            SWAPINT(num[lo],num[hi]);
        }
        return ret;
    }

    ct_t mid = (lo + hi) / 2;
    ct_t left  = ct(num,lo,mid);   // Left  Part : [lo]    [...] [mid]
    ct_t right = ct(num,mid+1,hi); // Righr Part : [mid+1] [...] [hi]
    ct_t my    = 0;
    ct_t li = lo;
    ct_t ri = mid + 1;
    while( li <= mid && ri <= hi ){
        if(num[li] > num[ri]*3){
            my += mid - li + 1;
            ri += 1;
        }
        else li += 1;
    }
    ct_t tmpi = 0;
    li = lo; ri = mid + 1;
    while( li <= mid && ri <= hi ){
        if(num[li] > num[ri])
            tmp[tmpi++] = num[ri++];
        else
            tmp[tmpi++] = num[li++];
    }
    while( li <= mid ) tmp[tmpi++] = num[li++];
    while( ri <= hi  ) tmp[tmpi++] = num[ri++];
    li = lo;
    tmpi = 0;
    while( li <= hi ) num[li++] = tmp[tmpi++];

    return left + right + my;
}

int main()
{
    ct_t n;
    ct_t num[100000];
    ct_t i;

    scanf("%d", &n);
    for( i = 0; i < n; ++i ){
        scanf("%d", num + i);
    }

    printf("%d\n", ct(num,0,n-1));

    return 0;
}
