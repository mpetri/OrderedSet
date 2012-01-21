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
            args->runs = 1;
            args->file = NULL;
            args->type = OSET_INVALID;
            args->verbose = false;

            int opt = GETOPT_FINISHED;

            if (argc <= 1) {
                utils::print_usage(argv[0]);
                exit(EXIT_FAILURE);
            }
            while ((opt = getopt(argc, argv, "i:t:r:v")) != GETOPT_FINISHED) {
                switch (opt) {
                    case 'i':
                        args->file = optarg;
                        break;
                    case 't':
                        if (strcmp(optarg, "linkedlist") == 0) {
                            args->type = OSET_LINKEDLIST;
                        } else if (strcmp(optarg, "bst") == 0) {
                            args->type = OSET_BST;
                        } else if (strcmp(optarg, "skiplist") == 0) {
                            args->type = OSET_SKIPLIST;
                        } else if (strcmp(optarg, "treap") == 0) {
                            args->type = OSET_TREAP;
                        } else if (strcmp(optarg, "23tree") == 0) {
                            args->type = OSET_23TREE;
                        } else if (strcmp(optarg, "splaytree") == 0) {
                            args->type = OSET_SPLAY;
                        } else {
                            fprintf(stderr, "ERROR: mode <%s> unknown!\n", optarg);
                            exit(EXIT_FAILURE);
                        }
                        break;
                    case 'r':
                        args->runs = atoi(optarg);
                        break;
                    case 'v':
                        args->verbose = true;
                        break;
                    case 'h':
                    default:
                        utils::print_usage(argv[0]);
                        exit(EXIT_SUCCESS);
                }
            }

            if (args->file == NULL || args->type == OSET_INVALID) {
                utils::print_usage(argv[0]);
                exit(EXIT_FAILURE);
            }

            fprintf(stdout,"FILE :                       %s\n",args->file);

            switch (args->type) {
                case OSET_LINKEDLIST:
                    fprintf(stdout,"TYPE :                       linked list\n");
                    break;
                case OSET_BST:
                    fprintf(stdout,"TYPE :                       bst\n");
                    break;
                case OSET_SKIPLIST:
                    fprintf(stdout,"TYPE :                       skip list\n");
                    break;
                case OSET_TREAP:
                    fprintf(stdout,"TYPE :                       treap\n");
                    break;
                case OSET_23TREE:
                    fprintf(stdout,"TYPE :                       2-3 tree\n");
                    break;
                case OSET_SPLAY:
                    fprintf(stdout,"TYPE :                       splay tree\n");
                    break;
                case OSET_INVALID:
                    fprintf(stdout,"Invalid type.");
                    utils::print_usage(argv[0]);
                    exit(EXIT_FAILURE);
            }

            fprintf(stdout,"RUNS :                       %zu\n",args->runs);

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
            size_t  i;
            uint64_t  total = 0;
            uint64_t  total_square = 0;
            uint64_t  stddev;
            uint64_t  avg;
            for (i = 0; i < n; i++) {
                total += times[i];
                total_square += (times[i] * times[i]);
            }
            avg = total / n;
            stddev = sqrt((total_square / n) - (avg * avg));
            return (((float)(stddev)) / 1.0e9);
        };
        static float calculate_avg(uint64_t* times,size_t n) {
            size_t i;
            uint64_t total = 0;
            for (i = 0; i < n; i++) {
                total += times[i];
            }
            /* convert to right format */
            return (((float)(total / n)) / 1.0e9);
        };
        static void* safe_malloc(size_t size) {
            void* mem_block = NULL;
            if ((mem_block = calloc(1,size)) == NULL) {
                fprintf(stderr, "ERROR: safe_malloc(%zu) cannot allocate memory.", size);
                exit(EXIT_FAILURE);
            }
            return (mem_block);
        };
        static void* safe_realloc(void* old_mem, size_t new_size) {
            if ((old_mem = realloc(old_mem, new_size)) == NULL) {
                fprintf(stderr, "ERROR: safe_realloc() cannot allocate"
                        "%zu blocks of memory.\n", new_size);
                exit(EXIT_FAILURE);
            }
            return (old_mem);
        };
};

#endif // UTILS_H
