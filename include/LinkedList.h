#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "OrderedSet.h"

typedef struct llnode {
    uint32_t data;
    struct llnode* next;
} llnode_t;

class LinkedList : public OrderedSet
{
    public:
        LinkedList(bool);
        virtual ~LinkedList();
        virtual int64_t Max();
        virtual int64_t Min();
        virtual int64_t Successor(uint32_t x);
        virtual void Insert(uint32_t x);
        virtual void Remove(uint32_t x);
        virtual bool Find(uint32_t x);
        virtual void Print();
    private:
        llnode_t* head;
};

#endif // LINKEDLIST_H

