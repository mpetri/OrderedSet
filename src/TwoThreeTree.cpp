#include "TwoThreeTree.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "Utils.h"

TwoThreeTree::TwoThreeTree(bool verbose) : OrderedSet(verbose)
{
    root = NULL;
}

TwoThreeTree::~TwoThreeTree()
{
    Free(root);
}

void
TwoThreeTree::Free(ttnode_t* no)
{
    if (no) {
        Free(no->left);
        Free(no->middle);
        Free(no->right);
        free(no);
    }
}

void
TwoThreeTree::Remove(uint32_t x)
{
    VERBOSE("REMOVE(%u)",x);
    ttnode_t* tmp = root;
    /* find node we want to remove */
    while (tmp) {
        if (IsTwoNode(tmp)) {
            if (tmp->data[0]>x) tmp=tmp->left;
            else if (tmp->data[0]<x) tmp=tmp->right;
            else break;
        } else {
            if (tmp->data[0]>x) tmp=tmp->left;
            else if (tmp->data[0]<x && tmp->data[1]>x) tmp = tmp->middle;
            else if (tmp->data[1]<x) tmp = tmp->right;
            else break;
        }
    }

    if (!tmp) return; /* not found */
    n--;

    if (!IsTerminal(tmp)) {
        /* replace with successor */
        ttnode_t* succ;
        if (IsTwoNode(tmp)) {
            succ = FindMin(tmp->right);
            tmp->data[0] = succ->data[0];
            succ->data[0] = x;
            tmp = succ;
        } else {
            if (tmp->data[0]==x) {
                succ = FindMax(tmp->left);
                if (IsTwoNode(succ)) {
                    tmp->data[0] = succ->data[0];
                    succ->data[0] = x;
                } else {
                    tmp->data[0] = succ->data[1];
                    succ->data[1] = x;
                }
            } else {
                succ = FindMin(tmp->right);
                tmp->data[1] = succ->data[0];
                succ->data[0] = x;
            }
            tmp = succ;
        }
    }

    if (IsTerminal(tmp)) {
        if (IsTwoNode(tmp)) {
            tmp->data[0] = UINT32_MAX;
            tmp->left = tmp->middle = tmp->right = NULL;
            if (!tmp->parent) {
                free(tmp); /* we are removing the root */
                root = NULL;
                return;
            }
        } else {
            /* transform to 2-node */
            if (tmp->data[0]==x) {
                tmp->data[0] = tmp->data[1];
                tmp->left = tmp->middle;
                tmp->middle = NULL;
            } else {
                tmp->right = tmp->middle;
                tmp->middle = NULL;
            }
            tmp->data[1] = UINT32_MAX;
            return;
        }
    }

    /* bubble up hole to the top */
    while (tmp) {
        ttnode_t* p = tmp->parent;
        if (IsTwoNode(p)) {
            ttnode_t* sib = Sibling(tmp);
            if (IsTwoNode(sib)) {
                /* case 1 */
                if (sib->data[0] < p->data[0]) {
                    p->data[1] = p->data[0];
                    p->data[0] = sib->data[0];
                    p->left = sib->left;
                    p->middle = sib->right;
                    p->right = tmp->left;
                } else {
                    p->data[1] = sib->data[0];
                    p->right = sib->right;
                    p->middle = sib->left;
                    p->left = tmp->left;
                }
                if (p->left) p->left->parent = p;
                if (p->middle) p->middle->parent = p;
                if (p->right) p->right->parent = p;
                free(sib);
                tmp->left = p;
                tmp->parent = p->parent;
                tmp->right = tmp->middle = NULL;
                if (p->parent) {
                    if (p->parent->left==p) p->parent->left = tmp;
                    if (p->parent->middle==p) p->parent->middle = tmp;
                    if (p->parent->right==p) p->parent->right = tmp;
                } else {
                    tmp = NULL; /* we are at root */
                }
                p->parent = tmp;
            } else {
                /* case 2 */
                tmp->data[0] = p->data[0];
                if (p->left==sib) {
                    p->data[0] = sib->data[1];
                    sib->data[1] = UINT32_MAX;
                    tmp->right = tmp->left;
                    tmp->left = sib->right;
                    if (tmp->left) tmp->left->parent = tmp;
                    sib->right = sib->middle;
                } else {
                    p->data[0] = sib->data[0];
                    sib->data[0] = sib->data[1];
                    sib->data[1] = UINT32_MAX;
                    tmp->right = sib->left;
                    if (tmp->right) tmp->right->parent = tmp;
                    sib->left = sib->middle;
                }
                p->middle = tmp->middle = sib->middle = NULL;
                tmp = NULL;
            }
        } else {
            /* case 3/4 */
            if (p->left==tmp) {
                /* hole on the left */
                if (p->middle && IsTwoNode(p->middle)) {
                    /* case 3a */
                    tmp->data[0] = p->data[0];
                    tmp->data[1] = p->middle->data[0];
                    p->data[0] = p->data[1];
                    p->data[1] = UINT32_MAX;
                    tmp->middle = p->middle->left;
                    tmp->right = p->middle->right;
                    if (tmp->middle) tmp->middle->parent = tmp;
                    if (tmp->right) tmp->right->parent = tmp;
                    free(p->middle);
                    p->middle = NULL;
                } else if (p->middle && !IsTwoNode(p->middle)) {
                    /* case 4a */
                    tmp->data[0] = p->data[0];
                    p->data[0] = p->middle->data[0];
                    p->middle->data[0] = p->middle->data[1];
                    p->middle->data[1] = UINT32_MAX;
                    tmp->right = p->middle->left;
                    if (tmp->right) tmp->right->parent = tmp;
                    p->middle->left = p->middle->middle;
                    p->middle->middle = NULL;
                }
            } else if (p->right==tmp) {
                /* hole on the right */
                if (p->middle && IsTwoNode(p->middle)) {
                    /* case 3b */
                    tmp->data[0] = p->middle->data[0];
                    tmp->data[1] = p->data[1];
                    p->data[1] = UINT32_MAX;
                    tmp->right = tmp->left;
                    tmp->left = p->middle->left;
                    tmp->middle = p->middle->right;
                    if (tmp->left) tmp->left->parent = tmp;
                    if (tmp->middle) tmp->middle->parent = tmp;
                    free(p->middle);
                    p->middle = NULL;
                } else if (p->middle && !IsTwoNode(p->middle)) {
                    /* case 4b */
                    tmp->data[0] = p->data[1];
                    p->data[1] = p->middle->data[1];
                    p->middle->data[1] = UINT32_MAX;
                    tmp->right = tmp->left;
                    tmp->left = p->middle->right;
                    if (tmp->left) tmp->left->parent = tmp;
                    p->middle->right = p->middle->middle;
                    p->middle->middle = NULL;
                }
            } else {
                /* hole in the middle */
                if (p->left && IsTwoNode(p->left)) {
                    /* case 3a */
                    p->left->data[1] = p->data[0];
                    p->left->middle = p->left->right;
                    p->left->right = tmp->left;
                    if (p->left->right) p->left->right->parent = p->left;
                    p->data[0] = p->data[1];
                    p->data[1] = UINT32_MAX;
                    free(tmp);
                    p->middle = NULL;
                } else if (p->right && IsTwoNode(p->right)) {
                    /* case 3b */
                    p->right->data[1] = p->right->data[0];
                    p->right->data[0] = p->data[1];
                    p->right->middle = p->right->left;
                    if (p->right->middle) p->right->middle->parent = p->right;
                    p->right->left = tmp->left;
                    if (p->right->left) p->right->left->parent = p->right;
                    p->data[1] = UINT32_MAX;
                    free(tmp);
                    p->middle = NULL;
                } else if (p->left && !IsTwoNode(p->left)) {
                    /* case 4a */
                    tmp->data[0] = p->data[0];
                    p->data[0] = p->left->data[1];
                    p->left->data[1] = UINT32_MAX;
                    tmp->right = tmp->left;
                    tmp->left = p->left->right;
                    if (tmp->left) tmp->left->parent = tmp;
                    p->left->right = p->left->middle;
                    p->left->middle = NULL;
                } else {
                    /* case 4b */
                    tmp->data[0] = p->data[1];
                    p->data[1] = p->right->data[0];
                    p->right->data[0] = p->right->data[1];
                    p->right->data[1] = UINT32_MAX;
                    tmp->right = p->right->left;
                    if (tmp->right) tmp->right->parent = tmp->right;
                    p->right->left = p->right->middle;
                    p->right->middle = NULL;
                }
            }
            tmp = NULL;
        }
    }
}

