#define DEFINE_VARIABLES
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include "utils.h"
using namespace std;

void usage(string error)
{
    exit(1);
}

void my_getargs(int argc, char **argv)
{
    char c;

    args["nRow"] = args["nCol"] = args["nBomb"] = args["nGame"] = -1;
    args["debug"] = args["interactive"] = 0;
    opterr = 0;
    while ((c = getopt (argc, argv, "r:c:b:dg:i")) != -1)
    {
        switch (c)
        {
        case 'r':
            args["nRow"] = atoi(optarg);
            break;
        case 'c':
            args["nCol"] = atoi(optarg);
            break;
        case 'b':
            args["nBomb"] = atoi(optarg);
            break;
        case 'd':
            args["debug"] = 1;
            break;
        case 'g':
            args["nGames"] = atoi(optarg);
            break;
        case 'i':
            args["interactive"] = 1;
            break;
        default:
            usage("Unknown Arguement");
        }
    }
}
