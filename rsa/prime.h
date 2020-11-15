#ifndef _PRIME_H_
#define _PRIME_H_

#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include<math.h>
#include "macro.h"
/*
*   There are about N/lnN prime numbers not exceeding N.
*   Randomly select an odd integer p, then P (p is a prime number) = 2*(1/lnp).
*   If p is 128 bits, then the P = 2*(1/ln(2^64)) = 0.04508422003(at least).
*   i.e., the average number of lookups is 22.1807097768 */
typedef __uint128_t bit128;

#define TEST_TIMES 100

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) > (y)) ? (y) : (x))

// bit operations
#define bit128_set_0(data,n) *(data) &= (~((bit128)(0x1) << ((n)&0x7f)))
#define bit128_set_1(data,n) *(data) |= ((bit128)(0x1) << ((n)&0x7f))
#define get_bit128_n(data, n) !!(((data) >> ((n)&0x7f)) & 0x1)

bit128 BigPrimeGenerator(int n_bits);
void bit128_swap(bit128* a, bit128* b);
bit128 bit128_power_mod(bit128 a, bit128 n, bit128 p);
bit128 bit128_gcd(bit128 a, bit128 b);
bit128 BigOddIntegerGenerator(int n_bits);
static int JacobiSymbol(bit128 numer, bit128 denom);

// If p is a prime number, return 0, otherwise 1. 
int SolovayStrassenPrimalityTest(bit128 p);
int MillerRabinPrimalityTest(bit128 p);
int bit128_print(bit128 n);
int PrimeTest(bit128 n);
int test(bit128);
#endif