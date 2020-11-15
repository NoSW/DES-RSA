#include "rsa.h"

void
RasInitial()
{
    /*
    *   n, e is public
    *   d is secret
    *   p, q, Phi_n must be destroyed or kept secret
    */
    bit128 p, q, n, d, e, Phi_n;
    p = BigPrimeGenerator(64);
    q = p;

    while ((p>q) && (p-q) < INTERVAL ||
        p <= q && (q-p) < INTERVAL)
            q = BigOddIntegerGenerator(64);
    n = p*q;
    Phi_n = (p-1)*(q-1);
    do {
        d = BigOddIntegerGenerator(64);
    } while (bit128_gcd(d, Phi_n) != 1);

    for(int i = 1; i < Phi_n; i++)
        if( (i*d) % Phi_n == 1)
        {
            e = i;
            break;
        }
    
    // TEST
    PRINT_BIT128(p)
    PRINT_BIT128(q)
    PRINT_BIT128(n)
    PRINT_BIT128(d)
    PRINT_BIT128(e)
    bit128 encoded, decoded;
    bit128 in = (bit128)(0x123456789abcdef0);
    PRINT_BIT128(in);
    encoded = bit128_power_mod(in, e, n);
    PRINT_BIT128(encoded);
    decoded = bit128_power_mod(encoded, d, n);
    PRINT_BIT128(decoded);
}

void
Rsa(bit128* out, bit128 in, bit128 key, bit128 n)
{
    *out = bit128_power_mod(in, key, n);
}