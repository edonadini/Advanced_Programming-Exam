#include "BinTree.h"
#include <iostream>


int main() {

  //------------------------------------------------------------------
  // Creating a tree (with int keys and int values) and populating it
  //------------------------------------------------------------------

  BinTree<int,int> b1{};

  b1.insert({8, 42});
  b1.insert({3, 42});
  b1.insert({10, 42});
  b1.insert({1, 42});
  b1.insert({6, 42});
  b1.insert({7, 42});
  b1.insert({14, 42});
  b1.insert({13, 42});
  b1.insert({4, 42});

  //--------------------------------
  // Printing the tree
  //--------------------------------

  std::cout << "Printing the content of the tree\n" << b1 << std::endl;

  //-------------------------------
  // Size of the tree
  //-------------------------------

  std::cout << "Size of the tree is:\t" << b1.size() << std::endl << std::endl;
  
  //--------------------------------
  // Inserting an existing key
  //--------------------------------

  b1.insert({3, 84});

  //--------------------------------
  // Balancing the tree
  //--------------------------------

  b1.balance();

  //------------------------------------------------
  // Checking the size of the tree after balance
  //------------------------------------------------

  std::cout << "Size of the tree after balance is:\t" << b1.size() << std::endl << std::endl;


  //-----------------------------------------------
  // Find function
  //-----------------------------------------------

  b1.find(7);
  b1.find(15);
  b1.find(43);

  std::cout << std::endl;

  //----------------------------------------------
  // [] operator
  //----------------------------------------------

  b1[14];
  std::cout << "\n";
  b1[16];
  std::cout << std::endl;
  
  //-------------------------------
  // Copy constructor
  //-------------------------------

  BinTree<int, int> copy_c{b1};

  std::cout << "Printing the original tree:\n" << b1 << "\n";
  std::cout << "Printing the tree created using copy ctor:\n" << copy_c << std::endl;

  
  //----------------------------------
  // Copy assignment
  //----------------------------------

  BinTree<int, int> copy_a;

  copy_a = b1;

  std::cout << "Printing the copied tree:\n" <<copy_a << std::endl;

  
  //---------------------------------
  // Move constructor
  //---------------------------------

  BinTree<int,int> move_c{std::move(copy_c)};

  std::cout << "Printing the tree created using move ctor:\n" << move_c << std::endl;

  //---------------------------------
  // Move assignment
  //---------------------------------

  BinTree<int,int> move_a;

  move_a = std::move(copy_a);

  std::cout << "Printing the tree for which we used move assignment:\n" << move_a << std::endl;


  //-----------------------------------------------
  // Clearing the tree (and calling balance on it)
  //-----------------------------------------------

  b1.clear();

  std::cout << "Size of the tree after clear is:\t" << b1.size() << std::endl;

  b1.balance();

  return 0;
  
}
