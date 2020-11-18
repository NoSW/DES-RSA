#ifndef _RSA_H_
#define _RSA_H_

#include "prime.h"

typedef unsigned long long bit64;

#define INTERVAL 0x4 //1024
#define MINPOWER 0x4 //1024

void RsaKeyPairGenerator(int n_bits, bit128 rsa[]);
void Rsa(bit128* out, bit128 in, bit128 key, bit128 n);

#endif