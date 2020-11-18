#include "../inc/cmd.h"

int
run(int argc, char* argv[])
{   
    if(CommandParsing(argc, argv) != 0)
        exit(1);
    // Read data from the file 'cmd_path[PATH_IN]'
    FILE* fp = fopen(cmd_path[PATH_IN], "rb");
    isError(fp==NULL, "Open file failed: %s\n", cmd_path[PATH_IN]);
    total_bytes = fread(cmd_in, 1, MAX_BUFFER*16, fp);
    int packet = total_bytes/16 + !!(total_bytes%16);

    // Scanning for over n
    for(int i = 0; i < packet; i++)
        isError(cmd_in[i] >= cmd_rsa[RSA_N], "The file contents cannot be encoded: '%s' \n", cmd_path[PATH_IN]);


    Counter( for(int i = 0; i < packet; i++){
                Rsa(&cmd_out[i], cmd_in[i], cmd_rsa[RSA_K], cmd_rsa[RSA_N]);
                });
    fp = fopen(cmd_path[PATH_OUT], "wb");
    fwrite(cmd_out, 16, packet, fp);
    fclose(fp);

    OutLog();
    return total_bytes/(int)(run_time/1000);
}

static int
CommandParsing(int argc, char* argv[])
{
    // No input file path, Print HELP
    if(argc == 1){
        printf("%s\n",help_info);
        return 1;
    }

    for(int i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            if(argv[i][1] == '-')
            {   if(strcmp(argv[i], "--init") == 0)
                {
                    srand((unsigned)time(NULL));
                    RsaKeyPairGenerator(128, cmd_rsa);
                    if(argv[i+1] && argv[i+1][0] != '-')
                        cmd_path[PATH_OUT_KEY] = argv[++i];
                    cmd_rsa[RSA_INIT] = 1;
                    cmd_rsa[RSA_K] = cmd_rsa[RSA_E];
                } else isError(1,"ERROR: Invalid Command: %s\n", argv[i]);
            } else {
                switch (argv[i][1])
                {
                case 'h':
                    printf("%s\n",help_info);
                    return 1;
                case 'i':
                    cmd_path[PATH_IN] = argv[++i];
                    break;
                case 'o':
                    cmd_path[PATH_OUT] = argv[++i];
                    break;
                case 'k':
                    cmd_rsa[RSA_K] = Str2Bit128(argv[++i]);
                    cmd_rsa[RSA_SPEC] = 1;
                    break;
                case 'm':
                    cmd_rsa[RSA_N] = Str2Bit128(argv[++i]);
                    cmd_rsa[RSA_SPEC] = 1;
                    break;
                default:
                    isError(1,"ERROR: Invalid Command: %s\n", argv[i]);
                    break;
                }
            }
        }
    }
    if(cmd_path[PATH_IN] == NULL)
        isError(1, "ERROR: Initial a key pair? Use your key?\n", NULL);

    if(cmd_rsa[RSA_INIT] == cmd_rsa[RSA_SPEC])
        isError(1, "ERROR: Initial a key pair? Use your key?\n", NULL);

    if(cmd_path[PATH_OUT] == NULL)
    {
        cmd_path[PATH_OUT] = (char*)malloc(512);
        sprintf(cmd_path[PATH_OUT],"./out/out-%s",getFileName(cmd_path[PATH_IN]));
    }

    if(cmd_path[PATH_OUT_KEY] == NULL)
    {
        cmd_path[PATH_OUT_KEY] = (char*)malloc(512);
        sprintf(cmd_path[PATH_OUT_KEY],"./key/rsa-key-%s.txt",getFileName(cmd_path[PATH_IN]));
    }

    return 0;
}

static bit128
Str2Bit128(char * str)
{
    if(str == NULL)
        return 0;
    bit128 ret = 0;
    for(int i = 0; str[i] != '\0'; i++)
    {
        ret *= 10;
        ret += str[i] - '0';
    }
    return ret;
}

static void
OutLog()
{
    // get current time

    getCurrentTime();
    // Print log
    FILE* fp = fopen("rsa_log.txt","a");
    fprintf(fp,"time:\t%s\n",curtime);
    fprintf(fp,"Input:\t%s\n",cmd_path[PATH_IN]);
    fprintf(fp,"Output:\t%s\n\n",cmd_path[PATH_OUT]);
    
    if(cmd_rsa[RSA_INIT]){
        fprintf(fp,"<SK, PK, n> saved in %s:\n", cmd_path[PATH_OUT_KEY]);
        fprintf(fp,"p:\t");
        bit128_print(cmd_rsa[RSA_P], P_DEC, fp);
        fprintf(fp,"\nq:\t");
        bit128_print(cmd_rsa[RSA_Q], P_DEC, fp);
        fprintf(fp,"\nPhi:");
        bit128_print(cmd_rsa[RSA_PhiN], P_DEC, fp);
        fprintf(fp,"\ne:\t");
        bit128_print(cmd_rsa[RSA_E], P_DEC, fp);
        fprintf(fp,"\nd:\t");
        bit128_print(cmd_rsa[RSA_D], P_DEC, fp);

        FILE* fp2 = fopen(cmd_path[PATH_OUT_KEY],"w");
        fputs("<SK, PK, mod>: <", fp2);
        bit128_print(cmd_rsa[RSA_D], P_DEC, fp2);
        fputs(", ", fp2);
        bit128_print(cmd_rsa[RSA_E], P_DEC, fp2);
        fputs(", ", fp2);
        bit128_print(cmd_rsa[RSA_N], P_DEC, fp2);
        fputs(">", fp2);
        fclose(fp2);
        
    } else {
        fprintf(fp,"\nk:\t");
        bit128_print(cmd_rsa[RSA_K], P_DEC, fp);
    }
    fprintf(fp,"\nn:\t");
    bit128_print(cmd_rsa[RSA_N], P_DEC, fp);

    fprintf(fp,"\n\nFile size:\t%dKB\n",total_bytes >> 10);
    fprintf(fp,"Run time:\t%fus\n",run_time);
    fprintf(fp,"Operating speed:\t%dB/ms\n",total_bytes/(int)(run_time/1000));
    fputs("-----------------------------------------\n",fp);
    fclose(fp);
}

static void
getCurrentTime()
{
    time_t tmpcal_ptr;
	struct tm *tmp_ptr = NULL;
	
	time(&tmpcal_ptr);
	tmp_ptr = gmtime(&tmpcal_ptr);
	tmp_ptr = localtime(&tmpcal_ptr);
	sprintf (curtime, "%d/%d/%d %d:%d:%d",
        (1900+tmp_ptr->tm_year), (1+tmp_ptr->tm_mon),
         tmp_ptr->tm_mday, tmp_ptr->tm_hour, 
         tmp_ptr->tm_min, tmp_ptr->tm_sec);
}

static char*
getFileName(char *p)
{
    int i = 0;
    while (p[i] != '\0') i++;
    while (i >= 0 && (p[i] != '\\' && p[i] != '/')) i--;
    return p + i + 1;
}

