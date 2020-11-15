#include "prime.h"

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
        p = BigOddIntegerGenerator(n_bits);

        total++;
        int ss_test = SolovayStrassenPrimalityTest(p);
        int mr_test = MillerRabinPrimalityTest(p);
        int pass_test = test(p);
        if(pass_test != ss_test || pass_test != mr_test)
        {
            if(pass_test == 0)
                printf("\033[1;31mERROR:\t\033[0m\t");
            else printf("\033[1;33mMISSING:\33[0m");
            PRINT_BIT128(p);
            if(pass_test != ss_test)
                printf("SolovayStrassen\n");
            if(pass_test != mr_test)
                printf("MillerRabin\n");
        }
        if( ss_test == 0 && mr_test == 0){
            printf("\033[1;32mGOOD:\033[0m");
            PRINT_BIT128(p);
            PRINT_INT(total);
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
bit128_power_mod(bit128 a, bit128 n, bit128 p)
{
    if(p == 0) //ERROR
        return 0;
    if(n == 1)
        return a % p;
    if(n == 0)
        return 1;
    bit128 ret;
    ret = bit128_power_mod(a, n/2, p);
    ret = (ret * ret) % p;
    if(n%2 == 1)
        ret = (ret * a) % p;
    return ret;
}

bit128
bit128_gcd(bit128 a, bit128 b)
{
    bit128 r = 0, q = 0;
    bit128 t = a;
    bit128 s = b;

    // r = q*t + s
    do
    {
        r = MAX(t, s);
        t = MIN(t, s);
        q = r / t;
        s = r - q * t;
    } while(s != 0);

    return t;
}

bit128 
BigOddIntegerGenerator(int n_bits)
{
    
    bit128 ret = 0x1;
    for(int i = 1; i < n_bits - 1; i++)
    {   
        if(rand()%2 == 1)
            bit128_set_1(&ret, i);
    }
    bit128_set_1(&ret, n_bits - 1);
    return ret;
}

static int
JacobiSymbol(bit128 numer, bit128 denom)
{
    // Assume that denom is an odd number and denom > 1
    if(denom%2 == 0)
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
        
        //  Property 4: 
        //  If n and m both is odd number,
        //            { -(n/m), when m = n = 3 (mod 4);
        //  (m/n) = ->{
        //            { (n/m), otherwise.
        if(m % 2 == 1) {
            if(m % 4 == 3 && n % 4 == 3) {
                sign = -sign;
            }
            bit128_swap(&m, &n);
        }

        //  Property 1:
        // If n is an odd number, m1 = m2 (mod n),
        // (m1/n) = (m2/n).
        if(m >= n) {
            m =  m % n;
        }

        // Property 3:
        // ( (m1*m2)/n ) = (m1/n)*(m2/n)
        //
        // Property 2:
        //           {  1, when n= 1 (mod 8) or n = -1 (mod 8)
        // (2/n) = ->{
        //           {  -1, when n= 3 (mod 8) or n = -3 (mod 8)
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

int
SolovayStrassenPrimalityTest(bit128 p)
{
    if(p%2 == 0 || p < 2)
        return 1;
    /*
    *  If p is not a prime number, there is at most 0.5 probability
    *  that 2. and 3. are true.*/
    for(int i = 0; i < TEST_TIMES; i++)
    {   
        /*
        *  1. Select m from {1, 2, ..., p-1},
        *  Note: When m=1, the following 2. and 3. are obviously true,
        *  so we select from {2, ... , p-1}. */
        bit128 m = rand()  % (p-2) + 2;
        /*
        *  2. Is (m, p) equal to 1 ?
        *  Yes->continue; No-> return false, i.e., return 1.*/
        if(bit128_gcd(m, p) != 1)
            return 1;
        /*
        *  3. Is (m/p) equal to m^{(p-1)/2} (mod p) ?
        *  Yes->continue; No-> return false, i.e., return 1.*/
        int J = JacobiSymbol(m, p);
        bit128 temp = bit128_power_mod(m, ((p-1)/2), p);
        if(! ((J == 1 && temp == 1) || (J == -1 && temp == p-1))) 
            return 1;
        

    }
    return 0;

}

int
MillerRabinPrimalityTest(bit128 n)
{
    // Assume that n is an odd number.
    if(n <= 2 || n%2 == 0)
        return 1;
    /*
    *   1. n -1 = 2^km, m is an odd number.*/
    bit128 m = n - 1;
    bit128 k = 0;
    while(m%2 == 0)
    {
        m /= 2;
        k++;
    }
    /*
    *   2. Select a integer 'a' randomly ( 1 <= a <= n-1)*/
    bit128 a = rand() % (n-1) + 1;

    /*
    *    3. b = a^m (mod n)*/
    bit128 b = bit128_power_mod(a, m, n);

    /*
    * If b =1 (mod n), answer n is prime number , i.e., return 0.*/
    if(b%n == 1)
        return 0;

    /*
    *   for i = 0 ~ k-1, do:
    * if b = -1 (mod n), return 0;
    * b = b^2 (mod n), otherwise.*/
   for(int i = 0; i < k; i++)
   {
       if(b % n == n - 1)
            return 0;
        b = (b*b) % n;
   }
    return 1;
}

int 
bit128_print(bit128 n)
{
  if (n == 0)
    return printf("0\n");

  char str[40] = {0}; // log10(1 << 128) + '\0'
  char *s = str + sizeof(str) - 1; // start at the end
  while (n != 0) {
	if (s == str)
        return -1; // never happens

    *--s = "0123456789abcdef"[n % 16]; // save last digit
    n /= 16;                     // drop it
  }
  return printf("0x%s\n", s);
}
