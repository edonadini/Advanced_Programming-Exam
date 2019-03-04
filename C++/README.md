# C++ project

This folder contains the C++ project.

In the **BinTree.h** file we implement a **template** binary search tree (BST). A BST, is a hierarchical (ordered) data structure where each **node** can have at most two children, namely, **left** and **right** child. Each node stores a **pair** of a **key** and the associated **value**. The binary tree is ordered according to the keys. Given a node `N`, all the nodes having keys **smaller** than the key of the node `N` can be found going **left**. While all the nodes with a key **greater** than the key of the node `N` can be reached going **right**. 

The **Benchmark** folder contains the file **Benchmark.cc** use to test the lookups using the `find()`function for the std::map, and our balance and unbalance tree; and **BinTree_plain.h** which has the same content as BinTree.h, but the couts used for debugging purposes were commented in order not to affect the performance of find function.

The **Report.pdf** file presents a brief description of the implementation details of the **template** binary search tree and the benchmark results.

The **Test.cc** file is use to check the implementation of the BinTree class and his functions.

The **Make file** contains the directive to compile the the **Test.cc** file.



