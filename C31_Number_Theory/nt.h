#ifndef _NT_H_
#define _NT_H_ 

#include <sys/types.h>

// number type
typedef ssize_t NT_t;
// a mod b
NT_t nt_mod( NT_t a, NT_t b );

// return d = gcd( a, b )
NT_t nt_gcd_euclid( NT_t a, NT_t b );
// d = x * a + y * b
NT_t nt_gcd_extended_euclid( NT_t a, NT_t b , NT_t * x, NT_t * y);

// 
// a * x = b ( mod n )
// 
void nt_modular_linear_equation_solver( NT_t a, NT_t b, NT_t n );

// 
// a ^ b mod n
// 
NT_t nt_modular_exponentiation( NT_t a, NT_t b, NT_t n );

// 
// prime test
// 
int nt_fake_prime_test( NT_t n );
int nt_miler_tabin_prime_test( NT_t n );
int nt_bf_prime_test( NT_t n );

#endif
