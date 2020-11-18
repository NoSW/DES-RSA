#include "../inc/rsa.h"
#include "../inc/cmd.h"

void
RsaKeyPairGenerator(int n_bits, bit128 rsa[])
{
    //  1. Select two big prime(64bit), assigned to p and q,
    //  and |p - q| can't be too small.
    printf("Search the 1st prime ...\n");
    rsa[RSA_P] = BigPrimeGenerator(n_bits/2);
    rsa[RSA_Q] = rsa[RSA_P];
    printf("Search the 2nd prime ...\n");
    while ((rsa[RSA_P]>=rsa[RSA_Q]) && (rsa[RSA_P]-rsa[RSA_Q]) < INTERVAL ||
        rsa[RSA_P] < rsa[RSA_Q] && (rsa[RSA_Q]-rsa[RSA_P]) < INTERVAL)
            rsa[RSA_Q] = BigPrimeGenerator(n_bits/2);

    //  2. Calculate n = p * q and the euler function Phi(n) for n.
    rsa[RSA_N] = rsa[RSA_P] * rsa[RSA_Q];
    rsa[RSA_PhiN] = (rsa[RSA_P]-1) * (rsa[RSA_Q]-1);

    //  3. Select d randomly and satisfy (d, Phi(n)) == 1.
    //  4. Get e , which satisfies condition (e * d) =  1 (mod Phi_n).
    do {
        rsa[RSA_D] = BigIntegerGenerator(n_bits/2, NT_RAND, NT_RAND);
    } while (bit128_gcd(rsa[RSA_D], rsa[RSA_PhiN], &rsa[RSA_E]) != 1);
}

void
Rsa(bit128* out, bit128 in, bit128 key, bit128 n)
{
    *out = bit128_power_mod(in, key, n);
}