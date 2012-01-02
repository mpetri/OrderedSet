#ifndef ORDEREDSET_H
#define ORDEREDSET_H

#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

class OrderedSet
{
    public:
        OrderedSet(bool v) : verbose(v) {
            n = 0;
        };
        virtual ~OrderedSet() {};
        virtual int64_t Successor(uint32_t x) = 0;
        virtual int64_t Max() = 0;
        virtual int64_t Min() = 0;
        virtual void Insert(uint32_t x) = 0;
        virtual void Remove(uint32_t x) = 0;
        virtual bool Find(uint32_t x) = 0;
        virtual void Print() = 0;
        virtual size_t Items() {
            return n;
        };
    protected:
        inline void VERBOSE(const char* format,...) {
            if (verbose) {
                va_list vargs;
                va_start(vargs,format);
                vfprintf(stderr,format,vargs);
                va_end(vargs);
                fprintf(stderr,"\n");

            }
        }
    protected:
        bool verbose;
        size_t n;
};

#endif // ORDEREDSET_H
