
all:	clean oset test

oset:
	g++ -g -I ./include -o oset -Wall src/main.cpp ./src/TwoThreeTree.cpp src/Treap.cpp src/SkipList.cpp src/LinkedList.cpp src/BinarySearchTree.cpp -lm -lrt

test:
	make -C tests

clean:
	make -C tests clean
	rm -f oset 
