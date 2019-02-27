#include <iostream>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>

//----------------------------------------------------------------------------
//CLASS BINTREE
//----------------------------------------------------------------------------

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
      //std::cout << "Node ctor" << std::endl;
    }

    ~Node() noexcept = default;
  };

  std::unique_ptr<Node> root;
 
  void insert_more(const std::pair<const T, V>& pair, Node* comp);
  
  Node* recfind(Node* subroot, const T& key) noexcept;

  void balance(std::vector<std::pair<const T, V>>& aux, std::size_t left, std::size_t right);

  void deepcopy(Node*before, std::unique_ptr<Node>& after);

  int size(Node* subroot) const noexcept;

  
public:
  
  BinTree() = default;

  BinTree(const BinTree& bst);

  BinTree& operator=(const BinTree& bst);

  BinTree(BinTree&& bst) noexcept : root{std::move(bst.root)} {
    //std::cout << "calling move ctor" << std::endl;
  }

  BinTree& operator=(BinTree&& bst) noexcept {
    //std::cout << "calling move assignment" << std::endl;
    root = std::move(bst.root);
    return *this;
  }

  void insert(const std::pair<const T, V>& pair);

  void clear() noexcept;

  class Iterator;

  class ConstIterator;

  Iterator begin();

  Iterator end() noexcept { return Iterator{nullptr}; };
  
  ConstIterator begin() const;

  ConstIterator end() const { return ConstIterator{nullptr}; };
  
  ConstIterator cbegin() const; 

  ConstIterator cend() const { return ConstIterator{nullptr}; };

  Iterator find(const T& key) noexcept;

  ConstIterator find(const T& key) const noexcept;

  V& operator[](const T& key) noexcept;

  const V& operator[](const T& key) const;
    
  void balance();

  int size() const noexcept { return size(root.get()); };

  // template <typename ot, typename ov>
  //friend std::ostream& operator<<(std::ostream&, const BinTree<ot, ov>&);
};


//--------------------------------------------------------------------------------
//CLASS ITERATOR
//---------------------------------------------------------------------------------

template <typename T, typename V>
class BinTree<T, V>::Iterator : public std::iterator<std::forward_iterator_tag,std::pair<const T, V>> {
  using Node = BinTree<T, V>::Node;                

  Node* current;

public:
  Iterator(Node* n) : current{n} {}

  std::pair<const T, V>& operator*() const { return current->pair; }

