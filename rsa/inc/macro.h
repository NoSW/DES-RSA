#ifndef _MACRO_H_
#define _MACRO_H_

typedef __uint128_t bit128;
static int __VAL_INT;
static bit128 __VAL_BIT128;

#define CAT(x, y) x##y

#define isError(is_error,f, v) do{ \
    if(is_error) {    \
        printf(#v); \
        printf(": ");\
        printf(f,v);   \
        exit(1);} \
}while(0)

#define PRINT_INT(x) do{\
            __VAL_INT = x; \
            printf(#x); \
            printf(": %d\n", __VAL_INT); \
}while(0)

#define PRINT_BIT128_HEX(x) do{\
            __VAL_BIT128 = x; \
            printf(#x); \
            printf(": ");\
            bit128_print(__VAL_BIT128, P_HEX, NULL); \
}while(0)         

#define PRINT_BIT128_DEC(x) do{\
            __VAL_BIT128 = x; \
            printf(#x); \
            printf(": ");\
            bit128_print(__VAL_BIT128, P_DEC, NULL); \
}while(0)

#endif