#include <iostream>
#include <stdint.h>

#include "Utils.h"
#include "OrderedSet.h"
#include "LinkedList.h"
#include "BinarySearchTree.h"
#include "SkipList.h"
#include "Treap.h"
#include "TwoThreeTree.h"

#define OP_INSERT    'I'
#define OP_REMOVE    'R'
#define OP_PRINT     'P'
#define OP_FIND      'F'
#define OP_SUCCESSOR 'S'
#define OP_MIN       'm'
#define OP_MAX       'M'

OrderedSet*
CreateOrderedSet(oset_t type,bool verbose)
{
    OrderedSet* oset = NULL;
    switch (type) {
        case OSET_LINKEDLIST:
            oset = new LinkedList(verbose);
            break;
        case OSET_BST:
            oset = new BinarySearchTree(verbose);
            break;
        case OSET_SKIPLIST:
            oset = new SkipList(verbose);
            break;
        case OSET_TREAP:
            oset = new Treap(verbose);
            break;
        case OSET_23TREE:
            oset = new TwoThreeTree(verbose);
            break;
        case OSET_SPLAY:
            oset = new LinkedList(verbose);
            break;
        case OSET_INVALID:
        default:
            break;
    }
    return oset;
}

int
main(int argc,char** argv)
{
    size_t n,mem;
    char* O;
    uint32_t* D;
    FILE* f;
    cmd_args_t* args;

    /* parse command line parameters */
    args = utils::parse_args(argc,argv);

    /* read data */
    f = fopen(args->file,"r");
    if (!f) utils::fatal("cannot read input file.");
    if (1 != fscanf(f,"%zu\n",&n)) utils::fatal("error reading input file.");
    O = (char*) utils::safe_malloc(n*sizeof(char));
    D = (uint32_t*) utils::safe_malloc(n*sizeof(uint32_t));
    for (size_t i=0; i<n; i++) {
        if (2 != fscanf(f,"%c %u\n",&O[i],&D[i]))
            utils::fatal("error reading commands from input file.");
    }
    fclose(f);

    uint64_t* times = (uint64_t*) utils::safe_malloc(args->runs*sizeof(uint64_t));

    /* perform operations */
    for (size_t j=0; j<args->runs; j++) {
        OrderedSet* oset = CreateOrderedSet(args->type,args->verbose);
        uint64_t start = utils::GetNanoTime();
        for (size_t i=0; i<n; i++) {
            switch (O[i]) {
                case OP_INSERT:
                    oset->Insert(D[i]);
                    break;
                case OP_REMOVE:
                    oset->Remove(D[i]);
                    break;
                case OP_FIND:
                    oset->Find(D[i]);
                    break;
                case OP_PRINT:
                    oset->Print();
                    break;
                case OP_MIN:
                    oset->Min();
                    break;
                case OP_MAX:
                    oset->Max();
                    break;
                default:
                    utils::fatal("unknown set operation.");
            }
        }
        uint64_t stop = utils::GetNanoTime();
        times[j] = stop - start;
        mem = oset->Memory();
        delete oset;
    }

    float avg = utils::calculate_avg(times, args->runs);
    float stdev = utils::calc_stddev(times, args->runs);

    /* output */
    fprintf(stdout,"Number of operations:        %zu\n",n);
    fprintf(stdout,"Time:                        %.5f s+/- %.5f s\n",avg,stdev);
    fprintf(stdout,"Memory:                      %zu kb MBytes\n",mem/1024);

    /* cleanup */
    free(times);
    free(O);
    free(D);
    free(args);

    return EXIT_SUCCESS;
}
