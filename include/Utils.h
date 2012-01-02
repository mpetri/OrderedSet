#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#define GETOPT_FINISHED -1

/* Macro values taken from http://predef.sourceforge.net/ */
#if defined(WIN32) || defined(_WIN32) \
    || defined(__WIN32__) || defined(__WINDOWS__) \
    || defined(__TOS_WIN__)
#define WINDOWS
#include <windows.h>
#elif defined(__MACH__) || defined(__APPLE__)
#define MACOSX
#include <mach/mach_time.h>
#elif defined(linux) || defined(__linux)  \
    || defined(__CYGWIN32__) || defined(__CYGWIN__)
/* May need to compile with -lrt */
#define LINUX
#include <time.h>
/* TODO Add support for other real-time POSIX systems */
#elif defined(sun) || defined(__sun) || defined(_AIX)
#define RTPOSIX
#include <sys/time.h>
#else /* Unsupported OS */
#if defined(_MSC_VER)
/* MS Vis C++ */
#pragma message "Unsupported OS - get_nanotime() will return 0"
#else
/* #warning May only be supported by GCC, not sure yet... */
#warning "Unsupported OS - get_nanotime() will return 0"
#endif
#endif

typedef enum oset {
    OSET_INVALID,
    OSET_LINKEDLIST,
    OSET_SKIPLIST,
    OSET_BST,
    OSET_TREAP,
    OSET_SPLAY,
    OSET_23TREE
} oset_t;

typedef struct  {
    char* file;
    size_t runs;
    oset_t type;
    bool verbose;
} cmd_args_t;

class utils
{
    public:
        static void print_usage(char* name) {
            fprintf(stdout,"USAGE: %s -i <file> -t <type> -r runs\n",name);
            fprintf(stdout,"  -i : input file\n");
            fprintf(stdout,"  -t : set type [linkedlist,bst,skiplist,treap,23tree,splaytree]\n");
            fprintf(stdout,"  -r : number of runs\n");
            fprintf(stdout,"  -v : verbose mode\n");
            fprintf(stdout,"EXAMPLE: %s -i input.txt -t treap -r 5\n\n",name);
        };
        static void fatal(const char* msg) {
            fprintf(stdout,"ERROR: %s\n",msg);
            exit(EXIT_FAILURE);
        };
        static cmd_args_t* parse_args(int argc,char** argv) {
            cmd_args_t* args = (cmd_args_t*) malloc(sizeof(cmd_args_t));

            /* TODO */

            return args;
        };
        static uint64_t GetNanoTime() {
#if defined(WINDOWS)
            LARGE_INTEGER time_var, frequency;
            QueryPerformanceCounter(&time_var);
            QueryPerformanceFrequency(&frequency);
            /* Convert to nanoseconds */
            return 1.0e9 * time_var.QuadPart / frequency.QuadPart;
#elif defined(MACOSX)
            uint64_t time_var;
            mach_timebase_info_data_t info;
            time_var = mach_absolute_time();
            mach_timebase_info(&info);
            /* Convert to nanoseconds */
            return time_var * (info.numer / info.denom);
#elif defined(LINUX)
            timespec time_var;
            clock_gettime(CLOCK_REALTIME, &time_var);
            /* Convert to nanoseconds */
            return time_var.tv_sec * 1.0e9 + time_var.tv_nsec;
#elif defined(RTPOSIX)
            return gethrtime();
#endif
            return 0;
        };
        static float calc_stddev(uint64_t* times,size_t n) {
            /* TODO */
            return 0.0f;
        };
        static float calculate_avg(uint64_t* times,size_t n) {
            /* TODO */
            return 0.0f;
        };
};

#endif // UTILS_H
