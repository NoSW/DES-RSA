#include"../inc/cmd.h"

int
run(int argc, char* argv[])
{
    // No input file path, Print HELP
    if(argc == 1){
        printf("%s",help_info);
        return 0;
    }
    // Initial static global variables
    algo = 0, mode = 0, k_flag = 0, total_bytes = 0;
    in_path = argv[1];
    fp = NULL; out_path =  NULL;
    keys[0] = keys[1] = keys[2] = NULL;

    // read from in_path
    fp = fopen(in_path, "rb");
    isError(fp == NULL, "Open in_file failed:%s",in_path);

    // Read  byte-by-byte
    total_bytes = fread(in_buff, 1, MAX_BUFF_SIZE, fp);
    fclose(fp);
    isError(total_bytes >= MAX_BUFF_SIZE, "Read file failed(Maximum support is 10MB)",NULL)
    // command parsing, get algo, mode, keys...
    CommandParsing(argc,argv);

    // If no out_path, choose deafalt out_file, 
    // encoded/in_file_name or decoded/in_file_name
    if(out_path == NULL) {
        out_path = (char*)malloc(512);
        if(mode == MODE_DECODE)
            sprintf(out_path,"./decoded/%s",getFileName(in_path));
        if(mode == MODE_ENCODE)
            sprintf(out_path,"./encoded/%s",getFileName(in_path));
    }
    // Call program and calculate running time
    DesCounter;
    // Write the results
    fp = fopen(out_path,"wb");
    isError(fp == NULL, "Open out_file failed:%s",out_path);
    fwrite(out_buff,1,total_bytes-(total_bytes%8)+8*(!!(total_bytes%8)),fp);
    fclose(fp);
    // If you randomly generate a set of simple alphanumeric keys,
    // output the keys to the file 'randomkeys.txt'
    OutLog();
    // B/ms
    return total_bytes/(int)(run_time/1000);
}

static void
OutLog()
{
    // get current time
    getCurrentTime();
    // Print log
    fp = fopen("des-log.txt","a");
    fprintf(fp,"time:\t%s\n",curtime);
    if(mode == MODE_ENCODE)
        fprintf(fp,"Type:\tENCODE\nAlgo:\t%d\n",algo);
    if(mode == MODE_DECODE)
        fprintf(fp,"Type:\tDECODE\nAlgo:\t%d\n",algo);
    fprintf(fp,"Input:\t%s\n",in_path);
    fprintf(fp,"Output:\t%s\n\n",out_path);
    for(int i = 0; i < AKM[algo]; i++) {
        fprintf(fp,"key %d:%s\n", i+1, keys[i]);
    }
    fprintf(fp,"\nFile size:\t%dKB\n",total_bytes >> 10);
    fprintf(fp,"Run time:\t%fus\n",run_time);
    fprintf(fp,"Operating speed:\t%dB/ms\n",total_bytes/(int)(run_time/1000));
    fputs("-----------------------------------------\n",fp);
    fclose(fp);
}

static void
CommandParsing(int argc, char* argv[])
{
    int k_flag = 0;
    for(int i = 2; i < argc; i++)
    {
        if(argv[i] != NULL)
            switch (argv[i][0])
            {
            case '0':
                break;
            case '1':
                algo = ALGO_DESEEE3;
                break;
            case '2':
                algo = ALGO_DESEDE3;
                break;
            case '3':
                algo = ALGO_DESEEE2;
                break;
            case '4':
                algo = ALGO_DESEDE2;
                break;
            case 'e':
                mode = MODE_ENCODE;
                break;
            case 'd':
                mode = MODE_DECODE;
                break;
            case '=':
                out_path = argv[++i];
                break;
            case '+':
                keys[k_flag++] = argv[++i];
                break;
            default:
                break;
            }
    }

    if(mode == MODE_ENCODE && k_flag == 0) {
            _64Bits_Random_Alphanumeric_Key_Generator();
            k_flag = AKM[algo];
    }
    isError(k_flag < AKM[algo],"No enough Keys!", NULL);

}

static void
_64Bits_Random_Alphanumeric_Key_Generator()
{
    srand(time(NULL));
    while(k_flag < AKM[algo]){
 
        if(keys[k_flag] == NULL){
            keys[k_flag] = (char *)malloc(9);
            for(int i = 0; i < 8; i++)
            {
                int t = rand() % 3;
                if(t == 0) 
                    keys[k_flag][i] = rand()%10 + '0';
                if(t == 1)
                    keys[k_flag][i] = rand()%26 + 'a';
                if(t == 2)
                    keys[k_flag][i] = rand()%26 + 'A';
            }
            keys[k_flag][8]='\0';
        }
        k_flag++;
        
    }
}

static void
getCurrentTime()
{
    time_t tmpcal_ptr;
	struct tm *tmp_ptr = NULL;
	
	time(&tmpcal_ptr);
	tmp_ptr = gmtime(&tmpcal_ptr);
	tmp_ptr = localtime(&tmpcal_ptr);
	sprintf (curtime, "%d.%d.%d-%d:%d:%d",
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