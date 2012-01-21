#include "BinarySearchTree.h"
#include <stdio.h>
#include "Utils.h"

BinarySearchTree::BinarySearchTree(bool verbose) : OrderedSet(verbose)
{
    root = NULL;
}

BinarySearchTree::~BinarySearchTree()
{
    Free(root);
}

void
BinarySearchTree::Free(bstnode_t* no)
{
    if (no) {
        Free(no->left);
        Free(no->right);
        free(no);
    }
}

void
BinarySearchTree::Insert(uint32_t x)
{
    bstnode_t* no = (bstnode_t*) utils::safe_malloc(sizeof(bstnode_t));
    mem += sizeof(bstnode_t);
    no->data = x;
    no->left = no->right = no->parent = NULL;

    bstnode_t* tmp = root;
    bstnode_t* parent = NULL;
    while (tmp) {
        parent = tmp;
        if (tmp->data>x) tmp=tmp->left;
        else if (tmp->data<x) tmp=tmp->right;
        else {
            free(no);
            mem -= sizeof(bstnode_t);
            return;
        }
    }
    if (parent) {
        if (parent->data < x) parent->right = no;
        else parent->left = no;
        no->parent = parent;
    } else {
        root = no;
    }
    n++;
}

bool
BinarySearchTree::Find(uint32_t x)
{
    bstnode_t* tmp = root;
    while (tmp) {
        if (tmp->data>x) tmp=tmp->left;
        else if (tmp->data<x) tmp=tmp->right;
        else return true;
    }
    return false;
}

void
BinarySearchTree::Remove(uint32_t x)
{
    root=Remove(root,x);
}

bstnode_t*
BinarySearchTree::Remove(bstnode_t* no,uint32_t x)
{
    if (no) {
        if (no->data > x) no->left =  Remove(no->left,x);
        else if (no->data < x) no->right = Remove(no->right,x);
        else {
            /* we found the item we want to delete */
            if (no->left && no->right) {
                bstnode_t* r = FindMin(no->right);
                no->data = r->data;
                no->right = Remove(no->right,no->data);
                if (no->right) no->right->parent = no;
            } else {
                /* one or zero childs */
                bstnode_t* tmp = no;
                if (!no->left) no = no->right;
                else if (!no->right) no = no->left;
                if (no) no->parent = tmp->parent;
                mem -= sizeof(bstnode_t);
                free(tmp);
            }
        }
        return no;
    }
    return NULL;
}

bstnode_t*
BinarySearchTree::FindMin(bstnode_t* no)
{
    if (no) {
        if (no->left) return FindMin(no->left);
        return no;
    }
    return NULL;
}

bstnode_t*
BinarySearchTree::FindMax(bstnode_t* no)
{
    if (no) {
        if (no->right) return FindMax(no->right);
        return no;
    }
    return NULL;
}

void
BinarySearchTree::Print()
{
    if (root) {
        Print(root);
        fprintf(stdout,"\n");
    }
}

void
BinarySearchTree::Print(bstnode_t* no)
{
    if (no) {
        Print(no->left);
        fprintf(stdout,"%u ",no->data);
        Print(no->right);
    }
}

int64_t
BinarySearchTree::Min()
{
    bstnode_t* min = FindMin(root);
    if (min) return min->data;
    return -1;
}

int64_t
BinarySearchTree::Max()
{
    bstnode_t* max = FindMax(root);
    if (max) return max->data;
    return -1;
}

int64_t
BinarySearchTree::Successor(uint32_t x)
{
    bstnode_t* tmp = root;
    while (tmp) {
        if (tmp->data>x) tmp=tmp->left;
        else if (tmp->data<x) tmp=tmp->right;
        else break;
    }
    if (tmp) {
        if (tmp->right) {
            bstnode_t* min = FindMin(tmp->right);
            return min->data;
        } else {
            bstnode_t* parent = tmp->parent;
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