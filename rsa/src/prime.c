#include "../inc/prime.h"

typedef unsigned long long bit64;
static bit128 LIMIT = (bit128)(0x1) << 0x40;

int test(bit128 p)
{
    if(p < 2)
        return 1;
    
    for (int i = 2;  i < p/2; i++)
   {
       if(p%i == 0)
        return 1;
   } 
   return 0;
}

bit128 
BigPrimeGenerator(int n_bits)
{
    bit128 p;
    int total = 0;
    do {
        p = BigIntegerGenerator(n_bits, NT_ODD, NT_BIG);

        total++;
        int ss_test = SolovayStrassenPrimalityTest(p);
        /* TEST */
        // int mr_test = MillerRabinPrimalityTest(p);
        // int pass_test = test(p);
        // if(pass_test != ss_test || pass_test != mr_test)
        // {
        //     if(pass_test == 0)
        //         printf("\033[1;31mERROR:\t\033[0m\t");
        //     else printf("\033[1;33mMISSING:\33[0m");
        //     bit128_print(p, P_HEX, NULL);
        //     if(pass_test != ss_test)
        //         printf("SolovayStrassen\n");
        //     if(pass_test != mr_test)
        //         printf("MillerRabin\n");
        // }
        if( ss_test == 0){
            //printf("\033[1;32mGOOD:\033[0m");
            //bit128_print(p, P_HEX, NULL);
            //printf("Number of odd numbers searched: %d\n", total);
            return p;
        }
        
    }while(1);
}

void
bit128_swap(bit128* a, bit128* b)
{
    bit128 temp = *a;
    *a = *b;
    *b = temp;
}


bit128 
bit128_product_mod(bit128 a, bit128 b, bit128 p)
{
    //return (a * n) % p

    bit128 max = MAX(a, b);
    bit128 min = MIN(a, b);

    if(min == 0 || min == 1)
        return (max * min) % p;

    bit128 ret = bit128_product_mod(max, min/2, p);
    if(ret > p/2)
        ret = ret -  (p - ret);
    else ret = (ret+ret) % p;

    if(min%2 == 1)
    {
        if(ret > (p-max))
            ret =  ret - (p - max);
        else ret = (ret + max) % p;
    }
    
    return ret;

}

bit128 
bit128_power_mod(bit128 a, bit128 n, bit128 p)
{
    if(p == 0)
        return 0;  // ERROR
    if(n == 1)
        return a % p;
    if(n == 0)
        return 1;

    bit128 ret;
    ret = bit128_power_mod(a, n/2, p);

    if(ret >= LIMIT)
        ret = bit128_product_mod(ret, ret, p);
    else ret = (ret * ret) % p;

    if(n % 2)
    {
        if(ret >= LIMIT || a >= LIMIT)
            ret = bit128_product_mod(ret, a, p);
        else ret = (ret * a) % p;
    }

    return ret;
}

bit128
bit128_gcd(bit128 n, bit128 b, bit128* inverse)
{
    bit128 max = MAX(n, b);
    bit128 min = MIN(n, b);
    bit128 r = 0, q = 0;
    bit128 t = max;
    bit128 s = min;

    bit128 t_2 = 0, t_1 = 1, temp = 0;
    int sign_t_2 = 1, sign_t_1 = 1, sign_temp = 1;
    // r = q * t + s
    do
    {
        r = t;
        t = s;
        q = r / t;
        s = r - q * t;
        // Calculate inverse
        temp = t_1;
        sign_temp = sign_t_1;
        
        bit128 product_q_t1 = bit128_product_mod(q, t_1, max);
        if(sign_t_1 == sign_t_2)
        {
            if(t_2 < product_q_t1)
            {
                t_1 = product_q_t1 - t_2;
                sign_t_1 = -sign_t_1;
            } else t_1 = t_2 - product_q_t1;
        } else if(sign_t_1 != sign_t_2)
        {
            t_1 = t_2 + product_q_t1;
            sign_t_1 = -sign_t_1;
        }

        t_2 = temp;
        sign_t_2 = sign_temp;
    
    } while(s != 0);

    if(t == 1 && inverse){
        if(sign_t_2 == -1)
        {
            while(t_2 > max) t_2 -= max;
            t_2 = max - t_2;
        }
        *inverse = t_2 % max;
    }

    return t;

}

//   'parity': NT_ODD, NT_EVEN, NT_RAND
//   'flag': NT_BIG, NT_RAND
bit128 
BigIntegerGenerator(int n_bits, int parity, int flag)
{
    //  Set 0 or 1 at random on bit i, i in [0, n_bit).
    bit128 ret = 0x0;
    for(int i = 0; i < n_bits; i++)
    {   
        if(rand()%2 == 1)
            bit128_set_1(&ret, i);
        else 
            bit128_set_0(&ret, i);
    }
    //  The first bit is set to 1/0 to make sure it's odd/even.
    if(parity == NT_ODD)
        bit128_set_1(&ret, 0);
    if(parity == NT_EVEN)
        bit128_set_0(&ret, 0);

    //  The highest bit is set to 1 to make sure it's a big integer.
    if(flag == NT_BIG)
        bit128_set_1(&ret, n_bits - 1);

    return ret;
}


