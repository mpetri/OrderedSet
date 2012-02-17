#include "Treap.h"

#include <stdio.h>

#include "Utils.h"

Treap::Treap(bool verbose) : OrderedSet(verbose)
{
    root = NULL;
}

Treap::~Treap()
{
    Free(root);
}

void
Treap::Free(tnode_t* no)
{
    if (no) {
        Free(no->left);
        Free(no->right);
        free(no);
    }
}

void
Treap::Insert(uint32_t x)
{
    tnode_t* no = (tnode_t*) utils::safe_malloc(sizeof(tnode_t));
    no->data = x;
    no->prio = rand();
    no->left = no->right = no->parent = NULL;
    mem += sizeof(tnode_t);

    if (!root) root = no;
    else {
        tnode_t* tmp = root;
        tnode_t* parent = NULL;
        while (tmp) {
            parent = tmp;
            if (tmp->data<x) tmp=tmp->right;
            else if (tmp->data>x) tmp=tmp->left;
            else {
                mem -= sizeof(tnode_t);
                free(no);
                return;
            }
        }
        if (parent->data < no->data) parent->right = no;
        else parent->left = no;
        no->parent = parent;

        /* heap adjustment */
        tmp = no;
        while (tmp->parent != NULL && tmp->parent->prio < tmp->prio) {
            Rotate(tmp->parent,tmp);
        }
        if (tmp->parent==NULL) root = tmp;
    }
    n++;
}

void
Treap::Remove(uint32_t x)
{
    tnode_t* tmp = root;
    while (tmp) {
        if (tmp->data<x) tmp = tmp->right;
        else if (tmp->data>x) tmp = tmp->left;
        else break;
    }
    if (tmp) {
        /* bubble down */
        tnode_t* child = NULL;
        while (tmp->left || tmp->right) {
            if (tmp->left && tmp->right) {
                if (tmp->left->prio > tmp->right->prio) child = tmp->left;
                else child = tmp->right;
            } else if (tmp->left) child = tmp->left;
            else child = tmp->right;
            Rotate(tmp,child);
        }
        /* delete */
        if (tmp->parent) {
            if (tmp->parent->left == tmp) tmp->parent->left = NULL;
            else tmp->parent->right = NULL;
            if (tmp->parent->parent == NULL) root = tmp->parent;
            free(tmp);
        } else {
            root = NULL;
            free(tmp);
        }
        mem -= sizeof(tnode_t);
        n--;
    }
}

bool
Treap::Find(uint32_t x)
{
    tnode_t* tmp = root;
    while (tmp) {
        if (tmp->data<x) tmp=tmp->right;
        else if (tmp->data>x) tmp=tmp->left;
        else return true;
    }
    return false;
}

void
Treap::Print(tnode_t* no)
{
    if (no) {
        Print(no->left);
        fprintf(stdout,"%u ",no->data);
        Print(no->right);
    }
}

void
Treap::Rotate(tnode_t* parent,tnode_t* child)
{
    /* adjust grandparent */
    tnode_t* gparent = parent->parent;
    child->parent = gparent;
    if (gparent) {
        if (gparent->left == parent) gparent->left = child;
        else gparent->right = child;
    } else {
        root = child;
    }

    /* rotate */
    parent->parent = child;
    if (child == parent->right) {
        parent->right = child->left;
        if (child->left) child->left->parent = parent;
        child->left = parent;
    } else {
        parent->left = child->right;
        if (child->right) child->right->parent = parent;
        child->right = parent;
    }
}

int64_t
Treap::Min()
{
    tnode_t* min = FindMin(root);
    if (min) return min->data;
    return -1;
}

int64_t
Treap::Max()
{
    tnode_t* max = FindMax(root);
    if (max) return max->data;
    return -1;
}

tnode_t*
Treap::FindMin(tnode_t* no)
{
    if (no) {
        if (no->left) return FindMin(no->left);
        return no;
    }
    return NULL;
}

tnode_t*
Treap::FindMax(tnode_t* no)
{
    if (no) {
        if (no->right) return FindMax(no->right);
        return no;
    }
    return NULL;
}

int64_t
Treap::Successor(uint32_t x)
{
    tnode_t* tmp = root;
    while (tmp) {
        if (tmp->data>x) tmp=tmp->left;
        else if (tmp->data<x) tmp=tmp->right;
        else break;
    }
    if (tmp) {
        if (tmp->right) {
            tnode_t* min = FindMin(tmp->right);
            return min->data;
        } else {
            tnode_t* parent = tmp->parent;
            while (parent!=NULL && parent->right == tmp) {
                tmp = tmp->parent;
                parent = tmp->parent;
            }
            if (parent) return parent->data;
            else return -1;
        }
    }
    return -1;
}

