#include "../inc/cmd.h"

int
run(int argc, char* argv[])
{   
    if(CommandParsing(argc, argv) != 0)
        exit(1);

    // 1.Encode/Decode one packet data and dump result into 'cmd_path[PATH_OUT]'
    if(cmd_mode[MODE_OnePacket] == 1)
        return OnePacket();

    // 2. Encode/Decode data from a file, 
    // and dump result into 'cmd_path[PATH_OUT]',
    // and save the running record to 'rsa_log.txt'
    ReadData();
    Check();
    Counter( for(int i = 0; i < packet; i++){
                Rsa(&cmd_out[i], cmd_in[i], cmd_rsa[RSA_K], cmd_rsa[RSA_N]);});
    WriteData();
    OutLog();
    return total_bytes/(int)(run_time/1000);
}

static int
OnePacket()
{
    bit128 temp_out_packet = 0;
    FILE* fp = fopen(cmd_path[PATH_OUT], "w");
    isError(fp==NULL, "ERROR: Can't create file: %s\n", cmd_path[PATH_OUT]);
    Rsa(&temp_out_packet, cmd_rsa[RSA_OnePacket], cmd_rsa[RSA_K], cmd_rsa[RSA_N]);
    // TEST BEGIN
    if(cmd_rsa[RSA_INIT] == 1)
    {
        while(cmd_rsa[RSA_OnePacket] >= cmd_rsa[RSA_N])
            RsaKeyPairGenerator(128, cmd_rsa);
        Rsa(&temp_out_packet, cmd_rsa[RSA_OnePacket], cmd_rsa[RSA_K], cmd_rsa[RSA_N]);
        bit128 test_packet = 0;
        do{
            Rsa(&test_packet, temp_out_packet, cmd_rsa[RSA_D], cmd_rsa[RSA_N]);
            if(test_packet == cmd_rsa[RSA_OnePacket])
                break;
            RsaKeyPairGenerator(128, cmd_rsa);
            Rsa(&temp_out_packet, cmd_rsa[RSA_OnePacket], cmd_rsa[RSA_K], cmd_rsa[RSA_N]);
        }while(1);
    }
    // TEST END
    if(cmd_mode[MODE_STATUS] == MODE_ENCODE)
        fprintf(fp,"Type:\t\t\tENCODE\n");
    else fprintf(fp,"Type:\t\t\tDECODE\n");
    fputs("Original Data:\t", fp);
    bit128_print(cmd_rsa[RSA_OnePacket], P_DEC, fp);
    if(cmd_rsa[RSA_INIT] == 1)
    {   
        fputs("\nSecret Key:\t\t", fp);
        bit128_print(cmd_rsa[RSA_D], P_DEC, fp);
        fputs("\nPublic Key:\t\t", fp);
        bit128_print(cmd_rsa[RSA_E], P_DEC, fp);
        fputs("\nModule:", fp);
        bit128_print(cmd_rsa[RSA_N], P_DEC, fp);
    }
    fputs("\nResult:", fp);
    bit128_print(temp_out_packet, P_DEC, fp);
    return 0;
}

static int
Check()
{
 // Scanning for over n
    for(int i = 0; i < packet; i++){
        while(cmd_in[i] >= cmd_rsa[RSA_N])          
        {
            if(cmd_rsa[RSA_INIT] == 1)
            {
                bit128 old_n = cmd_rsa[RSA_N];
                while(cmd_rsa[RSA_N] <= old_n)
                    RsaKeyPairGenerator(128, cmd_rsa);
            } else isError(1, "the module is too small!", NULL);
        }
        //PRINT_BIT128_HEX(cmd_in[i]);
    }
    
}

static int
ReadData()
{
     // Read data from the file 'cmd_path[PATH_IN]'
    FILE* fp = fopen(cmd_path[PATH_IN], "rb");
    isError(fp==NULL, "Open file failed: %s\n", cmd_path[PATH_IN]);
    total_bytes = fread(cmd_in, cmd_mode[MODE_READ_BIT], MAX_BUFFER, fp);

    int is_padding = (total_bytes%(16/cmd_mode[MODE_READ_BIT]));
    packet = total_bytes/(16/cmd_mode[MODE_READ_BIT]) + !!is_padding;
    PRINT_INT(total_bytes);
    PRINT_BIT128_DEC(cmd_mode[MODE_READ_BIT]);
    PRINT_BIT128_DEC(cmd_mode[MODE_STATUS]);
    // 0x24, 0010 0100 , '$'
    if(is_padding)
        for(int i = cmd_mode[MODE_READ_BIT]; i < 16; i++)
        {
            bit128_set_1(&cmd_in[packet -1], (i*8+2));
            bit128_set_1(&cmd_in[packet -1], (i*8+5));
        }
}
static int
WriteData()
{
    FILE* fp = fopen(cmd_path[PATH_OUT], "wb");
    fwrite(cmd_out, 16, packet, fp);
    fclose(fp);
}

