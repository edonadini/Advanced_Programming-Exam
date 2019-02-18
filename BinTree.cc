#include <iostream>
#include <memory>		//std::unique_ptr
#include <utility>	//std::pair
#include <vector>		//std::vector
#include <cmath>		//floor()

//---------------------------------------------------------------------------- CLASS BINTREE ---------------------------------------------------------------------------


template <typename T, typename V>			// The tree must be templated on the type of the key and the type of the value associated with it
class BinTree {
	struct Node {												// The node has two pointers (to the left and right child); if those pointers are pointing to nothing they are pointing to nullptr
		std::pair<const T, V> pair; 			// key-value pair
    std::unique_ptr<Node> left;				// unique pointer to left child
		std::unique_ptr<Node> right;			// unique pointer to right child
		Node* upper;											// raw pointer to upper node, not mandatory but useful to overload operator++ for Iterators
		
		Node(std::pair<const T,V>& p, Node* l, Node* r, Node* u)		// constructor [const on value or not? why reference?	why not <pair>?]
		: 			
    pair{p},
		left{l},
		right{r},
		upper{u} {
		std::cout << "Node ctor" << std::endl;																			// to check
		}
      
    ~Node() = default;																													
    
	};

	std::unique_ptr<Node> root;																										// unique pointer to the root node, nullptr default?


	public:

		BinTree() = default;																												// default constructor

		BinTree(const BinTree& bst);																								// copy constructor (implemented outside)																				

  	  BinTree& operator=(const BinTree& bst);																			// copy assignment (implemented outside)
  
  	BinTree(BinTree&& bst) noexcept : root{std::move(bst.root)} {								// move constructor
		std::cout <<"calling move ctor" <<std::endl;
		}	

  	BinTree& operator=(BinTree&& bst) noexcept {																// move assignment
		std::cout << "calling move assignment" <<std::endl;    
		root = std::move(bst.root);
		return *this;
		}

		void insert (std::pair<const T,V>& pair);																		// inserisce il primo nodo; NON const method

		void insert_more (std::pair<const T,V>& pair, Node* comp);									// inserisce i nodi successivi (private?); NON const method

		void clear () noexcept;																											// NON const method

		class Iterator;

		class ConstIterator;

		Iterator begin();																													// return an iterator to the first node (with smallest key; in general, !=root), impl. outside

		Iterator end() { return Iterator{nullptr}; };															// return a proper iterator

		ConstIterator cbegin() const;																							// return a const iterator to the first node; implemented outside the class

		ConstIterator cend() const { return ConstIterator{nullptr}; };						// return a proper const iterator
		
		Iterator find (const T& key);																							// find a given key and return an iterator to that node. If the key is not found returns end()
		
		Iterator recfind (Node* subroot, const T& key);														// private?

		void balance ();

		void balance (std::vector<std::pair<const T, V>>& aux, int left, int right);

		void deepcopy (Node* before, std::unique_ptr<Node> after);								// to implement copy ctor									

		template <typename ot, typename ov>
		friend std::ostream& operator<<(std::ostream&, const BinTree<ot,ov>&);		//friendship

	};

		// const to mark a method that doesn't alter the object on which we call it (es. find è const, clear no!)

//----------------------------------------------------------------------- CLASS ITERATOR  ---------------------------------------------------------------------------------


template <typename T, typename V>
class BinTree<T,V>::Iterator : public std::iterator<std::forward_iterator_tag, std::pair<const T, V>> {								// class Iterator for the tree
	using Node = BinTree<T,V>::Node;																																										// kind of an "alias"

	Node* current;

	public:

	Iterator (Node* n) : current{n} {}																					// constructor
	
	std::pair<const T, V>& operator*() const { return current->pair; }					// overloading of operator *, why reference? why const?
	
	Iterator& operator++() {																										// overloading of operator ++ (preincrement)
		if (current->right != nullptr){																						// if I can go right, I go right (cerco il mio successivo, che è alla mia destra)
			current = current->right.get();
				while (current->left != nullptr) 																			// if I can go left, I go left
					current = current->left.get();																			// (tra quelli a destra è il più piccolo, quindi vado a sinistra il più possibile)
			}
		else {																																		// there is nothing on my right, so I have to go up
			const T& key = current->pair.first;																			// key è la chiave del nodo da cui voglio risalire
			while (current != nullptr && key >= current->pair.first)								// se finisco in nullptr mi fermo; oppure mi fermo se la nuova chiave è più grande di me  
				current = current->upper;																							// I go up (il primo confronto è con me stesso quindi salgo sempre almeno di uno)
			}
		return *this;
	}

  Iterator operator++(int) {																									// post-increment
    Iterator it{*this};
    ++(*this);
    return it;
  }

	bool operator==(const Iterator& other) { return current == other.current; }					// comparison operator; if the two iterators point to the same node
	bool operator!=(const Iterator& other) { return !(*this == other); }

};																																										// end class Iterators



//---------------------------------------------------------------------- CLASS CONST_ITERATOR -----------------------------------------------------------------------------


