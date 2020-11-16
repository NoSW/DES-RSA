#ifndef _PRIME_H_
#define _PRIME_H_

#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include <assert.h>
#include "macro.h"

#define __DEBUG__ 0
/*
*   There are about N/lnN prime numbers not exceeding N.
*   Randomly select an odd integer p, then P (p is a prime number) = 2*(1/lnp).
*   If p is 128 bits, then the P = 2*(1/ln(2^64)) = 0.04508422003(at least).
*   i.e., the average number of lookups is 22.1807097768 */
typedef __uint128_t bit128;

enum{
    NT_ODD = 0,
    NT_EVEN,
    NT_BIG,
    NT_RAND,
};

#define TEST_TIMES 100

#define HLAF_OVERFLOW ((bit128)(0x10000000000000000U))

static bit128 HALF = 0x0;

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) <= (y)) ? (x) : (y))

// bit operations
#define bit128_set_0(data,n) *(data) &= (~((bit128)(0x1) << ((n)&0x7f)))
#define bit128_set_1(data,n) *(data) |= ((bit128)(0x1) << ((n)&0x7f))
#define get_bit128_n(data, n) !!(((data) >> ((n)&0x7f)) & 0x1)

int bit128_print_hex(bit128 n);
int bit128_print_dec(bit128 n);
void bit128_swap(bit128* a, bit128* b);
bit128 bit128_power_mod(bit128 a, bit128 n, bit128 p);
bit128 bit128_gcd(bit128 n, bit128 b, bit128* inverse);

bit128 BigIntegerGenerator(int n_bits, int parity, int flag);
bit128 BigPrimeGenerator(int n_bits);

// If p is a prime number, return 0, otherwise 1. 
int SolovayStrassenPrimalityTest(bit128 p);
static int JacobiSymbol(bit128 numer, bit128 denom);
int MillerRabinPrimalityTest(bit128 p);
int PrimeTest(bit128 n);
int test(bit128 p);
#endif