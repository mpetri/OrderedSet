#include "LinkedList.h"

#include <stdio.h>

#include "Utils.h"

LinkedList::LinkedList(bool verbose) : OrderedSet(verbose)
{
    head = NULL;
}

LinkedList::~LinkedList()
{
    llnode_t* tmp;
    while (head) {
        tmp = head->next;
        free(head);
        head = tmp;
    }
}

void
LinkedList::Insert(uint32_t x)
{
    llnode_t* tmp;
    tmp = (llnode_t*) utils::safe_malloc(sizeof(llnode_t));
    mem += sizeof(llnode_t);
    llnode_t* current = head;
    llnode_t* previous = NULL;
    while (current != NULL && current->data < x) {
        previous = current;
        current = current->next;
    }
    /* no duplicates */
    if (current && current->data == x) {
        free(tmp);
        mem -= sizeof(llnode_t);
        return;
    }
    tmp->data = x;
    tmp->next = current;
    n++;
    if (previous == NULL) head = tmp;
    else previous->next = tmp;
}

void
LinkedList::Remove(uint32_t x)
{
    llnode_t* current = head;
    llnode_t* previous = NULL;
    while (current != NULL && current->data != x) {
        previous = current;
        current = current->next;
    }
    if (current) {
        if (previous==NULL) head = current->next;
        else previous->next = current->next;
        n--;
        free(current);
    }
}

bool
LinkedList::Find(uint32_t x)
{
    llnode_t* tmp = head;
    while (tmp && tmp->data <= x) {
        if (tmp->data==x) return true;
        tmp = tmp->next;
    }
    return false;
}

void
LinkedList::Print()
{
    llnode_t* tmp;
    tmp = head;
    while (tmp != NULL) {
        fprintf(stdout,"%d ",tmp->data);
        tmp = tmp->next;
    }
    fprintf(stdout,"\n");
}

int64_t
LinkedList::Successor(uint32_t x)
{
    llnode_t* tmp = head;
    while (tmp && tmp->data <= x) {
        if (tmp->data==x) {
            if (tmp->next) return tmp->next->data;
            return -1;
        }
        tmp = tmp->next;
    }
    return -1;
}

int64_t
LinkedList::Max()
{
    if (!head) return -1;
    llnode_t* tmp = head;
    while (tmp->next != NULL) tmp = tmp->next;
    return tmp->data;
}

int64_t
LinkedList::Min()
{
    if (head) return head->data;
    return -1;
}