  Iterator &operator++() {
    if (current->right != nullptr) { 
      current = current->right.get();
      while (current->left != nullptr)
        current =
            current->left.get();
    } else {
      const T& key =
          current->pair.first;
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

  bool operator==(const Iterator& other) { return current == other.current; }
  bool operator!=(const Iterator& other) { return !(*this == other); }

};


//-----------------------------------------------------------------------------
//CLASS CONST_ITERATOR
//-----------------------------------------------------------------------------

template <typename T, typename V>
class BinTree<T, V>::ConstIterator : public BinTree<T, V>::Iterator {

public:
  using parent = BinTree<T, V>::Iterator;
  using parent::Iterator;

  const std::pair<const T, V>& operator*() const { return parent::operator*(); }
};


//--------------------------------------------------------------------------------
//INSERT
//--------------------------------------------------------------------------------

template <typename T, typename V>
void BinTree<T, V>::insert(const std::pair<const T, V>& p) {
  if (root == nullptr) {
    root.reset(new Node{p, nullptr, nullptr, nullptr});
  } else
    insert_more(p, root.get());
}

template <typename T, typename V>
void BinTree<T, V>::insert_more(const std::pair<const T, V>& p, Node* comp) {

  if (p.first < comp->pair.first) {
    if (comp->left == nullptr) {
      comp->left.reset(new Node{p, nullptr, nullptr, comp});
      return;                                   
    } else
      comp = comp->left.get();
  }

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
  insert_more(p, comp);
}

//--------------------------------------------------------------------------------------
//CLEAR
//---------------------------------------------------------------------------------------

template <typename T, typename V> void BinTree<T, V>::clear() noexcept {
  root.reset();
  //std::cout << "inside clear" << std::endl;
}

//----------------------------------------------------------------------------------------
//FIND
//-----------------------------------------------------------------------------------------

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

  if (key == subroot->pair.first) {
    //std::cout << "key found" << std::endl;
    return subroot;
  }

  if (key > subroot->pair.first) {
    if (subroot->right != nullptr)
      return recfind(subroot->right.get(), key);
    else {
      //std::cout << "key not found R" << std::endl;
      return nullptr;
    }
  }

  if (subroot->left != nullptr)
    return recfind(subroot->left.get(), key);
  else {
    //std::cout << "key not found L" << std::endl;
    return nullptr;
  }
}

//-------------------------------------------------------------------------------
//<< OPERATOR
//--------------------------------------------------------------------------------

template <typename T, typename V>
std::ostream& operator<<(std::ostream& os, BinTree<T, V>& bst) {
  for (const auto &x : bst)             
    os << x.first << ": " << x.second << std::endl;
  return os;
}

//--------------------------------------------------------------------------------------
//BEGIN
//--------------------------------------------------------------------------------------

template <typename T, typename V>
typename BinTree<T, V>::Iterator BinTree<T, V>::begin() {
  auto tmp = root.get();
  while (tmp->left != nullptr) {
    tmp = tmp->left.get();
  }
  return Iterator{tmp};
}

//-------------------------------------------------------------------------------------
//CBEGIN
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


//---------------------------------------------------------------------------------------
//OPERATOR []
//---------------------------------------------------------------------------------------

template <typename T, typename V>
V& BinTree<T, V>::operator[](const T& key) noexcept{
  Iterator get_iter {find(key)};
  if (get_iter != nullptr)
    return (*get_iter).second;
  //std::cout <<"Key not found: allocating a new node with key " << key <<" and value {}" << std::endl;
  insert ({key, V{}});
  get_iter = find(key);
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




//---------------------------------------------------------------------------------------
//BALANCE
//---------------------------------------------------------------------------------------

template <typename T, typename V>
void BinTree<T, V>::balance() {

  if (!root) {
    //std::cout << "You're trying to balance an empty tree" << std::endl;
    return;
  }
  std::vector<std::pair<const T, V>> aux;
  Iterator iter{begin()};
  for (; iter != nullptr; ++iter)
    aux.push_back(*iter);
  clear();
  balance(aux, 0, aux.size()); // chiamo la funzione (privata) balance
}

template <typename T, typename V>
void BinTree<T, V>::balance(std::vector<std::pair<const T, V>>& aux, std::size_t left, std::size_t right) {

  if ( right - left < 1)
    return;
  auto median = ((left + right) / 2);
  insert(aux[median]);
  //std::cout << "in func balance\t" << aux[median].first << std::endl;
  balance(aux, left, median);
  balance(aux, median + 1, right);
}

//----------------------------------------------------------------------------------
//COPY CTOR
//-----------------------------------------------------------------------------------

template <typename T, typename V> BinTree<T, V>::BinTree(const BinTree& bst) {
  //std::cout << "calling copy ctor" << std::endl;
  if (bst.root != nullptr) {
    root.reset(new Node{bst.root->pair, nullptr, nullptr, nullptr});
    deepcopy(bst.root.get(), root);
  }
}

template <typename T, typename V>
void BinTree<T, V>::deepcopy(Node* before, std::unique_ptr<Node>& after) {
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
//COPY ASSIGNMENT
//------------------------------------------------------------------------------

template <typename T, typename V>
BinTree<T, V> &BinTree<T, V>::operator=(const BinTree& bst) {
  //std::cout << "calling copy assignment" << std::endl;
  root.reset();
  auto tmp = bst;
  (*this) = std::move(tmp);
  return *this;
}


//---------------------------------------------------------------------------
//SIZE
//---------------------------------------------------------------------------

template <typename T, typename V>
int BinTree<T, V>::size(Node* subroot) const noexcept { 
  if (subroot == nullptr)
    return 0; 
  else { 
    return size(subroot->left.get()) + 1 + size(subroot->right.get()); 
  } 
} 

