#ifndef __CMD_H_
#define __CMD_H_

#include "rsa.h"
#include<string.h>
#include<assert.h>
#include<time.h>
#include<windows.h>
#define MAX_BUFFER 1024 //8MB
#define MODE_ENCODE 0
#define MODE_DECODE 1

enum{
    RSA_N = 0,
    RSA_P,
    RSA_Q,
    RSA_PhiN,
    RSA_E,
    RSA_D,
    RSA_K,
    RSA_INIT,
    RSA_SPEC,
};
enum{
    PATH_IN = 0,
    PATH_OUT,
    PATH_OUT_KEY,
    PATH_IN_KEY,
};
static bit128 cmd_rsa[9] = {0};
static char* cmd_path[9] = {NULL};
static bit128 cmd_in[MAX_BUFFER] = {0};
static bit128 cmd_out[MAX_BUFFER] = {0};
static bit64 bit64_buffer[MAX_BUFFER] = {0};
static int total_bytes;
static int mode;
static int packet; 
static char curtime[32];

int run(int argc, char* argv[]);
static int CommandParsing(int argc, char* argv[]);
static bit128 Str2Bit128(char *);
static int ReadData();
static int WriteData();
static void getCurrentTime();
static void OutLog();
static char* getFileName(char*);

// Run the  'RunFunction' and calculate the running time
static double run_time;
#define Counter(RunFunction)  LARGE_INTEGER  time_start;\
	                LARGE_INTEGER time_over;\
	                double dqFreq;\
	                LARGE_INTEGER f;\
	                QueryPerformanceFrequency(&f);\
	                dqFreq=(double)f.QuadPart;\
	                QueryPerformanceCounter(&time_start);\
                    RunFunction;\
	                QueryPerformanceCounter(&time_over);\
	                run_time=1000000*(time_over.QuadPart-time_start.QuadPart)/dqFreq; \

static char help_info[] = "Usage:\n" \
            "\trsa.exe [option1] [option2] ... \n"\
            "\nOptions:\n"\
            "\t-i <path>\t\tinput a file.\n" \
            "\t-o <path>\t\tthe out file(the default is './out/out-filename'.\n" \
            "\t-k <key>\t\tthe key for encode(Public Key) or decode(Secret Key).\n"
            "\t-m <modulu>\t\tthe modulu.\n"
            "\t--init <file>\tInitial a rsa key pair <SK, PK, mod>(saved in <file>) and perfrom encode.\n\n" \
            "Note:\n\tFor convenience, we dump all execution information into log.txt, regardless of the security.";

#endif