bool
TwoThreeTree::Find(uint32_t x)
{
    VERBOSE("FIND(%u)",x);
    ttnode_t* tmp = root;
    while (tmp) {
        if (IsTwoNode(tmp)) {
            if (tmp->data[0]>x) tmp=tmp->left;
            else if (tmp->data[0]<x) tmp=tmp->right;
            else return true;
        } else {
            if (tmp->data[0]>x) tmp=tmp->left;
            else if (tmp->data[0]<x && tmp->data[1]>x) tmp = tmp->middle;
            else if (tmp->data[1]<x) tmp = tmp->right;
            else return true;
        }
    }
    return false;
}

ttnode_t*
TwoThreeTree::FindMin(ttnode_t* no)
{
    ttnode_t* tmp = no;
    while (tmp && tmp->left) {
        tmp = tmp->left;
    }
    return tmp;
}

ttnode_t*
TwoThreeTree::FindMax(ttnode_t* no)
{
    ttnode_t* tmp = no;
    while (tmp && tmp->right) {
        tmp = tmp->right;
    }
    return tmp;
}

ttnode_t*
TwoThreeTree::Sibling(ttnode_t* no)
{
    if (no->parent) {
        if (no->parent->left!=no && no->parent->left != NULL)
            return no->parent->left;
        if (no->parent->right!=no && no->parent->right != NULL)
            return no->parent->right;
        if (no->parent->middle!=no && no->parent->middle != NULL)
            return no->parent->middle;
    }
    return NULL;
}

