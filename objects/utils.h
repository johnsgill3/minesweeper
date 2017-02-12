#ifndef _UTILS_H_
#define _UTILS_H_
#ifdef DEFINE_VARIABLES
#define EXTERN /* nothing */
#else
#define EXTERN extern
#endif /* DEFINE_VARIABLES */

#include <iostream>
#include <string>
#include <map>
#include <signal.h>

EXTERN std::map<std::string, int> args;

EXTERN void usage(std::string);
EXTERN void my_getargs(int argc, char **argv);
EXTERN void sig_handler(int sig);

#endif
