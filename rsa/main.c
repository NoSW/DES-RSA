#include<stdio.h>
#include"rsa.h"



int main()
{

    srand((unsigned)time(NULL));

    for( int i  =0 ; i < 100 ; i ++)
        BigPrimeGenerator(64);    

    
}