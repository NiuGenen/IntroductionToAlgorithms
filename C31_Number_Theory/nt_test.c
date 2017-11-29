#include "nt.h"
#include <getopt.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define RAND_RANGE 100000

void binary_pr( void * value, size_t n )
{
    size_t v = *((size_t*)value);
    char * o = (char* )malloc( n + 1 );
    o[ n ] = '\0';
    for(size_t i = 0; i < n; ++i){
        o[ n - i - 1 ] = ( v & 1 ) ? '1' : '0';
        v = v >> 1;
    }
    printf("%s", o );
    free( o );
}

void test_gcd_euclid( NT_t a, NT_t b )
{
    NT_t d = nt_gcd_euclid( a,b );
    printf("gcd( %5ld, %5ld ) = %5ld\n", a, b, d  );
}

void test_gcd_extended( NT_t a, NT_t b )
{
    NT_t x = 0, y = 0;
    NT_t d = nt_gcd_extended_euclid( a, b, &x, &y );
    printf("gcd = %5ld = %5ld * %5ld + %5ld * %5ld\n", d, x,a, y,b);
}

void test_gcd_ab( NT_t a, NT_t b )
{
    test_gcd_euclid( a,b );
    test_gcd_extended( a,b );
}

void test_gcd()
{
    printf("------ GCD ------\n");
    // random number
    NT_t a = rand() % RAND_RANGE;
    NT_t b = rand() % RAND_RANGE;
    printf("a = %5ld, b = %5ld\n", a, b);
    test_gcd_ab( a,b );
    // manual number
    NT_t A = 3 * 7 * 11 * 17;
    NT_t B = 7 * 11 * 19 * 23;
    printf("A = 7 * 11 * 17 *  3\n");
    printf("B = 7 * 11 * 19 * 23\n");
    printf("A = %5ld, B = %5ld\n", a, b);
    test_gcd_ab( A,B );
}

void test_linear()
{
    printf("------ LINEAR ------\n");
    // 14 * x = 30 ( mod 100 )
    nt_modular_linear_equation_solver( 14, 30, 100 );
}

void test_exponentiation()
{
    printf("------- EXPONENTIATION ------\n");
    NT_t a = 7, b = 560, n = 561;
    printf("b = ");
    binary_pr( &b, sizeof( NT_t ) * 8 );
    printf("\n");
    NT_t d = nt_modular_exponentiation( a,b,n );
    printf("%ld ^ %ld mod %ld = %ld\n", a,b,n,d);
}

void test_prime()
{
    printf("------ PRIME TEST ------\n");
    NT_t i = 2;
    while( i < 10000 ){
        int a = 0, b = 0, c = 0;
        if( nt_fake_prime_test( i ) )        a = 1;
        if( nt_miler_tabin_prime_test( i ) ) b = 1;
        if( nt_bf_prime_test( i ) )          c = 1;
        if( a | b | c ){
            printf("%-5ld : %1d - %1d - %1d\n", i, a,b,c);
        }
        i += 1;
    }
}

const char options_short[] = "glep";

const struct option options_long[] = {
    {"gcd" ,    0,  NULL,  'g' },
    {"linear",  0,  NULL,  'l' },
    {"expon" ,  0,  NULL,  'e' },
    {"prime" ,  0,  NULL,  'p' }
};

int main( int argc, char * argv[] )
{
    int op = 0, g = 0, l = 0, e = 0, p = 0;
    while( ( op = getopt_long( argc, argv, options_short, options_long, NULL ) ) != -1 ){
        switch( op ){
            case 'g': g = 1; break;
            case 'l': l = 1; break;
            case 'e': e = 1; break;
            case 'p': p = 1; break;
            default:  break;
        }
    }

    srand( ( unsigned int )time( 0 ) );

    if( g ) test_gcd();
    if( l ) test_linear();
    if( e ) test_exponentiation();
    if( p ) test_prime();

    return 0;
}