static int
CommandParsing(int argc, char* argv[])
{
    // No input file path, Print HELP
    if(argc == 1){
        printf("%s\n",help_info);
        return 1;
    }
    cmd_mode[MODE_STATUS] = MODE_ENCODE;//default
    cmd_mode[MODE_READ_BIT] = 8; //default
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
                } else if(strcmp(argv[i], "--max") == 0)
                {
                    bit128 old_mod = 5;
                    do{
                        int i;
                        for(i = 0; i < 100000; i++)
                        {
                            srand(time(0));
                            RsaKeyPairGenerator(128, cmd_rsa);
                            if(cmd_rsa[RSA_N] > old_mod)
                                break;
                        }
                        if(i == 100000)
                        {
                            printf("Total searched RSA key pairs more than 100000, search failed\n");
                            exit(1);
                        }
                        printf("Total searched RSA key pairs is %d: ", i + 1);
                        bit128_print(cmd_rsa[RSA_N],P_HEX, NULL);
                        old_mod = cmd_rsa[RSA_N];
                    }while(1);

                } else isError(1,"ERROR: Invalid Command: %s\n", argv[i]);
            } else {
                switch (argv[i][1])
                {
                case 's':
                    cmd_mode[MODE_STATUS] = MODE_DECODE;
                case 'p':
                    cmd_rsa[RSA_K] = Str2Bit128(argv[++i]);
                    cmd_rsa[RSA_SPEC] = 1;
                    break;
                case 'h':
                    printf("%s\n",help_info);
                    return 1;
                case 'i':
                    cmd_path[PATH_IN] = argv[++i];
                    break;
                case 'd':
                    cmd_rsa[RSA_OnePacket] = Str2Bit128(argv[++i]);
                    cmd_mode[MODE_OnePacket] = 1;
                    cmd_path[PATH_OUT] = (char*)malloc(512);
                    sprintf(cmd_path[PATH_OUT],"./out/out-%s.txt",argv[i]);
                    break;
                case 'o':
                    cmd_path[PATH_OUT] = argv[++i];
                    break;
                case 'm':
                    cmd_rsa[RSA_N] = Str2Bit128(argv[++i]);
                    cmd_rsa[RSA_SPEC] = 1;
                    break;
                case 'a':
                    cmd_mode[MODE_READ_BIT] = Str2Bit128(argv[++i]);
                    break;
                default:
                    isError(1,"ERROR: Invalid Command: %s\n", argv[i]);
                    break;
                }
            }
        }
    }
    // Check cmd
    if(cmd_rsa[RSA_INIT] == cmd_rsa[RSA_SPEC])
        isError(1, "ERROR: Initial a key pair? Use your key?\n", NULL);

    if(cmd_mode[MODE_OnePacket] == 1)
        return 0;
    // Input file
    if(cmd_path[PATH_IN] == NULL)
        isError(1, "ERROR: Initial a key pair? Use your key?\n", NULL);
    //Output file
    if(cmd_path[PATH_OUT] == NULL)
    {
        cmd_path[PATH_OUT] = (char*)malloc(512);
        sprintf(cmd_path[PATH_OUT],"./out/out-%s",getFileName(cmd_path[PATH_IN]));
    }
    //Output Keys
    if(cmd_path[PATH_OUT_KEY] == NULL)
    {
        cmd_path[PATH_OUT_KEY] = (char*)malloc(512);
        sprintf(cmd_path[PATH_OUT_KEY],"./key/rsa-key-%s.txt",getFileName(cmd_path[PATH_IN]));
    }

    if(cmd_mode[MODE_STATUS] == MODE_DECODE)
        cmd_mode[MODE_READ_BIT] = 16;

    return 0;
}

static bit128
Str2Bit128(char * str)
{
    if(str == NULL)
        return 0;
    while(str[0] == '0') str = str + 1;
    bit128 ret = 0;
    for(int i = 0; i < 39 && str[i] != '\0'; i++)
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
    fprintf(fp,"Time:\t%s\n",curtime);
    if(cmd_mode[MODE_STATUS] == MODE_ENCODE)
        fprintf(fp,"Type:\tENCODE\n");
    else fprintf(fp,"Type:\tDECODE\n");
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

    fprintf(fp,"\n\nFile size:\t%dKB\n",total_bytes * cmd_mode[MODE_READ_BIT]);
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

