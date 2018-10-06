#include <stdio.h>
#include "closest_pair.h"
/*
int main()
{
    int n;
    int i;
    fp_t p[ 2 * 100000 ];
    fp_t p_cp[ 2 * 100000 ];

    scanf("%d", &n);

    for( i = 0; i < n; ++i ){
        scanf("%lf %lf", &( p[ 2 * i ] ), &( p[ 2 * i + 1 ]) );
    }

    printf("%.2lf\n", closest_pair(p,n,p_cp) );

    return 0;
}*/
int main()
{
    int n;
    int i;

    fp_t p[ 2 * 100000 ];
    fp_t p_cp[ 2 * 100000 ];
    /*fp_t p_x[ 100000 ];*/
    /*fp_t p_y[ 100000 ];*/
    /*fp_t p_cp_x[ 100000 ];*/
    /*fp_t p_cp_y[ 100000 ];*/

    scanf("%d", &n);

    /*for( i = 0; i < n; ++i ){*/
        /*scanf("%lf %lf", &( p_x[ i ] ), &( p_y[ i ]) );*/
    /*}*/

    for( i = 0; i < n; ++i )
    {
        scanf("%lf %lf", p + 2 * i, p + 2 * i + 1 );
    }

    /*printf("%.2lf\n", closest_pair(p_x,p_y,n,p_cp_x,p_cp_y) );*/
    printf("%.2lf\n", closest_pair(p, n,p_cp ));

    return 0;
}