static int
JacobiSymbol(bit128 numer, bit128 denom)
{
    // Assume that denom is an odd number and denom > 1
    if(denom % 2 == 0)
        return 1;
    // n = p1p2...pr
    // (m/n) = (m/p1)(m/p2)...(m/pr)
    bit128 m = numer;
    bit128 n = denom;
    int sign = 1;

    while (1)
    {
        // Theorem: (1/n) = 1
        if(m < 2)
            break;
        //   Property 4: 
        //   If n and m both is odd number,
        //             { -(n/m), when m = n = 3 (mod 4);
        //   (m/n) = ->{
        //             { (n/m), otherwise.
        //
        if(m % 2 == 1) {
            if(m % 4 == 3 && n % 4 == 3) {
                sign = -sign;
            }
            bit128_swap(&m, &n);
        }

        //   Property 1:
       //   If n is an odd number, m1 = m2 (mod n),
        //   (m1/n) = (m2/n).
        //
        if(m >= n) {
            m =  m % n;
        }

        //   Property 3:
       //   ( (m1*m2)/n ) = (m1/n)*(m2/n)
        //
       //   Property 2:
       //           {  1, when n= 1 (mod 8) or n = -1 (mod 8)
       //   (2/n)=->{
       //           {  -1, when n= 3 (mod 8) or n = -3 (mod 8)
       //
        while(m >= 2 && m % 2 == 0) {
            if(n%8 == 1 || n%8 == 7)
                sign = sign;
            else if(n%8 == 3 || n%8 == 5)
                sign = -sign;
            m /= 2;
        }
    }
    return sign;
}

/*
*   Assume that p is an odd number,
*   1. p is a prime, returns 0;
*   2. p is odd composite number, returns 0 with a probability of at most 0.5.
*/
int
SolovayStrassenPrimalityTest(bit128 p)
{
    if(p%2 == 0 || p < 2)
        return 1;

   //   If p is not a prime number(i.e., an odd composite number), 
   //   there is at most 0.5 probability that 2. and 3. are both true.
    for(int i = 0; i < TEST_TIMES; i++)
    {   

       //  1. Select m from {1, 2, ..., p-1},
       //  Note: When m=1, the following 2. and 3. are obviously true,
       //  so we select from {2, ... , p-1}. */
        bit128 m = rand()  % (p-2) + 2;

       //   2. Is (m, p) equal to 1 ?
       //   Yes->continue; No-> return false.(i.e., return 1)
       //
        if(bit128_gcd(m, p, NULL) != 1)
            return 1;
        
       //   3. Is (m/p) equal to m^{(p-1)/2} (mod p) ?
       //   Yes->continue; No-> return false.(i.e., return 1)
       //
        int J = JacobiSymbol(m, p);
        bit128 temp = bit128_power_mod(m, ((p-1)/2), p);
        if(! ((J == 1 && temp == 1) || (J == -1 && temp == p-1))) 
            return 1;
    }
    return 0;

}

/*
*   Assume that p is an odd number,
*   1. p is a prime, returns 0; otherwise returns 1;
*   the probability of error is 0.25 at most.
*/
int
MillerRabinPrimalityTest(bit128 n)
{
    // Assume that n is an odd number.
    if(n <= 2 || n%2 == 0)
        return 1;

   //   1. n -1 = 2^km, m is an odd number.
    bit128 m = n - 1;
    bit128 k = 0;
    while(m%2 == 0)
    {
        m /= 2;
        k++;
    }

   //   2. Select a integer 'a' randomly ( 1 <= a <= n-1)
    bit128 a = rand() % (n-1) + 1;

   //    3. b = a^m (mod n)
    bit128 b = bit128_power_mod(a, m, n);

   // If b =1 (mod n), answer n is prime number.(i.e., return 0)
    if(b%n == 1)
        return 0;

   //   for i = 0 ~ k-1, do:
   //       if b = -1 (mod n), return 0;
   //       otherwise,update b <- b^2 (mod n).
   for(int i = 0; i < k; i++)
   {
       if(b % n == n - 1)
            return 0;
        b = bit128_product_mod(b, b, n);// b = (b * b) % n
   }
    return 1;
}

//  Print bit128 in bin/dec/hex format to terminal or file
// Flag: P_HEX, P_DEC, P_BIN
char * 
bit128_print(bit128 n, int flag, FILE *fp)
{
  if (n == 0)
  {
        printf("0 \n");
        return NULL;
  }

  char str[128] = {0}; // log10(1 << 128) + '\0'
  char *s = str + sizeof(str) - 1; // start at the end
  while (n != 0) {
	if (s == str)
        return NULL; // never happens
    if(flag == P_HEX)
    {
        *--s = "0123456789abcdef"[n % 16]; // save last digit
        n /= 16;                     // drop it
    } else if(flag == P_DEC){
        *--s = "0123456789"[n % 10]; // save last digit
        n /= 10;                     // drop it
    }else if(flag == P_BIN){
        *--s = "01"[n % 2];
        n /= 2;
    }
  }
    if(fp)
        fputs(s, fp);
    else {
        if(flag == P_HEX)
            printf("0x");
        printf("%s\n", s);

    }
    return s;
}