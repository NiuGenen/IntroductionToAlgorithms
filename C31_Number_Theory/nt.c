#include "nt.h"
#include <stdlib.h>
#include <stdio.h>

NT_t nt_mod( NT_t a, NT_t b )
{
    NT_t ret = a % b;
    while( ret < 0 ) ret += b;
    return ret;
}

NT_t nt_gcd_euclid( NT_t a, NT_t b )
{
    if( b == 0 ) return a;
    else return nt_gcd_euclid( b, nt_mod( a, b ) );
}

NT_t nt_gcd_extended_euclid( NT_t a, NT_t b, NT_t * x, NT_t * y )
{
    if( b == 0 ){
        *x = 1;
        *y = 0;
        return a;
    }else{
        NT_t xx = 0, yy = 0;
        NT_t dd = nt_gcd_extended_euclid( b, nt_mod( a,b ), &xx, &yy );
        // dd = xx * b + yy * ( a mod b )
        *x = yy;
        *y = xx - ( a / b ) * yy;
        return dd;
    }
}

void nt_modular_linear_equation_solver( NT_t a, NT_t b, NT_t n )
{
    printf("%ld * x = %ld ( mod %ld )\n", a, b, n);
    NT_t d = 0, x = 0, y = 0;
    d = nt_gcd_extended_euclid( a, n, &x, &y );
    printf("gcd(%ld,%ld) = %ld = %ld * %ld + %ld * %ld\n", a,n,d,x,a,y,n);
    if( nt_mod( b, d ) == 0 ){
        NT_t x0 = nt_mod( x * ( b / d ) , n );
        NT_t i = 0;
        while( i < d ){
            printf("x%-3ld = %-5ld\n", i, nt_mod( x0 + i * ( n / d ) , n ) );
            i += 1;
        }
    }
    else{
        printf("No solutions\n");
    }
}

NT_t nt_binary_bits( NT_t num )
{
    NT_t c = 1, ret = 0;
    NT_t max = sizeof( NT_t ) * 8;
    while( !( num < c ) && ret < max ){
        c = c << 1;
        ret += 1;
    }
    return ret > max ? max : ret ;
}

NT_t nt_modular_exponentiation( NT_t a, NT_t b, NT_t n )
{
    NT_t c = 0;
    NT_t d = 1;
    NT_t k = nt_binary_bits( b );
    NT_t i = 0;
    while( i < k ){
        c = 2 * c;
        d = nt_mod( d * d, n );
        if( ( b >> ( k - i - 1 ) ) & 1 ){
            c += 1;
            d = nt_mod( d * a, n );
        }
        i += 1;
    }
    return d;
}

int nt_fake_prime_test( NT_t n )
{
    if( n == 2 ) return 1;
    NT_t t = nt_modular_exponentiation( 2, n-1, n ); // ( 2 ^ ( n - 1 ) ) mod n
    if( nt_mod( t,n ) != 1 ) return 0; // definitely not prime
    else return 1; // great chance to be prime
}

int nt_witness( NT_t a, NT_t n )
{
    NT_t u = n - 1, t = 0;
    while( nt_mod( u, 2 ) == 0 ){
        u = u / 2;
        t += 1;
    }
    NT_t x = nt_modular_exponentiation( a,u,n ); // a ^ u mod n
    NT_t i = 1;
    while( i <= t ){
        NT_t _x = x;
        x = nt_mod( _x * _x, n );
        if( x == 1 && _x != 1 && _x != n-1 ) return 1;
        i += 1;
    }
    // NT_t x = nt_modular_exponentiation( a, n-1 ,n ); // ( a ^ ( n -1 ) ) mod n
    if( x != 1 ) return 1; // not prime
    return 0;
}

int nt_miler_tabin_prime_test( NT_t n )
{
    int s = 10;
    while( s > 0 ){
        NT_t a = rand() % ( n - 1 ) + 1;
        if( nt_witness( a,n ) ) return 0;
        s -= 1;
    }
    return 1;
}

int nt_bf_prime_test( NT_t n )
{
    for( NT_t i = 2; i <= n / 2; ++i ){
        if( nt_mod( n, i ) == 0 ){
            /*printf("%5ld %% %5ld == 0\n", n, i);*/
            return 0;
        }
    }
    /*printf("%5ld is prime\n", n);*/
    return 1;
}
