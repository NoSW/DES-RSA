#include<stdio.h>
#include"rsa.h"



int main()
{

    srand((unsigned)time(NULL));
    RsaKeyPairGenerator(64);
}