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
    CHECK(5 == 3);
}

TEST(LinkedList , Min)
{
    CHECK(5 == 3);
}


TEST(LinkedList , Max)
{
    CHECK(5 == 3);
}

TEST(LinkedList , Successor)
{
    CHECK(5 == 3);
}