void
TwoThreeTree::Print()
{
    Print(root);
    fprintf(stdout,"\n");
}

void
TwoThreeTree::Print(ttnode_t* no)
{
    if (no) {
        Print(no->left);
        fprintf(stdout,"%u ",no->data[0]);
        if (!IsTwoNode(no)) {
            Print(no->middle);
            fprintf(stdout,"%u ",no->data[1]);
        }
        Print(no->right);
    }
}

void
TwoThreeTree::Insert(uint32_t x)
{
    VERBOSE("INSERT(%u)",x);
    if (!root) {
        root = NewNode(x);
        n = 1;
    } else {
        ttnode_t* tmp = Insert(root,x);
        if (tmp) root = tmp;
    }
}

ttnode_t*
TwoThreeTree::Insert(ttnode_t* tn,uint32_t x)
{
    VERBOSE("INSERTR(%u)",x);
    ttnode_t* tmp;
    if (IsTerminal(tn)) {
        if (tn->data[0] == x||tn->data[1]==x) return NULL;
        n++;
        if (IsTwoNode(tn)) {
            VERBOSE("TERMINAL(%u)",tn->data[0]);
            MergeNodes(tn,NewNode(x));
        } else {
            VERBOSE("TERMINAL(%u,%u)",tn->data[0],tn->data[1]);
            return SplitNode(tn,NewNode(x));
        }
    } else {
        if (IsTwoNode(tn)) {
            VERBOSE("NON-TERMINAL(%u)",tn->data[0]);
            if (tn->data[0] < x) tmp = Insert(tn->right,x);
            else if (tn->data[0] > x) tmp = Insert(tn->left,x);
            else /* already in tree */ return NULL;
            if (tmp) {
                /* upward */ MergeNodes(tn,tmp);
            }
        } else {
            VERBOSE("NON-TERMINAL(%u,%u)",tn->data[0],tn->data[1]);
            if (tn->data[0] > x) tmp = Insert(tn->left,x);
            else if (tn->data[0]<x && tn->data[1]>x) tmp = Insert(tn->middle,x);
            else if (tn->data[1]<x) tmp = Insert(tn->right,x);
            else {
                /* already in tree */ return NULL;
            }
            if (tmp) return SplitNode(tn,tmp);
        }
    }
    return NULL;
}

ttnode_t*
TwoThreeTree::SplitNode(ttnode_t* tnode,ttnode_t* ins)
{
    VERBOSE("SPLITNODE( (%u,%u) (%u))",tnode->data[0],tnode->data[1],ins->data[0]);
    uint32_t left,right,middle;
    ttnode_t* tl,*tr,*tlr,*trl;
    tlr = trl = NULL;
    /* find out which gets promoted */
    if (tnode->data[0]<ins->data[0]) {
        if (ins->data[0]<tnode->data[1]) {
            middle = ins->data[0];
            left = tnode->data[0];
            right = tnode->data[1];
            tlr = ins->left;
            trl = ins->right;
            tl = tnode->left;
            tr = tnode->right;
        } else {
            middle = tnode->data[1];
            left = tnode->data[0];
            right = ins->data[0];

            trl = ins->left;
            tlr = tnode->middle;

            tr = ins->right;
            tl = tnode->left;
        }
    } else {
        middle = tnode->data[0];
        left = ins->data[0];
        right = tnode->data[1];

        tr = tnode->right;
        trl = tnode->middle;
        tl = ins->left;
        tlr = ins->right;
    }

    /* perform the split */
    ttnode_t* tnp = tnode->parent;
    ins->data[0] = middle;
    ins->data[1] = UINT32_MAX;
    ins->parent = tnp;
    if (ins->parent) {
        if (ins->parent->left == tnode) ins->parent->left = ins;
        if (ins->parent->right == tnode) ins->parent->right = ins;
        if (ins->parent->middle == tnode) ins->parent->middle = ins;
    }

    ttnode_t* ln = NewNode(left);
    ln->left = tl;
    ln->right = tlr;
    ttnode_t* rn = tnode;
    rn->data[0] = right;
    rn->data[1] = UINT32_MAX;
    rn->left = trl;
    rn->right = tr;
    ln->parent = ins;
    rn->parent = ins;
    ins->left = ln;
    ins->right = rn;
    rn->middle = ln->middle = ins->middle = NULL;

    if (ln->left) ln->left->parent = ln;
    if (rn->left) rn->left->parent = rn;
    if (ln->right) ln->right->parent = ln;
    if (rn->right) rn->right->parent = rn;

    return ins;
}

