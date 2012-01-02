#ifndef SKIPLIST_H
#define SKIPLIST_H

#include "OrderedSet.h"

#define DEFAULT_PROB       0.5

typedef struct slnode {
    uint32_t data;
    struct slnode** next;
} slnode_t;

class SkipList : public OrderedSet
{
    public:
        SkipList(bool);
        virtual ~SkipList();
        virtual int64_t Successor(uint32_t x);
        virtual int64_t Max();
        virtual int64_t Min();
        virtual void Insert(uint32_t x);
        virtual void Remove(uint32_t x);
        virtual bool Find(uint32_t x);
        virtual void Print();
    private:
        int CalcLevel();
    private:
        slnode_t* head;
        float     p;
        int       height;
};

#endif // SKIPLIST_H
