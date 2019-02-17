template <typename T, typename V>
void BinTree<T,V>::clear(){
	Node* n = root.get();
	rclear(n);
	root.reset();																					
}

//--------------------------------------------------------------------- RCLEAR ----------------------------------------------


template <typename T, typename V>
void BinTree<T,V>::rclear(Node* n){																//recursive function, to clear the content of the tree
	if (n->left != nullptr)
		rclear(n->left);
	if (n->right != nullptr)
		rclear(n->right);		
 	n.reset(nullptr);																								// == n.reset() ???
}																																									
