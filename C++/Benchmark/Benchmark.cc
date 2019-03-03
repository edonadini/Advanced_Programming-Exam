#include <algorithm>
#include "BinTree_plain.h"
#include <chrono>
#include <iostream>
#include <map>
#include <vector>

  

int main(){

  
  BinTree<int, int> unb_tree;
  BinTree<int, int> b_tree;
  std::map<int, int> map;


  std::vector<int> v;
  for (auto i=0; i < N_NODES; ++i)
    v.push_back(i);

  std::vector<int> building_vector = v;
  std::random_shuffle (building_vector.begin(), building_vector.end());

  std::vector<int> lookup_vector = v;
  std::random_shuffle (lookup_vector.begin(), lookup_vector.end());


  /*
  for (const auto z : v)
    std::cout << z << " ";
  std::cout << std::endl;

  for (const auto z : building_vector)
    std::cout << z << " ";
  std::cout << std::endl;

  for (const auto z : lookup_vector)
    std::cout << z << " ";
  std::cout << std::endl;
  
  */

  
  //-----------------------------------------------
  // UNBALANCED TREE
  //-----------------------------------------------

  for (auto k : building_vector)
    unb_tree.insert({k, k});

  std::cout <<"Size of unbalanced tree is " << unb_tree.size() << std::endl;
  //std::cout << unb_tree << std::endl;
	
  auto start_time_unbalanced = std::chrono::high_resolution_clock::now();

  for (const auto x : lookup_vector)
    unb_tree.find(x);

  auto end_time_unbalanced = std::chrono::high_resolution_clock::now();

  auto total_time_unbalanced = std::chrono::duration_cast<std::chrono::microseconds>(end_time_unbalanced-start_time_unbalanced).count();

  std::cout<<"Lookup with unbalanced tree took on average " <<total_time_unbalanced/double(N_NODES) <<" microsec" <<std::endl;



  //------------------------------------------------------------
  // BALANCED TREE
  //------------------------------------------------------------

  b_tree = unb_tree;
  b_tree.balance();

  std::cout <<"Size of balanced tree is " << b_tree.size() << std::endl;
  //std::cout << b_tree <<std::endl;

  auto start_time_balanced = std::chrono::high_resolution_clock::now();

  for (const auto x : lookup_vector)
    b_tree.find(x);

  auto end_time_balanced = std::chrono::high_resolution_clock::now();

  auto total_time_balanced = std::chrono::duration_cast<std::chrono::microseconds>(end_time_balanced-start_time_balanced).count();

  std::cout<<"Lookup with balanced tree took on average " <<total_time_balanced/double(N_NODES) <<" microsec" <<std::endl;


  
  //--------------------------------------------------------
  // STD::MAP
  //--------------------------------------------------------

  
  for (auto k : building_vector)
    map.insert({k, k});

  auto start_time_map = std::chrono::high_resolution_clock::now();

  for (const auto x : lookup_vector)
    map.find(x);

  auto end_time_map = std::chrono::high_resolution_clock::now();

  auto total_time_map = std::chrono::duration_cast<std::chrono::microseconds>(end_time_map-start_time_map).count();

  std::cout<<"Lookup with map took on average " <<total_time_map/double(N_NODES) <<" microsec" <<std::endl;



  return 0;	
}


