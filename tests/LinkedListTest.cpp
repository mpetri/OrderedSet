#include "TestHarness.h"

#include "LinkedList.h"

#include <stdlib.h>

TEST(LinkedList , Remove)
{
    LinkedList* list = new LinkedList(false);

    list->Insert(1);
    list->Insert(5);
    list->Insert(2);
    list->Insert(61);
    list->Insert(61);
    list->Insert(24);
    list->Insert(51);
    list->Insert(61);
    list->Insert(57);
    list->Insert(32);
    list->Insert(24);

    CHECK(list->Find(1) == true);
    list->Remove(1);
    CHECK(list->Find(1) == false);
    list->Insert(1);
    CHECK(list->Find(1) == true);

    CHECK(list->Find(61) == true);
    list->Remove(61);
    CHECK(list->Find(61) == false);
    list->Remove(61);
    CHECK(list->Find(61) == false);
    list->Insert(62);
    CHECK(list->Find(61) == false);
    list->Insert(61);
    list->Remove(62);
    CHECK(list->Find(61) == true);
    CHECK(list->Find(62) == false);

    delete list;
}

TEST(LinkedList , Find)
{
    LinkedList* list = new LinkedList(false);

    list->Insert(1);
    list->Insert(5);
    list->Insert(2);
    list->Insert(61);
    list->Insert(61);
    list->Insert(24);
    list->Insert(51);
    list->Insert(61);
    list->Insert(57);
    list->Insert(32);
    list->Insert(24);

    CHECK(list->Find(1) == true);
    CHECK(list->Find(2) == true);
    CHECK(list->Find(3) == false);
    CHECK(list->Find(4) == false);
    CHECK(list->Find(5) == true);
    CHECK(list->Find(24) == true);
    CHECK(list->Find(27) == false);
    CHECK(list->Find(32) == true);
    CHECK(list->Find(33) == false);
    CHECK(list->Find(51) == true);
    CHECK(list->Find(61) == true);
    CHECK(list->Find(57) == true);

    delete list;
}

TEST(LinkedList , Insert)
{
    LinkedList* list = new LinkedList(false);

    list->Insert(1);
    list->Insert(5);
    list->Insert(2);
    list->Insert(61);
    list->Insert(61);
    list->Insert(24);
    list->Insert(51);
    list->Insert(61);
    list->Insert(57);
    list->Insert(32);
    list->Insert(24);

    CHECK(list->Items() == 8);

    llnode_t* head = list->Head();
    CHECK(head->data == 1); head=head->next;
    CHECK(head->data == 2); head=head->next;
    CHECK(head->data == 5); head=head->next;
    CHECK(head->data ==24); head=head->next;
    CHECK(head->data ==32); head=head->next;
    CHECK(head->data ==51); head=head->next;
    CHECK(head->data ==57); head=head->next;
    CHECK(head->data ==61); head=head->next;
    CHECK(head == NULL);

    delete list;
}

TEST(LinkedList , Min)
{
    LinkedList* list = new LinkedList(false);

    list->Insert(1);
    list->Insert(5);
    list->Insert(2);
    list->Insert(61);
    list->Insert(61);
    list->Insert(24);
    list->Insert(51);
    list->Insert(61);
    list->Insert(57);
    list->Insert(32);
    list->Insert(24);

    CHECK(list->Min() == 1);
    list->Remove(1);
    list->Remove(2);
    CHECK(list->Min() == 5);
    list->Remove(5);
    CHECK(list->Min() == 24);
    list->Remove(24);
    CHECK(list->Min() == 32);
    list->Remove(32);
    CHECK(list->Min() == 51);
    list->Remove(51);
    CHECK(list->Min() == 57);
    list->Remove(57);
    CHECK(list->Min() == 61);
    list->Remove(61);
    CHECK(list->Min() == -1);

    delete list;
}

TEST(LinkedList , Max)
{
    LinkedList* list = new LinkedList(false);

    CHECK(list->Max() == -1);

    list->Insert(1);
    list->Insert(5);
    list->Insert(2);
    list->Insert(61);
    list->Insert(61);
    list->Insert(24);
    list->Insert(51);
    list->Insert(61);
    list->Insert(57);
    list->Insert(32);
    list->Insert(24);

    CHECK(list->Max() == 61);
    list->Remove(61);
    list->Remove(57);
    CHECK(list->Max() == 51);
    list->Remove(51);
    CHECK(list->Max() == 32);
    list->Remove(32);
    CHECK(list->Max() == 24);
    list->Remove(24);
    CHECK(list->Max() == 5);
    list->Remove(5);
    CHECK(list->Max() == 2);
    list->Remove(2);
    CHECK(list->Max() == 1);
    list->Remove(1);
    CHECK(list->Max() == -1);

    delete list;
}

TEST(LinkedList , Successor)
{
    LinkedList* list = new LinkedList(false);

    CHECK(list->Successor(5) == -1);

    list->Insert(1);
    list->Insert(5);
    list->Insert(2);

    CHECK(list->Successor(5) == -1);

    list->Insert(61);
    list->Insert(61);
    list->Insert(24);
    list->Insert(51);
    list->Insert(61);
    list->Insert(57);
    list->Insert(32);
    list->Insert(24);

    CHECK(list->Successor(1) == 2);
    CHECK(list->Successor(2) == 5);
    CHECK(list->Successor(5) == 24);
    CHECK(list->Successor(24) == 32);
    CHECK(list->Successor(32) == 51);
    CHECK(list->Successor(51) == 57);
    CHECK(list->Successor(57) == 61);
    CHECK(list->Successor(61) == -1);
    CHECK(list->Successor(6) == -1);
}
