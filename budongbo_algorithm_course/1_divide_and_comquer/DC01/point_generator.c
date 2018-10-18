#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

const char *file = "./asd";

int main( int argc, char *argv[] )
{
    if( argc < 2 ) return 1;

    int n = atoi(argv[1]);
    int i = 0;
    double ran;

    int fd = open(file, O_RDWR | O_CREAT , 0777 );
    if( fd < 0 ){
        printf("Fail to create file %s\n", file);
        return 1;
    }

    write( fd, &n, sizeof(int) );

    for( i = 0; i < n; ++i )
    {
        ran = rand() * 100000;
        printf("%lf ", ran);
        write( fd, &ran, sizeof(double) );

        ran = rand() * 100000;
        printf("%lf ", ran);
        write( fd, &ran, sizeof(double) );
    }
    printf("\n");

    close(fd);

    return 0;
}
