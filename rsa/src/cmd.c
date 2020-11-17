#include "../inc/cmd.h"


int run(int argc, char* argv[])
{
    if(CommandParsing(argc, argv) != 0)
        exit(1);

    Rsa(&cmd_out, cmd_in, cmd_k, cmd_n);

    FILE* fp = fopen("out.txt", "w");
    bit128_print(cmd_out, P_DEC, fp);
    fclose(fp);

    //TEST
    printf("--------------------------------------------------------\n");
    PRINT_BIT128_DEC(cmd_n)
    PRINT_BIT128_DEC(cmd_d)
    PRINT_BIT128_DEC(cmd_e)
    printf("\n");
    PRINT_BIT128_DEC(cmd_in)
    PRINT_BIT128_DEC(cmd_out)
    bit128 encoded = 0, decoded = 0;
    encoded = cmd_out;
    Rsa(&cmd_out, encoded, cmd_d, cmd_n);
    decoded = cmd_out;

    printf("Done.\n\nHere are the results of the test:\n");
    // PRINT_BIT128_DEC(cmd_k)
    PRINT_BIT128_DEC(encoded)
    PRINT_BIT128_DEC(decoded)
    isError(decoded != cmd_in, "!!!ERROR!!!", NULL);
    return 0;
}

static int
CommandParsing(int argc, char* argv[])
{
    // No input file path, Print HELP
    if(argc == 1){
        printf("%s\n",help_info);
        return 1;
    }
    if(argc != 2 && argc != 4) {
        printf("ERROR: invalid command!\n");
        printf("%d", argc);
        return 1;
    }

    cmd_in = Str2Bit128(argv[1]);
    if(argc == 4)
    {
        cmd_k = Str2Bit128(argv[2]);//
        cmd_n = Str2Bit128(argv[3]);//
    } else {
        srand((unsigned)time(NULL));
        RsaKeyPairGenerator(128, &cmd_e, &cmd_d, &cmd_n);
        FILE* fp = fopen("key.txt","w");
        fputs("<SK, PK, n>: <", fp);
        bit128_print(cmd_d, P_DEC, fp);
        fputs(", ", fp);
        bit128_print(cmd_e, P_DEC, fp);
        fputs(", ", fp);
        bit128_print(cmd_n, P_DEC, fp);
        fputs(">", fp);
        fclose(fp);
        cmd_k = cmd_e;
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
