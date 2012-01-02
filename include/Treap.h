#ifndef TREAP_H
#define TREAP_H

#include "OrderedSet.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct tnode {
    uint32_t data;
    float prio;
    struct tnode* left;
    struct tnode* right;
    struct tnode* parent;
} tnode_t;

class Treap : public OrderedSet
{
    public:
        Treap(bool);
        virtual ~Treap();
        virtual int64_t Successor(uint32_t x);
        virtual int64_t Min();
        virtual int64_t Max();
        virtual void Insert(uint32_t x);
        virtual void Remove(uint32_t x);
        virtual bool Find(uint32_t x);
        virtual void Print();
    private:
        tnode_t* root;
};

#endif // TREAP_H
