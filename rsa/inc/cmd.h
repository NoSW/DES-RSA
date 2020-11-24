#ifndef __CMD_H_
#define __CMD_H_

#include "rsa.h"
#include<string.h>
#include<assert.h>
#include<time.h>
#include<windows.h>
#define MAX_BUFFER 4096 //4MB
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
    RSA_SPEC, //Non-INIT
    RSA_OnePacket // a packet data
};
enum{
    PATH_IN = 0,
    PATH_OUT,
    PATH_OUT_KEY,
    PATH_IN_KEY,
};
enum{
    MODE_STATUS = 0,
    MODE_READ_BIT,
    MODE_WRITE_BIT,
    MODE_OnePacket,
};
static bit128 cmd_rsa[10] = {0};
static char* cmd_path[9] = {NULL};
static bit128 cmd_mode[4];
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
static int OnePacket();
static int Check();
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
                    do{RunFunction;}while(0);\
	                QueryPerformanceCounter(&time_over);\
	                run_time=1000000*(time_over.QuadPart-time_start.QuadPart)/dqFreq; \

static char help_info[] = "Usage:\n" \
            "\trsa.exe [option1] [option2] ... \n"\
            "\nOptions:\n"\
            "\t-i <path>\t\tinput a file.(no more than 4 MB)\n" \
            "\t-d <data(dec)>\t\tinput Decimal data.(no more than 10^37)\n" \
            "\t-o <path>\t\tthe out file(the default is './out/out-filename'.\n" \
            "\t-s <key>\t\tthe Secret Key.\n"
            "\t-p <key>\t\tthe Public Key.\n"
            "\t-m <modulu>\t\tthe modulu.\n"
            "\t-a <num>\t\tRead the file in <num>-byte alignment. (Filled with the character '$', when less than 16 bytes(128 bit))\n"
            "\t--init <file>\tInitial a rsa key pair <SK, PK, mod>(saved in <file>) and perfrom encode.\n\n" \
            "Note:\n\tFor convenience, we dump all execution information into rsa_log.txt, regardless of the security.";

#endif