#include "TestHarness.h"

#include "BinarySearchTree.h"
#include "Treap.h"

#include <stdlib.h>


TEST(Treap , InsertFind)
{
    Treap* tr = new Treap(false);
    BinarySearchTree* bst = new BinarySearchTree(false);

    size_t n=5000;
    uint32_t* A = (uint32_t*)malloc(n*sizeof(uint32_t));
    for (size_t i=0; i<n; i++) A[i] = rand() % 5001;
    for (size_t i=0; i<n; i++) {
        tr->Insert(A[i]);
        bst->Insert(A[i]);
    }
    for (size_t i=0; i<n; i++) {
        CHECK(tr->Find(A[i]) == bst->Find(A[i]));
        CHECK(tr->Find(A[i]+1) == bst->Find(A[i]+1));
    }
    delete tr;
    delete bst;
    free(A);
}

TEST(Treap , Remove)
{
    Treap* tr = new Treap(false);
    size_t n=5000;
    uint32_t* A = (uint32_t*)malloc(n*sizeof(uint32_t));
    for (size_t i=0; i<n; i++) A[i] = rand() % 5001;

    for (size_t i=0; i<n; i++) tr->Insert(A[i]);
    for (size_t i=1; i<n; i+=2) tr->Remove(A[i]);

    for (size_t i=0; i<n; i++) {
        if (i%2==1) {
            CHECK(tr->Find(A[i]) == false);
        }
    }

    free(A);
    delete tr;
}

TEST(Treap , Min)
{
    Treap* tr = new Treap(false);

    tr->Insert(1);
    tr->Insert(5);
    tr->Insert(2);
    tr->Insert(61);
    tr->Insert(61);
    tr->Insert(24);
    tr->Insert(51);
    tr->Insert(61);
    tr->Insert(57);
    tr->Insert(32);
    tr->Insert(24);

    CHECK(tr->Min() == 1);
    tr->Remove(1);
    tr->Remove(2);
    CHECK(tr->Min() == 5);
    tr->Remove(5);
    CHECK(tr->Min() == 24);
    tr->Remove(24);
    CHECK(tr->Min() == 32);
    tr->Remove(32);
    CHECK(tr->Min() == 51);
    tr->Remove(51);
    CHECK(tr->Min() == 57);
    tr->Remove(57);
    CHECK(tr->Min() == 61);
    tr->Remove(61);
    CHECK(tr->Min() == -1);

    delete tr;
}

TEST(Treap , Max)
{
    Treap* tr = new Treap(false);

    CHECK(tr->Max() == -1);

    tr->Insert(1);
    tr->Insert(5);
    tr->Insert(2);
    tr->Insert(61);
    tr->Insert(61);
    tr->Insert(24);
    tr->Insert(51);
    tr->Insert(61);
    tr->Insert(57);
    tr->Insert(32);
    tr->Insert(24);

    CHECK(tr->Max() == 61);
    tr->Remove(61);
    tr->Remove(57);
    CHECK(tr->Max() == 51);
    tr->Remove(51);
    CHECK(tr->Max() == 32);
    tr->Remove(32);
    CHECK(tr->Max() == 24);
    tr->Remove(24);
    CHECK(tr->Max() == 5);
    tr->Remove(5);
    CHECK(tr->Max() == 2);
    tr->Remove(2);
    CHECK(tr->Max() == 1);
    tr->Remove(1);
    CHECK(tr->Max() == -1);

    delete tr;
}

TEST(Treap , Successor)
{
    Treap* tr = new Treap(false);

    CHECK(tr->Successor(5) == -1);

    tr->Insert(1);
    tr->Insert(5);
    tr->Insert(2);

    CHECK(tr->Successor(5) == -1);

    tr->Insert(61);
    tr->Insert(61);
    tr->Insert(24);
    tr->Insert(51);
    tr->Insert(61);
    tr->Insert(57);
    tr->Insert(32);
    tr->Insert(24);

    CHECK(tr->Successor(1) == 2);
    CHECK(tr->Successor(2) == 5);
    CHECK(tr->Successor(5) == 24);
    CHECK(tr->Successor(24) == 32);
    CHECK(tr->Successor(32) == 51);
    CHECK(tr->Successor(51) == 57);
    CHECK(tr->Successor(57) == 61);
    CHECK(tr->Successor(61) == -1);
    CHECK(tr->Successor(6) == -1);

    delete tr;
}
