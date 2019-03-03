#include <iostream>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>

//----------------------------------------------------------------------------------------------------------
// CLASS BINTREE
// Implementation of the class BinTree
// The tree must be templated on the type of the key (T) and the type of the value associated with it (V).
// Within our class, we implement a structure Node.
// Each node must have two pointers, to the left and right child respectively; we use unique pointers.
// We also have a raw pointer to the upper node.
// Among the private implementation details of the class, we have a unique pointer to the root node and
// we declare some (recursive) functions that are called through the public corresponding methods.
// In the public interface, we have copy and move semantics, the classes Iterator and ConstIterator, and
// several methods (in most of the cases, implemented outside the BinTree class).
//----------------------------------------------------------------------------------------------------------

template <typename T, typename V>
class BinTree {
  struct Node {
    std::pair<const T, V> pair;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    Node* upper;

    Node(const std::pair<const T, V>& p, Node* l, Node* r,Node *u) :
      pair{p},
      left{l},
      right{r},
      upper{u} {
      //std::cout << "Calling node ctor" << std::endl;
    }

    ~Node() noexcept = default;
  };

  std::unique_ptr<Node> root;
 
  void insert_more(const std::pair<const T, V>& pair, Node* comp) noexcept;
  
  Node* recfind(Node* subroot, const T& key) noexcept;

  void balance(std::vector<std::pair<const T, V>>& aux, std::size_t left, std::size_t right) noexcept;

  void deepcopy(Node*before, std::unique_ptr<Node>& after) noexcept;

  int size(Node* subroot) const noexcept;

  
public:

  // default constructor
  BinTree() = default;

  // copy constructor (implemented ouside)
  BinTree(const BinTree& bst);

  // copy assignment (implement outside)
  BinTree& operator=(const BinTree& bst);

  // move constructor
  BinTree(BinTree&& bst) noexcept : root{std::move(bst.root)} {
    std::cout << "Calling move ctor" << std::endl;
  }

  // move assignment
  BinTree& operator=(BinTree&& bst) noexcept {
    std::cout << "Calling move assignment" << std::endl;
    root = std::move(bst.root);
    return *this;
  }

  ~BinTree() noexcept = default;

  void insert(const std::pair<const T, V>& pair) noexcept;

  void clear() noexcept;

  class Iterator;

  class ConstIterator;

  Iterator begin();

  Iterator end() { return Iterator{nullptr}; };
  
  ConstIterator begin() const;

  ConstIterator end() const { return ConstIterator{nullptr}; };
  
  ConstIterator cbegin() const; 

  ConstIterator cend() const { return ConstIterator{nullptr}; };

  Iterator find(const T& key) noexcept;

  ConstIterator find(const T& key) const noexcept;

  V& operator[](const T& key) noexcept;

  const V& operator[](const T& key) const;
    
  void balance() noexcept;

  int size() const noexcept { return size(root.get()); };

};


//--------------------------------------------------------------------------------
// CLASS ITERATOR
// Implementing the class Iterator for the BinTree
//---------------------------------------------------------------------------------

template <typename T, typename V>
class BinTree<T, V>::Iterator : public std::iterator<std::forward_iterator_tag,std::pair<const T, V>> {

  // defining kind of an alias
  using Node = BinTree<T, V>::Node;                

  Node* current;

public:

  Iterator(Node* n) : current{n} {}

  // overloading of operator*
  std::pair<const T, V>& operator*() const { return current->pair; }

  // overloading of operator++ (pre-increment)
  // if we can go right, we go right, then we go left as much as possible;
  // if there's nothing on the right, we keep going up, until either we find a bigger key or we end up in nullptr
  Iterator& operator++() {
    if (current->right != nullptr) { 
      current = current->right.get();
      while (current->left != nullptr)
        current = current->left.get();
    } else {
      const T& key = current->pair.first;
      // the first comparison is with the key itself, so we always go up at least one step
      while (current != nullptr && key >= current->pair.first)
        current = current->upper;
    }
    return *this;
  }

  Iterator operator++(int) {
    Iterator it{*this};
    ++(*this);
    return it;
  }

  // overloading of comparison operator
  bool operator==(const Iterator& other) { return current == other.current; }
  bool operator!=(const Iterator& other) { return !(*this == other); }

};


//-----------------------------------------------------------------------------
// CLASS CONST_ITERATOR
//-----------------------------------------------------------------------------

template <typename T, typename V>
class BinTree<T, V>::ConstIterator : public BinTree<T, V>::Iterator {

public:
  // defining an alias
  using parent = BinTree<T, V>::Iterator;
  // same constructor of Iterator class
  using parent::Iterator;

