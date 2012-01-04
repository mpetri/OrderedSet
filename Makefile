UNAME := $(shell uname)

all:	clean oset test

oset:
	g++ -g -I ./include -o oset -Wall src/main.cpp ./src/TwoThreeTree.cpp src/Treap.cpp src/SkipList.cpp src/LinkedList.cpp src/BinarySearchTree.cpp -lm -lrt

test:
ifeq ($(UNAME),SunOS)
	gmake -C tests
else
	make -C tests
endif

clean:
ifeq ($(UNAME),SunOS)
	gmake -C tests clean
else
	make -C tests clean
endif
	rm -f oset 

