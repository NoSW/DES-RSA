#ifndef _RSA_H_
#define _RSA_H_

#include "prime.h"

typedef unsigned long long bit64;

#define INTERVAL 0xffff
#define MODE_ENCODE 0
#define MODE_DECODE 1

void RasInitial();
void Rsa(bit128* out, bit128 in, bit128 key, bit128 n);

#endif