#include <stdio.h>
#include <stdlib.h>
#include "closest_pair.h"

int main( int argc, char *argv[] )
{
    int n,i;
    if( argc < 2 ) return 1;

    n = atoi( argv[1] );

    fp_t p[ 2 * 100000 ];
    fp_t p_cp[ 2 * 100000 ];
    /*fp_t p_x[ 100000 ];*/
    /*fp_t p_y[ 100000 ];*/
    /*fp_t p_cp_x[ 100000 ];*/
    /*fp_t p_cp_y[ 100000 ];*/

    /*for( i = 0; i < n; ++i ){*/
        /*p_x[ i ] = atof( argv[ 2 + 2 * i ] );*/
        /*p_y[ i ] = atof( argv[ 2 + 2 * i + 1 ] );*/
    /*}*/

    for( i = 0; i < n; ++i ){
        p[ 2 * i     ] = atof( argv[ 2 + 2 * i     ] );
        p[ 2 * i + 1 ] = atof( argv[ 2 + 2 * i + 1 ] );
    }

    /*printf("%.2f\n", closest_pair(p_x, p_y, n, p_cp_x, p_cp_y) );*/
    /*printf("%.2f\n", native_closest_pair(p_x, p_y,n));*/

    printf("%.2f\n", closest_pair(p, n, p_cp) );
    printf("%.2f\n", native_closest_pair(p,n));

    return 0;
}
