#include "TestHarness.h"

#include "BinarySearchTree.h"
#include "LinkedList.h"

#include <stdlib.h>


TEST(BST , InsertFind)
{
    LinkedList* list = new LinkedList(false);
    BinarySearchTree* bst = new BinarySearchTree(false);

    size_t n=5000;
    uint32_t* A = (uint32_t*)malloc(n*sizeof(uint32_t));
    for (size_t i=0; i<n; i++) A[i] = rand() % 5001;
    for (size_t i=0; i<n; i++) {
        list->Insert(A[i]);
        bst->Insert(A[i]);
    }
    for (size_t i=0; i<n; i++) {
        CHECK(list->Find(A[i]) == bst->Find(A[i]));
        CHECK(list->Find(A[i]+1) == bst->Find(A[i]+1));
    }
    delete list;
    delete bst;
    free(A);
}

TEST(BST , Remove)
{
    BinarySearchTree* bst = new BinarySearchTree(false);
    size_t n=5000;
    uint32_t* A = (uint32_t*)malloc(n*sizeof(uint32_t));
    for (size_t i=0; i<n; i++) A[i] = rand() % 5001;

    for (size_t i=0; i<n; i++) {
        bst->Insert(A[i]);
    }

    for (size_t i=1; i<n; i+=2) {
        bst->Remove(A[i]);
    }

    for (size_t i=0; i<n; i++) {
        if (i%2==1) {
            CHECK(bst->Find(A[i]) == false);
        }
    }

    free(A);
    delete bst;
}

TEST(BST , RemoveRoot)
{
    BinarySearchTree* bst = new BinarySearchTree(false);

    bst->Insert(5);
    bst->Insert(6);
    bst->Insert(2);
    bst->Insert(7);
    bst->Insert(3);

    bst->Remove(5);

    CHECK(bst->Find(5) == false);
    CHECK(bst->Find(6) == true);
    CHECK(bst->Find(2) == true);
    CHECK(bst->Find(7) == true);
    CHECK(bst->Find(3) == true);

    delete bst;
}

TEST(BST , Min)
{
    BinarySearchTree* bst = new BinarySearchTree(false);

    bst->Insert(1);
    bst->Insert(5);
    bst->Insert(2);
    bst->Insert(61);
    bst->Insert(61);
    bst->Insert(24);
    bst->Insert(51);
    bst->Insert(61);
    bst->Insert(57);
    bst->Insert(32);
    bst->Insert(24);

    CHECK(bst->Min() == 1);
    bst->Remove(1);
    bst->Remove(2);
    CHECK(bst->Min() == 5);
    bst->Remove(5);
    CHECK(bst->Min() == 24);
    bst->Remove(24);
    CHECK(bst->Min() == 32);
    bst->Remove(32);
    CHECK(bst->Min() == 51);
    bst->Remove(51);
    CHECK(bst->Min() == 57);
    bst->Remove(57);
    CHECK(bst->Min() == 61);
    bst->Remove(61);
    CHECK(bst->Min() == -1);

    delete bst;
}

TEST(BST , Max)
{
    BinarySearchTree* bst = new BinarySearchTree(false);

    CHECK(bst->Max() == -1);

    bst->Insert(1);
    bst->Insert(5);
    bst->Insert(2);
    bst->Insert(61);
    bst->Insert(61);
    bst->Insert(24);
    bst->Insert(51);
    bst->Insert(61);
    bst->Insert(57);
    bst->Insert(32);
    bst->Insert(24);

    CHECK(bst->Max() == 61);
    bst->Remove(61);
    bst->Remove(57);
    CHECK(bst->Max() == 51);
    bst->Remove(51);
    CHECK(bst->Max() == 32);
    bst->Remove(32);
    CHECK(bst->Max() == 24);
    bst->Remove(24);
    CHECK(bst->Max() == 5);
    bst->Remove(5);
    CHECK(bst->Max() == 2);
    bst->Remove(2);
    CHECK(bst->Max() == 1);
    bst->Remove(1);
    CHECK(bst->Max() == -1);

    delete bst;
}

TEST(BST , Successor)
{
    BinarySearchTree* bst = new BinarySearchTree(false);

    CHECK(bst->Successor(5) == -1);

    bst->Insert(1);
    bst->Insert(5);
    bst->Insert(2);

    CHECK(bst->Successor(5) == -1);

    bst->Insert(61);
    bst->Insert(61);
    bst->Insert(24);
    bst->Insert(51);
    bst->Insert(61);
    bst->Insert(57);
    bst->Insert(32);
    bst->Insert(24);

    CHECK(bst->Successor(1) == 2);
    CHECK(bst->Successor(2) == 5);
    CHECK(bst->Successor(5) == 24);
    CHECK(bst->Successor(24) == 32);
    CHECK(bst->Successor(32) == 51);
    CHECK(bst->Successor(51) == 57);
    CHECK(bst->Successor(57) == 61);
    CHECK(bst->Successor(61) == -1);
    CHECK(bst->Successor(6) == -1);
}
