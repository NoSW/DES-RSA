#ifndef _MACRO_H_
#define _MACRO_H_

typedef __uint128_t bit128;
static int __VAL_INT;
static bit128 __VAL_BIT128;

#define CAT(x, y) x##y

#define isError(is_error,f, v)  \
    if(is_error) {    \
        printf(#v); \
        printf(": ");\
        printf(f,v);   \
        exit(1);}

#define PRINT_INT(x) \
            __VAL_INT = x; \
            printf(#x); \
            printf(": %d\n", __VAL_INT);

#define PRINT_BIT128_HEX(x) \
            __VAL_BIT128 = x; \
            printf(#x); \
            printf(": ");\
            bit128_print(__VAL_BIT128, P_HEX, NULL);

#define PRINT_BIT128_DEC(x) \
            __VAL_BIT128 = x; \
            printf(#x); \
            printf(": ");\
            bit128_print(__VAL_BIT128, P_DEC, NULL);


#endif