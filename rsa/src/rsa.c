#include "../inc/rsa.h"

void
RsaKeyPairGenerator(int n_bits, bit128* e, bit128* d, bit128* n)
{
    bit128 p, q, Phi_n;

    //  1. Select two big prime(64bit), assigned to p and q,
    //  and |p - q| can't be too small.
    printf("Search the 1st prime ...\n");
    p = BigPrimeGenerator(n_bits/2);
    q = p;
    printf("Search the 2nd prime ...\n");
    while ((p>=q) && (p-q) < INTERVAL ||
        p < q && (q-p) < INTERVAL)
            q = BigPrimeGenerator(n_bits/2);

    //  2. Calculate n = p * q and the euler function Phi(n) for n.
    *n = p * q;
    Phi_n = (p-1) * (q-1);

    //  3. Select d randomly and satisfy (d, Phi(n)) == 1.
    //  4. Get e , which satisfies condition (e * d) =  1 (mod Phi_n).
    do {
        *d = BigIntegerGenerator(n_bits/2, NT_RAND, NT_RAND);
    } while (bit128_gcd(*d, Phi_n, e) != 1);
}

void
Rsa(bit128* out, bit128 in, bit128 key, bit128 n)
{
    *out = bit128_power_mod(in, key, n);
}