template <typename T, typename V>
class BinTree<T,V>::ConstIterator : public BinTree<T,V>::Iterator {									// inheritance (a ConstIterator IS an Iterator)

	public:
		using parent = BinTree<T,V>::Iterator;																					// using an alias
		using parent::Iterator;																													// same constructor

		const std::pair<const T, V>& operator*() const { return parent::operator*(); }	// overloading of operator*

};

//-------------------------------------------------------------------- INSERT ---------------------------------------------------------------------------------------------

template <typename T, typename V>																		
void BinTree<T,V>::insert(std::pair<const T,V>& p) {											// p è la nuova coppia che voglio inserire. reference?
	if (root == nullptr) {																									// if the tree is empty
		root.reset(new Node{p, nullptr, nullptr, nullptr});										// when inserted, a node has no children, that's why nullptr; the root node has no upper node
	}
	else
		insert_more(p, root.get());																						// if the tree is NOT empty
}					

template <typename T, typename V>		
void BinTree<T,V>::insert_more(std::pair<const T,V>& p, Node* comp){			
																																	
	if (p.first < comp->pair.first){																				// compare the new key with the key of the node pointed by "comp"; if the key is smaller...
		if (comp->left==nullptr){																							// go left, and if there isn't a left child...
			comp->left.reset(new Node{p, nullptr, nullptr, comp});							// ... insert the new node here
			return;																															// return to exit the function
			}	
		else comp=comp->left.get();																						// aggiorno il node con cui effettuare il confronto
		}

	else if (p.first == comp->pair.first){																	// if the two keys are equal
		comp->pair.second=p.second;																						// sovrascrivo il valore
		return;																																// return to exit the function
		}

	else if (p.first > comp->pair.first){																		// just "else"?
		if (comp->right==nullptr){		
			comp->right.reset(new Node{p, nullptr, nullptr, comp});
			return;																															// return to exit the function
		}
		else comp=comp->right.get();																					// aggiorno il node con cui effettuare il confronto
	}
	insert_more(p, comp);																										// in case of no insertion, I keep comparing; recursive call to insert_more
}


//--------------------------------------------------------------------- CLEAR ()--------------------------------------------------------------------------------------------

template <typename T, typename V>
void BinTree<T,V>::clear() noexcept {															
 	root.reset(nullptr);																										//root.reset(nullptr)?
	std::cout <<"inside clear" <<std::endl;
}

//-------------------------------------------------------------------------- FIND -----------------------------------------------------------------------------------------

template <typename T, typename V>
typename BinTree<T,V>::Iterator BinTree<T,V>::find(const T& key){											
	if (root == nullptr)
		return end();
	return recfind(root.get(), key);																										// no else?																		
} 

template <typename T, typename V>
typename BinTree<T,V>::Iterator BinTree<T,V>::recfind(Node* subroot, const T& key){											
    																																									
	if (key == subroot->pair.first) {																										// Base case: key is present at subroot 																 
		std::cout <<"key found" <<std::endl;		
		return Iterator{subroot};
	}																																										// Construisco un iteratore
   																																								
	if (key > subroot->pair.first){																											// Key is greater than root's key 
		if (subroot->right !=nullptr) 																										
    	return recfind(subroot->right.get(), key); 
  	else {std::cout <<"key not found R" <<std::endl; return end();}
	}

	//if (key < subroot->pair.first){																											// Key is smaller than root's key
		if (subroot->left !=nullptr)																											
  		return recfind(subroot->left.get(), key);																									
		else {std::cout <<"key not found L" <<std::endl; return end();}
	//}
}

//------------------------------------------------------------------------ << OPERATOR --------------------------------------------------------------------------------

template <typename T, typename V> 
std::ostream& operator<<(std::ostream& os, const BinTree<T,V>& bst) {						// overloading of operator <<, to print in order the tree
	for(const auto& x : bst )																											// range for loop
		os << x.first <<": " <<x.second << std::endl;
	return os;
}

//--------------------------------------------------------------------------BEGIN-----------------------------------------------------------------------------------------

template <typename T, typename V>
typename BinTree<T,V>::Iterator BinTree<T,V>::begin(){													// typename and fully qualified name, returns an iterator to the first node (leftmost)
	auto tmp = root.get();
  while (tmp->left != nullptr){
    tmp = tmp->left.get();
	}
	return Iterator{tmp};																													// costruisco un iteratore
}


//--------------------------------------------------------------------------CBEGIN-----------------------------------------------------------------------------------------

template <typename T, typename V>
typename BinTree<T,V>::ConstIterator BinTree<T,V>::cbegin() const {										// fully qualified name
	if (root=nullptr)
		return ConstIterator {nullptr};																										// necessario caso empty tree? no?
	auto tmp = root.get();
  while (tmp->left != nullptr){
    tmp = tmp->left.get();
	}
	return ConstIterator{tmp};																													// costruisco un constiterator
}


//--------------------------------------------------------------------------- BALANCE -------------------------------------------------------------------------------------

