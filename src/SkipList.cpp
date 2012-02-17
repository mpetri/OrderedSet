#include "SkipList.h"

#include <stdio.h>

#include "Utils.h"

SkipList::SkipList(bool verbose) : OrderedSet(verbose)
{
    head = NULL;
    height = 0;
    p = DEFAULT_PROB;
}

SkipList::~SkipList()
{
    slnode_t* tmp;
    while (head) {
        tmp = head->next[0];
        free(head->next);
        free(head);
        head = tmp;
    }
}

void
SkipList::Insert(uint32_t x)
{
    /* calc level of new node */
    int lvl = CalcLevel();

    if (lvl+1 > height) {
        if (head) {
            head->next = (slnode_t**) utils::safe_realloc(head->next,(lvl+1)*sizeof(slnode_t*));
            for (int i=height; i<=lvl; i++) head->next[i] = NULL;
            head->lvl = lvl+1;
        }
        height = lvl+1;
    }

    /* create new node */
    slnode_t* tmp;
    tmp = (slnode_t*) utils::safe_malloc(sizeof(slnode_t));
    tmp->data = x;
    tmp->next = (slnode_t**) utils::safe_malloc((lvl+1)*sizeof(slnode_t*));
    tmp->lvl = lvl+1;
    mem += sizeof(slnode_t);
    mem += (lvl+1)*sizeof(slnode_t*);

    slnode_t** save = (slnode_t**) utils::safe_malloc((lvl+1)*sizeof(slnode_t*));

    if (head==NULL) head = tmp;
    else {
        /* never replace head */
        if (head->data>x) {
            tmp->data = head->data;
            head->data = x;
            x = tmp->data;
        }

        /* move through the list */
        slnode_t* cur = head;
        int i = height-1;
        bool found = false;
        for (i=height-1; i>=0; i--) {
            while (cur->next[i]!=NULL && cur->next[i]->data < x) cur = cur->next[i];
            if (cur->next[i]!=NULL&&cur->next[i]->data ==x) found = true;
            if (i<=lvl) {
                tmp->next[i] = cur->next[i];
                save[i] = cur;
            }
        }
        if (found) {
            mem -= sizeof(slnode_t);
            mem -= (tmp->lvl)*sizeof(slnode_t*);
            free(tmp->next);
            free(tmp);
            n--;
        } else {
            for (int i=lvl; i>=0; i--) save[i]->next[i] = tmp;
        }
    }
    free(save);
    n++;
}

void
SkipList::Remove(uint32_t x)
{
    slnode_t* tmp = head;
    if (head && head->data==x && n == 1) {
        mem = 0;
        free(head->next);
        free(head);
        n = 0;
        head = NULL;
        height = 0;
        return;
    }

    /* never remove head unless last item */
    if (head && head->data==x && n > 1) {
        head->data = head->next[0]->data;
        head->next[0]->data = x;
    }

    slnode_t* del = NULL;
    for (int i=height-1; i>=0; i--) {
        while (tmp->next[i]!=NULL && tmp->next[i]->data <x) tmp=tmp->next[i];
        /* unlink */
        if (tmp->next[i]!=NULL && tmp->next[i]->data==x) {
            del=tmp->next[i];
            tmp->next[i] = tmp->next[i]->next[i];
        }
    }
    if (del) {
        mem -= sizeof(slnode_t);
        mem -= (del->lvl)*sizeof(slnode_t*);
        free(del->next);
        free(del);
        n--;
    }
}

bool
SkipList::Find(uint32_t x)
{
    slnode_t* tmp = head;
    int lvl = height -1;
    while (lvl>=0) {
        while (tmp->next[lvl]!=NULL && tmp->next[lvl]->data <= x) tmp = tmp->next[lvl];
        if (tmp->data == x) return true;
        lvl--;
    }
    return false;
}

void
SkipList::Print()
{
    slnode_t* tmp;
    tmp = head;
    while (tmp != NULL) {
        fprintf(stdout,"%d ",tmp->data);
        tmp = tmp->next[0];
    }
    fprintf(stdout,"\n");
}

int
SkipList::CalcLevel()
{
    int lvl=0;
    while (rand() < p*RAND_MAX) lvl++;
    return lvl;
}

int64_t
SkipList::Successor(uint32_t x)
{
    slnode_t* tmp = head;
    int lvl = height -1;
    while (lvl>=0) {
        while (tmp->next[lvl]!=NULL && tmp->next[lvl]->data <= x) tmp = tmp->next[lvl];
        if (tmp->data == x) {
            if (tmp->next[0] != NULL) return tmp->next[0]->data;
            return -1;
        }
        lvl--;
    }
    return -1;
}

int64_t
SkipList::Max()
{
    if (!head) return -1;

    slnode_t* tmp = head;
    int lvl = height -1;
    while (lvl>0) {
        while (tmp->next[lvl]!=NULL) tmp = tmp->next[lvl];
        lvl--;
    }
    while (tmp->next[0] != NULL) tmp = tmp->next[0];
    return tmp->data;
}

int64_t
SkipList::Min()
{
    if (head) return head->data;
    return -1;
}



