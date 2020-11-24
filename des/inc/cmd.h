#ifndef _UI_H_
#define _UI_H_

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<assert.h>
#include<time.h>
#include<windows.h>
#include "des.h"

#define MAX_BUFF_SIZE 1024*1024*10

#define isError(is_error,f, v)  \
    if(is_error) {    \
        printf(f,v);   \
        exit(1);}

// Run the  'RunFunction' and calculate the running time
#define Counter(RunFunction)  LARGE_INTEGER  time_start;\
	                LARGE_INTEGER time_over;\
	                double dqFreq;\
	                LARGE_INTEGER f;\
	                QueryPerformanceFrequency(&f);\
	                dqFreq=(double)f.QuadPart;\
	                QueryPerformanceCounter(&time_start);\
                    do{RunFunction;}while(0);\
	                QueryPerformanceCounter(&time_over);\
	                run_time=1000000*(time_over.QuadPart-time_start.QuadPart)/dqFreq; \


static int algo, mode, k_flag;
static char* out_path;
static bit64 out_buff[MAX_BUFF_SIZE];
static bit64 in_buff[MAX_BUFF_SIZE];
static char* in_path;
static bit64 bit_keys[3];
static char curtime[32];
static FILE *fp;
static int total_bytes;
static double run_time;

int run(int argc, char* argv[]);
static void KeyGenerator();
static void CommandParsing(int argc, char* argv[]);
static void getCurrentTime();
static bit64 Str2Bit64(char *);
static void OutLog();
static char* getFileName(char*);

static char help_info[] = "******************************************************************\n" \
            "This is the 'HELP' of this DES encryption applet             \n" \
            "******************************************************************\n" \
            "Optional algorithm: \n"  \
            "DES        0(deafault) \n"  \
            "DESEEE3    1 \n" \
            "DESEDE3    2 \n" \
            "DESEEE2    3  \n" \
            "DESEDE2    4 \n" \
            "******************************************************************\n" \
            "Optional mode:\n"  \
            "Encryption     e(deafault)\n"  \
            "Decryption     d\n"  \
            "******************************************************************\n" \
            "Input format:\n des.exe in_file [option1] [option2] [option3] [option4] ...\n"\
            "" \
            "******************************************************************\n" \
            "PS:\n\n"\
            "1. All [option] are optional, and the order can be arbitrary (last-input-first-used, except for your keys)\n\n"\
            "2. Specify the output file path (preferably an absolute path), using the indicator '=',i.e.,  \n"\
            "= [out_file] \n"\
            "(the deafault is the \"./decoded/filename\" or \"./encoded/filename\")\n\n"\
            "3. If you need input key(s), use the indicator '+',i.e., \n"\
            " --dec [k1] --dec [k2] ...\n"\
            "(The default is the 64-bit random key(s)\n"
            "******************************************************************";
#endif