// Copyright 2018 John Gill
#ifndef OBJECTS_UTILS_H_
#define OBJECTS_UTILS_H_
#ifdef DEFINE_VARIABLES
#define EXTERN /* nothing */
#else
#define EXTERN extern
#endif /* DEFINE_VARIABLES */
#include <signal.h>
#include <iostream>
#include <string>
#include <map>

EXTERN std::map<std::string, int> args;

EXTERN void usage(std::string);
EXTERN void my_getargs(int argc, char **argv);
EXTERN void sig_handler(int sig);

#endif  // OBJECTS_UTILS_H_
