INCLUDES := -I ../include -I ./CppUnitLite
LIBS := -lm -lrt
COMMON := ./CppUnitLite/*.cpp test-main.cpp
CFLAGS := -Wall -g

all: clean twothree treap skiplist linkedlist bst run

twothree:
	g++ ${CFLAGS} ${INCLUDES} -o TwoThreeTest ${COMMON} ../src/TwoThreeTree.cpp ../src/LinkedList.cpp ./TwoThreeTest.cpp

treap:
	g++ ${CFLAGS} ${INCLUDES} -o TreapTest ${COMMON} ../src/Treap.cpp ../src/BinarySearchTree.cpp ./TreapTest.cpp

skiplist:
	g++ ${CFLAGS} ${INCLUDES} -o SkipListTest ${COMMON} ../src/SkipList.cpp ./SkipListTest.cpp

linkedlist:
	g++ ${CFLAGS} ${INCLUDES} -o LinkedListTest ${COMMON} ../src/LinkedList.cpp ./LinkedListTest.cpp

bst:
	g++ ${CFLAGS} ${INCLUDES} -o BSTTest ${COMMON} ../src/BinarySearchTree.cpp ../src/LinkedList.cpp ./BSTTest.cpp

clean:
	rm -f TreapTest SkipListTest LinkedListTest BSTTest TwoThreeTest

run:
	./LinkedListTest
	./TwoThreeTest
	./BSTTest
	./TreapTest
	./SkipListTest