  const std::pair<const T, V>& operator*() const { return parent::operator*(); }
};


//----------------------------------------------------------------------------------------------
// INSERT
// The insert method inserts the first node (root node) if the tree is empty;
// otherwise, it calls insert_more private method which is a recursive one.
// Of course, these are not const methods, since they alter the object on which they are called.
//----------------------------------------------------------------------------------------------

template <typename T, typename V>
void BinTree<T, V>::insert(const std::pair<const T, V>& p) noexcept {
  if (root == nullptr) {
    root.reset(new Node{p, nullptr, nullptr, nullptr});
  } else
    insert_more(p, root.get());
}

template <typename T, typename V>
void BinTree<T, V>::insert_more(const std::pair<const T, V>& p, Node* comp) noexcept {

  if (p.first < comp->pair.first) {
    if (comp->left == nullptr) {
      comp->left.reset(new Node{p, nullptr, nullptr, comp});
      return;                                   
    } else
      // updating the node used for comparison
      comp = comp->left.get();
  }

  // if the two keys are equal, our choice is to overwrite the value
  else if (p.first == comp->pair.first) {
    comp->pair.second = p.second;
    return;
  }

  else if (p.first > comp->pair.first) {
    if (comp->right == nullptr) {
      comp->right.reset(new Node{p, nullptr, nullptr, comp});
      return;
    } else
      comp = comp->right.get();
  }
  // in case of no insertion, we keep comparing:
  // recursive call to insert_more
  insert_more(p, comp);
}

//--------------------------------------------------------------------------------------
// CLEAR
// The implementation of this method is very simple (there's no need for recusion),
// since we can exploit the features of unique pointers.
// Of course, this is not a const method.
//---------------------------------------------------------------------------------------

template <typename T, typename V> void BinTree<T, V>::clear() noexcept {
  root.reset();
  std::cout << "Inside func clear" << std::endl;
}

//--------------------------------------------------------------------------------------------
// FIND
// We implemented find method in two versions: non-const and const.
// They are used to find a given key and return an iterator (or constiterator) to that node.
// If the tree is not empty, they both call the recursive private method recfind.
//--------------------------------------------------------------------------------------------

template <typename T, typename V>
typename BinTree<T, V>::Iterator BinTree<T, V>::find(const T& key) noexcept{
  if (root == nullptr)
    return end();
  return Iterator{recfind(root.get(), key)};
}

template <typename T, typename V>
typename BinTree<T, V>::ConstIterator BinTree<T, V>::find(const T& key) const noexcept{
  if (root == nullptr)
    return end();
  return ConstIterator{recfind(root.get(), key)};
}

template <typename T, typename V>
typename BinTree<T, V>::Node* BinTree<T, V>::recfind(Node* subroot, const T& key) noexcept {

  // base case: key is found at subroot
  if (key == subroot->pair.first) {
    std::cout << "From func find: key " << key << " found" << std::endl;
    return subroot;
  }

  if (key > subroot->pair.first) {
    if (subroot->right != nullptr)
      return recfind(subroot->right.get(), key);
    else {
      std::cout << "From func find: key " << key << " not found" << std::endl;
      return nullptr;
    }
  }

  if (subroot->left != nullptr)
    return recfind(subroot->left.get(), key);
  else {
    std::cout << "From func find: key " << key << " not found" << std::endl;
    return nullptr;
  }
}

//------------------------------------------------------------------------------------
// << OPERATOR
// Overloading of operator<<, in order to print the tree in order
//------------------------------------------------------------------------------------

template <typename T, typename V>
std::ostream& operator<<(std::ostream& os, BinTree<T, V>& bst) {
  for (const auto &x : bst)             
    os << x.first << ": " << x.second << std::endl;
  return os;
}

//------------------------------------------------------------------------------------------------------
// BEGIN
// This function must return an Iterator to the first node (i.e. with smallest key; in general, != root)
//------------------------------------------------------------------------------------------------------

template <typename T, typename V>
typename BinTree<T, V>::Iterator BinTree<T, V>::begin() {
  auto tmp = root.get();
  while (tmp->left != nullptr) {
    tmp = tmp->left.get();
  }
  return Iterator{tmp};
}

//-------------------------------------------------------------------------------------
// CBEGIN
// This function must return a ConstIterator to the first node 
//-------------------------------------------------------------------------------------

