#ifndef TWOTHREETREE_H
#define TWOTHREETREE_H

#include "OrderedSet.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef UINT32_MAX
#define UINT32_MAX  (0xffffffff)
#endif

typedef struct ttnode {
    uint32_t data[2];
    struct ttnode* parent;
    struct ttnode* left;
    struct ttnode* middle;
    struct ttnode* right;
} ttnode_t;

class TwoThreeTree : public OrderedSet
{
    public:
        TwoThreeTree(bool);
        virtual ~TwoThreeTree();
        virtual int64_t Successor(uint32_t x);
        virtual int64_t Max();
        virtual int64_t Min();
        virtual void Insert(uint32_t x);
        virtual void Remove(uint32_t x);
        virtual bool Find(uint32_t x);
        virtual void Print();
    private:
        void Free(ttnode_t*);
        void Print(ttnode_t*);
        ttnode_t* Insert(ttnode_t* n,uint32_t x);
        ttnode_t* NewNode(uint32_t x,uint32_t y = UINT32_MAX);
        ttnode_t* SplitNode(ttnode_t* a,ttnode_t* b);
        void MergeNodes(ttnode_t* old,ttnode_t* tn);
        inline bool IsTerminal(ttnode_t*);
        inline bool IsTwoNode(ttnode_t*);
        inline ttnode_t* FindMin(ttnode_t*);
        inline ttnode_t* FindMax(ttnode_t*);
        inline ttnode_t* Sibling(ttnode_t*);
    private:
        ttnode_t* root;
};

#endif // TWOTHREETREE_H
