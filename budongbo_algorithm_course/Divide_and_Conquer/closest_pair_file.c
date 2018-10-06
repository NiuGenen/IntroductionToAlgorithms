#include <stdio.h>
#include "closest_pair.h"
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

const char *file = "./asd";

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

    int fd = open( file, O_RDONLY );
    if( fd < 0 ){
        printf("Fail open file %s\n", file);
        return 1;
    }

    read( fd, &n, sizeof(int) );
    /*for( i = 0; i < n; ++i ){*/
        /*read( fd, &(p_x[i]), sizeof(double) );*/
        /*read( fd, &(p_y[i]), sizeof(double) );*/
    /*}*/
    for( i = 0; i < n; ++i ){
        read( fd, p + 2 * i, sizeof(fp_t) );
        read( fd, p + 2 * i + 1, sizeof(fp_t) );
    }

    clock_t st,ed;

    st = clock();
    /*printf("%.2lf\n", closest_pair(p_x, p_y, n, p_cp_x, p_cp_y) );*/
    printf("%.2lf\n", closest_pair(p, n, p_cp) );
    ed = clock();
    printf("DC clock = %ld\n", ed - st);

    st = clock();
    /*printf("%.2f\n", native_closest_pair(p_x, p_y, n));*/
    printf("%.2f\n", native_closest_pair(p, n));
    ed = clock();
    printf("BP clock = %ld\n", ed - st);

    close(fd);

    return 0;
}