template <typename T, typename V>
void BinTree<T,V>::balance(){																							// not in place balance

	if (!root){																															// if the tree is empty...
		std::cout <<"You're trying to balance an empty tree" <<std::endl;
		return;																																// ... exit from the function
	}																																				// if (iter == end) return;
  std::vector<std::pair<const T, V>> aux;																	// saving the nodes of the tree, in order, in an std::vector; "aux" is an auxiliary vector, ref?
	Iterator iter {begin()};																								// we use iterators
	for(; iter != nullptr; ++iter)
		aux.push_back(*iter);
	clear();																																// clear the content of the tree (already copied into the vector), altrimenti	rimpiazzo solo chiavi
	balance(aux, 0, aux.size() - 1);																				// chiamo la funzione (privata) balance
}


template <typename T, typename V>
void BinTree<T,V>::balance(std::vector<std::pair<const T, V>>& aux, int left, int right){
	
	int median;

	if(left > right) return;																																		
		median = floor((left + right)/2);																			// trovo l'indice mediano; è necessaria funzione floor/ceil?
		insert(aux[median]);																									// std::pair<const T,V> p = aux[median];    
		std::cout <<"in func balance\t" <<aux[median].first <<std::endl;			// inserisco nell'albero un nodo la coppia corrispondente all'indice mediano
    balance(aux, left, median - 1);																				// ricorsivo a sinistra
    balance(aux, median + 1, right);			 																// ricorsivo a destra
 
}



//-------------------------------------------------------------------------- COPY CTOR -----------------------------------------------------------------------------------

template <typename T, typename V>
BinTree<T,V>::BinTree(const BinTree& bst) {
std::cout << "calling copy ctor" <<std::endl;																		// check
if (bst.root!=nullptr){																													// the tree to be copied is not empty
	std::unique_ptr<Node> after;
	deepcopy (bst.root.get(), after);
	}
}


template <typename T, typename V>
void BinTree<T,V>::deepcopy (Node* before, std::unique_ptr<Node> after){						// auxiliary function deepcopy to implement a copy ctor
after.reset(new Node{before->pair, before->left, before->right, before->upper});
if (before->left != nullptr)																												// left-hand recursion
	deepcopy(before->left.get(), after->left);																				
if (before.right != nullptr)																												// right-hand recursion		
	deepcopy(before->left.get(), after->left);
}

//-------------------------------------------------------------------------- COPY ASSIGNMENT ------------------------------------------------------------------------------

template <typename T, typename V>																								// difference with copy ctor: the tree already exists
BinTree<T,V>& BinTree<T,V>::operator=(const BinTree& bst){											// l'oggetto bst è l'albero che voglio copiare, doing an hand reset (design choice)
	std::cout <<"calling copy assignment" <<std::endl;														// check
	root.reset();																																	// first of all clean my memory
	auto tmp = bst;																																// then use copy ctor
	(*this) = std::move(tmp);																											// finally move assignment
	return *this;																																	// dereferenzio per aver una referenza all'albero
}

//----------------------------------------------------------------------------MAIN-----------------------------------------------------------------------------------------


int main (){
	BinTree<int,int> bst{};																									// const?
	
	std::pair<const int, int>	pair1{8,45};																			
 	std::pair<const int, int>	pair2{3,45};																			
 	std::pair<const int, int>	pair3{10,45};																			
 	std::pair<const int, int>	pair4{1,45};																			
 	std::pair<const int, int>	pair5{6,45};																			
 	std::pair<const int, int>	pair6{3,45};																			
 	std::pair<const int, int>	pair7{7,45};																			
 	std::pair<const int, int>	pair8{14,45};																			
 	std::pair<const int, int>	pair9{13,45};	
	std::pair<const int, int>	pair10{4,45};																		
 	

	bst.insert(pair1);
	bst.insert(pair2);
  bst.insert(pair3);
	bst.insert(pair4);
	bst.insert(pair5);
	bst.insert(pair6);
	bst.insert(pair7);
	bst.insert(pair8);
	bst.insert(pair9);	
	bst.insert(pair10);																											

	for (const auto& x : bst) {
		std::cout << "(" << x.first << ":" << x.second << ") ";
	}
	std::cout << std::endl;
	
	bst.find(7);
	bst.find(16);
	bst.find(9);

	//bst.clear();
																																						
	//bst.balance();

	BinTree<int,int> moved{std::move(bst)};
		
	for (const auto& x : moved) {																															// se stampo bst dà segmentation fault, perché è stato spostato
		std::cout << "(" << x.first << ":" << x.second << ") ";
	}
	std::cout << "moved" << std::endl;

	BinTree<int,int> copied{bst};			

	for (const auto& x : copied) {																													
		//std::cout << "(" << x.first << ":" << x.second << ") ";
	//}
	//std::cout << "copied" << std::endl;						

	BinTree<int,int> b3;
	
	//b3 = bst;

	//for (const auto& x : b3) {																														
	//	std::cout << "(" << x.first << ":" << x.second << ") ";	
	//}
	//std::cout << "b3" << std::endl;						

	return 0;
}