template <typename T, typename V>
typename BinTree<T, V>::ConstIterator BinTree<T, V>::cbegin() const {
  if (root == nullptr)
    return ConstIterator{nullptr};
  auto tmp = root.get();
  while (tmp->left != nullptr) {
    tmp = tmp->left.get();
  }
  return ConstIterator{tmp};
}


//----------------------------------------------------------------------------------------------
// OPERATOR []
// Overloading of operator[], in both non-const and const versions.
// These functions must return a reference to the value associated to the key k;
// if the key is not present, a new node with key k is allocated having the value value_type{}.
//----------------------------------------------------------------------------------------------

template <typename T, typename V>
V& BinTree<T, V>::operator[](const T& key) noexcept{
  Iterator get_iter {find(key)};
  if (get_iter != nullptr){
    std::cout << "Value corresponding to key " << key <<" is: " << (*get_iter).second << std::endl;
    return (*get_iter).second;
  }
  std::cout << "Key not found: allocating a new node with key " << key <<" and value {}" << std::endl;
  insert ({key, V{}});
  get_iter = find(key);
  std::cout << "Value corresponding to key " << key <<" is: " << (*get_iter).second << std::endl;
  return (*get_iter).second;
}

template <typename T, typename V>
const V& BinTree<T, V>::operator[](const T& key) const{
  ConstIterator get_iter {find(key)};
  if (get_iter != nullptr)
    return (*get_iter).second;
  throw
    std::runtime_error("Key not found");
}



//----------------------------------------------------------------------------------------------
// BALANCE
// This method performs a non-in-place balancing of the tree.
// If the tree is not empty, the nodes of the tree are stored, in order, in an auxiliary vector;
// then the tree is cleared and the private method balance is called to rebuild the tree in a
// balanced way (this function calls itself recursively on the two halves of the std::vector)
//----------------------------------------------------------------------------------------------

template <typename T, typename V>
void BinTree<T, V>::balance() noexcept {

  if (!root) {
    std::cout << "You're trying to balance an empty tree" << std::endl;
    return;
  }
  std::vector<std::pair<const T, V>> aux;
  Iterator iter{begin()};
  for (; iter != nullptr; ++iter)
    aux.push_back(*iter);
  clear();
  balance(aux, 0, aux.size());
}

template <typename T, typename V>
void BinTree<T, V>::balance(std::vector<std::pair<const T, V>>& aux, std::size_t left, std::size_t right) noexcept {

  if ( right - left < 1)
    return;
  auto median = ((left + right) / 2);
  insert(aux[median]);
  std::cout << "From func balance:\t" << aux[median].first << std::endl;
  balance(aux, left, median);
  balance(aux, median + 1, right);
}

//-------------------------------------------------------------------------------------
// COPY CTOR
// After constructing the root node of the tree, we call the private function deepcopy,
// which performs a deep copy of the original tree using recursion.
//-------------------------------------------------------------------------------------

template <typename T, typename V> BinTree<T, V>::BinTree(const BinTree& bst) {
  std::cout << "Calling copy ctor" << std::endl;
  if (bst.root != nullptr) {
    root.reset(new Node{bst.root->pair, nullptr, nullptr, nullptr});
    deepcopy(bst.root.get(), root);
  }
}

template <typename T, typename V>
void BinTree<T, V>::deepcopy(Node* before, std::unique_ptr<Node>& after) noexcept {
  if (before->left != nullptr) {
    after->left.reset(new Node{before->left->pair, nullptr, nullptr, after.get()});
    deepcopy(before->left.get(), after->left);
  }
  if (before->right != nullptr) {
    after->right.reset(new Node{before->right->pair, nullptr, nullptr, after.get()});
    deepcopy(before->right.get(), after->right);
  }
}

//-----------------------------------------------------------------------------
// COPY ASSIGNMENT
//------------------------------------------------------------------------------

template <typename T, typename V>
BinTree<T, V>& BinTree<T, V>::operator=(const BinTree& bst) {
  std::cout << "Calling copy assignment" << std::endl;
  root.reset();
  // using copy ctor
  auto tmp = bst;
  // using move assignment
  (*this) = std::move(tmp);
  // dereferencing, since it must return a reference to the tree
  return *this;
}


//---------------------------------------------------------------------------
// SIZE
// This private recursive function is called by the public method size,
// in order to determine the size of the tree.
//---------------------------------------------------------------------------

template <typename T, typename V>
int BinTree<T, V>::size(Node* subroot) const noexcept { 
  if (subroot == nullptr)
    return 0; 
  else { 
    return size(subroot->left.get()) + 1 + size(subroot->right.get()); 
  } 
} 
