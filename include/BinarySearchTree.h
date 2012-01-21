#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include "OrderedSet.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct bstnode {
    uint32_t data;
    struct bstnode* parent;
    struct bstnode* left;
    struct bstnode* right;
} bstnode_t;

class BinarySearchTree : public OrderedSet
{
    public:
        BinarySearchTree(bool);
        virtual ~BinarySearchTree();
        virtual int64_t Successor(uint32_t x);
        virtual int64_t Max();
        virtual int64_t Min();
        virtual void Insert(uint32_t x);
        virtual void Remove(uint32_t x);
        virtual bool Find(uint32_t x);
        virtual void Print();
    private:
        void Print(bstnode_t* n);
        bstnode_t* Remove(bstnode_t* n,uint32_t x);
        bstnode_t* FindMin(bstnode_t* n);
        bstnode_t* FindMax(bstnode_t* n);
        void Free(bstnode_t* n);
    private:
        bstnode_t* root;
};

#endif // BINARYSEARCHTREE_H
