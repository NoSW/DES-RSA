#include<stdlib.h>
#include<stdio.h>
#include "../inc/des.h"

void
Des(bit64* out, bit64 *in, bit64* key, int total_bytes, int Algo_t ,int mode)
{
    int packet_n = total_bytes/8 + !!(total_bytes % 8);
    bit64 fill_bytes = 0x0;
    if(total_bytes%8)
        for(int i = 0; i < 8-total_bytes%8; i++){
            fill_bytes <<= 8;
            fill_bytes += 36; //'$';
        }
    fill_bytes <<= (total_bytes%8)*8;
    in[packet_n-1] |= fill_bytes;

    if(mode == MODE_ENCODE)
        *in ^= IV;

    packet_n++;
    for(int i = 0; i < packet_n; i++)
    {
        switch (Algo_t)
        {
        case ALGO_DES:
            DesUnit(out+i, *(in+i), *key, mode);
            break;
        case ALGO_DESEEE3:
        case ALGO_DESEDE3:
        case ALGO_DESEEE2:
        case ALGO_DESEDE2:
            Des2or3(out+i, in+i, key,Algo_t, mode);
            break;
        default:
            break;
        }
        // CBC M[i] = M[i] ^ C[i-1]
        if(mode == MODE_ENCODE && (i+1) < packet_n)
            *(in+i+1) ^= *(out+i);
        if(mode == MODE_DECODE && i>0 && i < packet_n)
            *(out+i) ^= *(in+i-1);
    }
    if(mode == MODE_DECODE)
        *out ^= IV;
}

static void
RoundFunction(bit64* out, bit64* R_idec1, bit64* subkey)
{
    //32-bits -> 48-bits (by E[48])
    bit48 *d48 = (bit48 *)malloc(8);
    *d48 = 0;
    Permutation(d48, R_idec1, E, 48);

    // d48 = d48 ^ subkey
    *d48 ^= *subkey;

    // S-box
    bit32 *d32 = (bit32 *)malloc(4);
    *d32 = 0;
    S_box(d32, d48, S0, 0);
    S_box(d32, d48, S1, 1);
    S_box(d32, d48, S2, 2);
    S_box(d32, d48, S3, 3);
    S_box(d32, d48, S4, 4);
    S_box(d32, d48, S5, 5);
    S_box(d32, d48, S6, 6);
    S_box(d32, d48, S7, 7);
    
    //48-bits -> 32-bits (by P[32])
    Permutation(out, d32, P, 32);

}

static void
S_box(bit32* out, bit48* in, const int* box_n, int n)
{
    int h1, h2, t;
    // in[n*6:n*6+5] = x0x1x2x3x4x5
    // h1 = x0x5
    h1 = get_bit_n(*in, n*6) *2 + get_bit_n(*in, n*6+5);
    // h2 = x1x2x3x4
    h2 =    get_bit_n(*in, n*6 + 1) * 8 + 
            get_bit_n(*in, n*6 + 2) * 4 +
            get_bit_n(*in, n*6 + 3) * 2 + 
            get_bit_n(*in, n*6 + 4) * 1;
    // t = S0[h1][h2] , S0[4x16]
    t = S0[h1*16 + h2];

    // t(4bits) -> out[n*4:n+3]
    *out <<= 4;
    *out += t;
}

static void
InitialKey(bit64 key)
{
    // key:64bit -> 56bit (remove 8, 16, 24, 32, 40, 48, 56, 64bit)
    bit64 *kp = (bit64 *)malloc(8);
    *kp = key;
    //*key = 0;
    for(int i = 0; i < 8; i++)
    {
        *kp <<= 7;
        *kp |= (key >> (56-8*i) & 0x7f);
    }

    Permutation(kp, kp, IPC, 56);
    for(int i = 0; i < 16; i++)
        GenerateSubkey(subkeys + i, kp, i);

}

static void
GenerateSubkey(bit48* subkey, bit64* key, int n)
{
    //(C0，D0)＝ IPC(KEY)
    // Loop left shift, the number of loop depends on LS[]
    bit28 C0, D0;
    C0 = (*key >> 28) & 0xfffffff;
    D0 = *key & 0xfffffff;
    // loop left shift
    for(int i = 0; i < LS[n]; i++)
    {
        C0 <<= 1;
        C0 += (C0 >> 28) & 0x1;
        C0 &= 0xfffffff;
        D0 <<= 1;
        D0 += (D0 >> 28) & 0x1;
        D0 &= 0xfffffff;
    }

    // contact C0D0
    *key = 0;
    *key = C0;
    *key <<= 28;
    *key += D0;

    Permutation(subkey, key, PC, 48);

}


static void
Permutation(bit64* out, bit64* in, const int* permuMatrix, int n)
{
    bit64 temp = *in;
    for(int i = 0; i < n; i++)
    {
        if(get_bit_n(temp, permuMatrix[i]))
            bit_set_1(out, i);
        else bit_set_0(out, i);
    }
}

static void
DesUnit(bit64* out, bit64 data_in, bit64 key, int mode)
{
    // InitialKey
    InitialKey(key);
    bit64 *in = &data_in;
    // First, the input 64 bits to do a subscript permutation transformation,
    // through the transformation matrix IP[]
    Permutation(in, in, IP, PACKETLENGTH);
    // (R0, L0) = IP(in),  L=in[0:31] ,R=[32: 63]
    bit32* L_i = (bit32 *)malloc(4);
    bit32* R_i = (bit32 *)malloc(4);
    *L_i = (*in << 32) >> 32;
    *R_i = *in >> 32;
    // Do 16 iterations
    bit32* L_idec1 = (bit32 *)malloc(4);
    bit32* R_idec1 = (bit32 *)malloc(4);
    bit32* RF_out = (bit32 *)malloc(4);
    for(int i = 0; i < ROUNDTIMES; i++) {
        *L_idec1 = *L_i;
        *R_idec1 = *R_i;
        if(mode == MODE_DECODE){//1,MODE_DECODE
            // R_i = L_(i-1)
            *R_i = *L_idec1;
            // R_i = L_(i-1) ^ F(R_(i-1), K_(i-1))
            RoundFunction(RF_out, L_idec1, subkeys+ (16-i-1));
            *L_i = *R_idec1 ^ *RF_out;
        } else {
            // L_i = R_(i-1)
            *L_i = *R_idec1;
            // R_i = L_(i-1) ^ F(R_(i-1), K_(i-1))
            RoundFunction(RF_out, R_idec1, subkeys+i);
            *R_i = *L_idec1 ^ *RF_out;
        }
    }
    // in = (L16 R16)
    *in = *R_i;
    *in <<= 32;
    *in += *L_i;
    // Inverse transformation； IP^(-1) (in)
    Permutation(out, in, InvIP, PACKETLENGTH);
}


static void
Des2or3(bit64* out, bit64* in, bit64* key, int Algo_t, int mode)
{
    bit64 k1, k2, k3;
    k2 = key[1];
    if(Algo_t == ALGO_DESEEE3 || Algo_t == ALGO_DESEDE3)
    {
        k1 = mode == MODE_ENCODE ? key[0] : key[2];
        k3 = mode == MODE_ENCODE ? key[2] : key[0];
    }

    if(Algo_t == ALGO_DESEDE2 || Algo_t == ALGO_DESEEE2)
        k1 = k3 = key[0];
    
    DesUnit(out, *in, k1, mode);
    DesUnit(out, *out, k2, mode);
    DesUnit(out, *out, k3, mode);
}