// Copyright 2018 John Gill
#define DEFINE_VARIABLES
#include <execinfo.h>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include "../objects/utils.h"


void usage(std::string error) {
    std::cerr << error << std::endl;
    exit(1);
}

void my_getargs(int argc, char **argv) {
    char c;

    args["nRow"] = args["nCol"] = args["nBomb"] = args["nGame"] = -1;
    args["debug"] = args["interactive"] = 0;
    // opterr = 0;
    while ((c = getopt(argc, argv, "r:c:b:dg:i")) != -1) {
        switch (c) {
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

void sig_handler(int sig) {
    void *array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}
