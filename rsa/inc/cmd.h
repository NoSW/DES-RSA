#ifndef __CMD_H_
#define __CMD_H_

#include "rsa.h"

static bit128 cmd_k, cmd_n, cmd_e, cmd_d;
static bit128 cmd_in, cmd_out;

int run(int argc, char* argv[]);
static int CommandParsing(int argc, char* argv[]);
static bit128 Str2Bit128(char *);

static char help_info[] = "******************************************************************\n" \
            "This is the 'HELP' of this RSA encryption applet             \n" \
            "******************************************************************\n" \
            "Input format:\n rsa.exe data [k] [n] \n"\
            "******************************************************************\n" \
            "1. 'data' is 64bit (max)\n\n"\
            "2. [k] is public key (fro encode) or secret key (for decode)\n\n"\
            "3. [n] is (mod n)\n"\
            "4. If [k] or [N] is the default (i.e., is empty), \n" \
            "the program will automatically generate an RSA key pair <e, d, n> and performs the encode.\n" \
            "******************************************************************";

#endif