ttnode_t*
TwoThreeTree::NewNode(uint32_t x,uint32_t y)
{
    ttnode_t* tn = (ttnode_t*) malloc(sizeof(ttnode_t));
    if (y<x) {
        tn->data[1] = x;
        tn->data[0] = y;
    } else {
        tn->data[0] = x;
        tn->data[1] = y;
    }
    tn->left = tn->middle = tn->right = tn->parent = NULL;
    return tn;
}

void
TwoThreeTree::MergeNodes(ttnode_t* old,ttnode_t* tn)
{
    VERBOSE("MergeNodes( (%u),(%u)  )",old->data[0],tn->data[0]);
    if (tn->data[0] < old->data[0]) {
        tn->data[1] = old->data[0];
        tn->middle = tn->right;
        tn->right = old->right;
        if (old->right) old->right->parent = tn;
    } else {
        tn->data[1] = tn->data[0];
        tn->data[0] = old->data[0];
        tn->middle = tn->left;
        tn->left = old->left;
        if (old->left) old->left->parent = tn;
    }
    VERBOSE("MERGED( (%u,%u) )",tn->data[0],tn->data[1]);
    tn->parent = old->parent;
    if (!tn->parent) root = tn;
    else {
        if (tn->parent->left == old) tn->parent->left = tn;
        if (tn->parent->middle == old) tn->parent->middle = tn;
        if (tn->parent->right == old) tn->parent->right = tn;
    }
    free(old);
}

bool
TwoThreeTree::IsTerminal(ttnode_t* a)
{
    if (a->left == NULL && a->middle == NULL && a->right == NULL) return true;
    return false;
}

bool
TwoThreeTree::IsTwoNode(ttnode_t* a)
{
    if (!a) return false;
    if (a->data[1] == UINT32_MAX) return true;
    return false;
}

int64_t
TwoThreeTree::Min()
{
    if (!root) {
        return -1;
    }
    ttnode_t* tmp = root;
    while (tmp->left) {
        tmp = tmp->left;
    }
    return tmp->data[0];
}

int64_t
TwoThreeTree::Max()
{
    if (!root) {
        return -1;
    }
    ttnode_t* tmp = root;
    while (tmp->right) {
        tmp = tmp->right;
    }
    if (IsTwoNode(tmp)) return tmp->data[0];
    else return tmp->data[1];
}

int64_t
TwoThreeTree::Successor(uint32_t x)
{
    ttnode_t* tmp = root;
    while (tmp) {
        if (IsTwoNode(tmp)) {
            if (tmp->data[0]>x) tmp=tmp->left;
            else if (tmp->data[0]<x) tmp=tmp->right;
            else break;
        } else {
            if (tmp->data[0]>x) tmp=tmp->left;
            else if (tmp->data[0]<x && tmp->data[1]>x) tmp = tmp->middle;
            else if (tmp->data[1]<x) tmp = tmp->right;
            else break;
        }
    }

    if (tmp) {
        /* find successor */
        if (tmp->data[0] == x) {
            if (IsTerminal(tmp) && !IsTwoNode(tmp)) return tmp->data[1];

            ttnode_t* suc;
            if (tmp->middle) suc = FindMin(tmp->middle);
            else if (tmp->right) suc = FindMin(tmp->right);
            else {
                /* no right or middle child :-( */
                ttnode_t* prev = tmp;
                ttnode_t* cur = tmp->parent;
                while (cur) {
                    if (cur->left == prev) return cur->data[0];
                    if (cur->middle == prev) return cur->data[1];
                    prev = cur;
                    cur = cur->parent;
                }
                return -1; /* no successor */
            }
            return suc->data[0];
        } else {
            ttnode_t* suc;
            if (tmp->right) suc = FindMin(tmp->right);
            else {
                /* 3 node with no right child -> is this possible? */
                ttnode_t* prev = tmp;
                ttnode_t* cur = tmp->parent;
                while (cur) {
                    if (cur->left == prev) return cur->data[0];
                    if (cur->middle == prev) return cur->data[1];
                    prev = cur;
                    cur = cur->parent;
                }
                return -1; /* no successor */
            }
            return suc->data[0];
        }
    } else {
        return -1;
    }
}

