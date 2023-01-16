#ifndef _rbt_VALUE
#define _rbt_VALUE
#include	"RBT_main.h"
#include	"RBT_sort.h"

	/*	 GLOBAL VARIABLES	*/
		// External From 'RBT_sort.cpp'
template <typename K, typename D>
extern inline bool(*fooPtr)(rbtNode<K,D> *, rbtNode<K,D> *);

/*	============================================================================  */
/* |                                                                            | */
/* |                             HELPER FUNCTION                                | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: A private function used to reposition a single node, into the right place, in a sorted node array
 * parameters: 			A node array, the start and end indexes of the array, and the node to reposition
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::nodeReposition(rbtNode<K,D> **rbtNodes, rbtNode<K,D> **IdxS, rbtNode<K,D> **IdxE, rbtNode<K,D> *curr) {
		// If there is a future node to consider and if that node is less than current node
	if ((IdxE != IdxS + 1) && keyDataCompair(*(IdxS + 1), curr)) {
			// If so, the current node is in the wrong place. Reposition it
		while(++IdxS != IdxE && keyDataCompair(*IdxS, curr))
			*(IdxS - 1) = *IdxS;
		*(IdxS - 1) = curr;
	}
		// Else check if their is a past node to consider and if that node is greater than current node
	else if ((IdxS != rbtNodes) && keyDataCompair(curr, *(IdxS - 1))) {
			// If so, the current node is in the wrong place. Reposition it
		while(--IdxS >= rbtNodes && keyDataCompair(curr, *IdxS))
			*(IdxS + 1) = *IdxS;
		*(IdxS + 1) = curr;
	}
		// The node is now in correct position

		// Set current node array as the red-black tree and end clear it
	treeifyShallow(0, size-1, rbtNodes, &root);
	delete [] rbtNodes;
}

/*	============================================================================  */
/* |                                                                            | */
/* |                               KEY FUNCTIONS                                | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: Changes the key value of a given node
 *						Automatically repositions the node in the red-black tree
 *						Does nothing if the node is not from this tree
 * parameters: 			A node from the current tree, and a new key
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_setKey(rbtNode<K,D> *curr, K key) {
		// Initializes values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxE;

		// Gets all nodes
	IdxS = rbtNodes = rbt_getAllNodes(); IdxE = IdxS + size;

		// Cycles through all nodes of this red-black tree
	while(true) {
			// If the node is not found in this red-black tree, just return
		if (IdxS == IdxE) {
			delete [] rbtNodes;
			return;
		}
			// Else, if node is found in this red-black tree, begin repositioning
		if (*IdxS == curr)
			break;
			// Otherwise, check another node
		IdxS++;
	}

		// Sets the node's key and begins repositioning
	curr->key = key;
	nodeReposition(rbtNodes, IdxS, IdxE, curr);
}

/*
 * function_identifier: Changes the key value of a given node, according to a given function
 *						Automatically repositions the node in the red-black tree
 *						Does nothing if the node is not from this tree
 * parameters: 			A node from the current tree, and function that returns a key value
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcKey(rbtNode<K,D> *curr, K (*someFunc)()) {
		// Initializes values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxE;

		// Gets all nodes
	IdxS = rbtNodes = rbt_getAllNodes(); IdxE = IdxS + size;

		// Cycles through all nodes of this red-black tree
	while(true) {
			// If the node is not found in this red-black tree, just return
		if (IdxS == IdxE) {
			delete [] rbtNodes;
			return;
		}
			// Else, if node is found in this red-black tree, begin repositioning
		if (*IdxS == curr)
			break;
			// Otherwise, check another node
		IdxS++;
	}

		// Sets the node's key and begins repositioning
	curr->key = someFunc();
	nodeReposition(rbtNodes, IdxS, IdxE, curr);
}

/*
 * function_identifier: Changes the key value of a given node, according to a given function
 *						Automatically repositions the node in the red-black tree
 *						Does nothing if the node is not from this tree
 * parameters: 			A node from the current tree, and function that returns a key value (with key and data parameters)
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcKey(rbtNode<K,D> *curr, K(*someFunc)(K, D)) {
		// Initializes values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxE;

		// Gets all nodes
	IdxS = rbtNodes = rbt_getAllNodes(); IdxE = IdxS + size;

		// Cycles through all nodes of this red-black tree
	while(true) {
			// If the node is not found in this red-black tree, just return
		if (IdxS == IdxE) {
			delete [] rbtNodes;
			return;
		}
			// Else, if node is found in this red-black tree, begin repositioning
		if (*IdxS == curr)
			break;
			// Otherwise, check another node
		IdxS++;
	}

		// Sets the node's key and begins repositioning
	curr->key = someFunc(curr->key, curr->data);
	nodeReposition(rbtNodes, IdxS, IdxE, curr);
}

/*
 * function_identifier: Changes the key value of a given node, according to a given function
 *						Automatically repositions the node in the red-black tree
 *						Does nothing if the node is not from this tree
 * parameters: 			A node from the current tree, and function that returns a key value (with key parameter)
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcKeyK(rbtNode<K,D> *curr, K (*someFunc)(K)) {
		// Initializes values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxE;

		// Gets all nodes
	IdxS = rbtNodes = rbt_getAllNodes(); IdxE = IdxS + size;

		// Cycles through all nodes of this red-black tree
	while(true) {
			// If the node is not found in this red-black tree, just return
		if (IdxS == IdxE) {
			delete [] rbtNodes;
			return;
		}
			// Else, if node is found in this red-black tree, begin repositioning
		if (*IdxS == curr)
			break;
			// Otherwise, check another node
		IdxS++;
	}

		// Sets the node's key and begins repositioning
	curr->key = someFunc(curr->key);
	nodeReposition(rbtNodes, IdxS, IdxE, curr);
}

/*
 * function_identifier: Changes the key value of a given node, according to a given function
 *						Automatically repositions the node in the red-black tree
 *						Does nothing if the node is not from this tree
 * parameters: 			A node from the current tree, and function that returns a key value (with data parameter)
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcKeyD(rbtNode<K,D> *curr, K (*someFunc)(D)) {
		// Initializes values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxE;

		// Gets all nodes
	IdxS = rbtNodes = rbt_getAllNodes(); IdxE = IdxS + size;

		// Cycles through all nodes of this red-black tree
	while(true) {
			// If the node is not found in this red-black tree, just return
		if (IdxS == IdxE) {
			delete [] rbtNodes;
			return;
		}
			// Else, if node is found in this red-black tree, begin repositioning
		if (*IdxS == curr)
			break;
			// Otherwise, check another node
		IdxS++;
	}

		// Sets the node's key and begins repositioning
	curr->key = someFunc(curr->data);
	nodeReposition(rbtNodes, IdxS, IdxE, curr);
}

/*
 * function_identifier: Changes the key value of all nodes, in the red-black tree
 *						Automatically repositions the nodes in the red-black tree
 * parameters: 			A key value
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_setAllKeys(K key) {
		// Return if red-black tree is empty
	if (!size)
		return;

		// Initializes values
	rbtNode<K,D> **IdxS, **IdxE;

		// Gets all nodes
	IdxS = rbt_getAllNodes(); IdxE = IdxS + size;

		// Cycles through all nodes of this red-black tree
	while(IdxS != IdxE)
			// Changes the key of all values
		(*IdxS++)->key = key;

		// Repositions all nodes
	IdxS -= size;
	fooPtr<K,D> = keyDataCompair;
	mergeSortCallerBase(IdxS, 0, size - 1);

		// Clears array of nodes
	delete [] IdxS;
}

/*
 * function_identifier: Changes the key value of all nodes, in the red-black tree, according to a given function
 *						Automatically repositions the nodes in the red-black tree
 * parameters: 			A function that returns a key value
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcAllKeys(K (*someFunc)()) {
		// Return if red-black tree is empty
	if (!size)
		return;

		// Initializes values
	rbtNode<K,D> **IdxS, **IdxE;

		// Gets all nodes
	IdxS = rbt_getAllNodes(); IdxE = IdxS + size;

		// Cycles through all nodes of this red-black tree
	while(IdxS != IdxE)
			// Changes the keys of all values
		(*IdxS++)->key = someFunc();

		// Repositions all nodes
	IdxS -= size;
	fooPtr<K,D> = keyDataCompair;
	mergeSortCallerBase(IdxS, 0, size - 1);

		// Clears array of nodes
	delete [] IdxS;
}

/*
 * function_identifier: Changes the key value of all nodes, in the red-black tree, according to a given function
 *						Automatically repositions the nodes in the red-black tree
 * parameters: 			A function that returns a key value (with kay and data parameters)
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcAllKeys(K (*someFunc)(K, D)) {
		// Return if red-black tree is empty
	if (!size)
		return;

		// Initializes values
	rbtNode<K,D> **IdxS, **IdxE;

		// Gets all nodes
	IdxS = rbt_getAllNodes(); IdxE = IdxS + size;

		// Cycles through all nodes of this red-black tree
	while(IdxS != IdxE) {
			// Changes the keys of all values
		(*IdxS)->key = someFunc((*IdxS)->key, (*IdxS)->data);
		IdxS++;
	}

		// Repositions all nodes
	IdxS -= size;
	fooPtr<K,D> = keyDataCompair;
	mergeSortCallerBase(IdxS, 0, size - 1);

		// Clears array of nodes
	delete [] IdxS;
}

/*
 * function_identifier: Changes the key value of all nodes, in the red-black tree, according to a given function
 *						Automatically repositions the nodes in the red-black tree
 * parameters: 			A function that returns a key value (with key parameter)
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcAllKeysK(K (*someFunc)(K)) {
		// Return if red-black tree is empty
	if (!size)
		return;

		// Initializes values
	rbtNode<K,D> **IdxS, **IdxE;

		// Gets all nodes
	IdxS = rbt_getAllNodes(); IdxE = IdxS + size;

		// Cycles through all nodes of this red-black tree
	while(IdxS != IdxE) {
			// Changes the keys of all values
		(*IdxS)->key = someFunc((*IdxS)->key);
		IdxS++;
	}

		// Repositions all nodes
	IdxS -= size;
	fooPtr<K,D> = keyDataCompair;
	mergeSortCallerBase(IdxS, 0, size - 1);

		// Clears array of nodes
	delete [] IdxS;
}

/*
 * function_identifier: Changes the key value of all nodes, in the red-black tree, according to a given function
 *						Automatically repositions the nodes in the red-black tree
 * parameters: 			A function that returns a key value (with data parameter)
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcAllKeysD(K (*someFunc)(D)) {
		// Return if red-black tree is empty
	if (!size)
		return;

		// Initializes values
	rbtNode<K,D> **IdxS, **IdxE;

		// Gets all nodes
	IdxS = rbt_getAllNodes(); IdxE = IdxS + size;

		// Cycles through all nodes of this red-black tree
	while(IdxS != IdxE) {
			// Changes the keys of all values
		(*IdxS)->key = someFunc((*IdxS)->data);
		IdxS++;
	}

		// Repositions all nodes
	IdxS -= size;
	fooPtr<K,D> = keyDataCompair;
	mergeSortCallerBase(IdxS, 0, size - 1);

		// Clears array of nodes
	delete [] IdxS;
}

/*	============================================================================  */
/* |                                                                            | */
/* |                              DATA FUNCTIONS                                | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: Changes the data value of a given node
 *						Automatically repositions the node in the red-black tree
 *						Does nothing if the node is not from this tree
 * parameters: 			A node from the current tree, and a new data
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_setData(rbtNode<K,D> *curr, D data) {
		// Initializes values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxE;

		// Gets all nodes
	IdxS = rbtNodes = rbt_getAllNodes(); IdxE = IdxS + size;

		// Cycles through all nodes of this red-black tree
	while(true) {
			// If the node is not found in this red-black tree, just return
		if (IdxS == IdxE) {
			delete [] rbtNodes;
			return;
		}
			// Else, if node is found in this red-black tree, begin repositioning
		if (*IdxS == curr)
			break;
			// Otherwise, check another node
		IdxS++;
	}

		// Sets the node's data and begins repositioning
	curr->data = data;
	nodeReposition(rbtNodes, IdxS, IdxE, curr);
}

/*
 * function_identifier: Changes the data value of a given node, according to a given function
 *						Automatically repositions the node in the red-black tree
 *						Does nothing if the node is not from this tree
 * parameters: 			A node from the current tree, and function that returns a data value
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcData(rbtNode<K,D> *curr, D (*someFunc)()) {
		// Initializes values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxE;

		// Gets all nodes
	IdxS = rbtNodes = rbt_getAllNodes(); IdxE = IdxS + size;

		// Cycles through all nodes of this red-black tree
	while(true) {
			// If the node is not found in this red-black tree, just return
		if (IdxS == IdxE) {
			delete [] rbtNodes;
			return;
		}
			// Else, if node is found in this red-black tree, begin repositioning
		if (*IdxS == curr)
			break;
			// Otherwise, check another node
		IdxS++;
	}

		// Sets the node's data and begins repositioning
	curr->data = someFunc();
	nodeReposition(rbtNodes, IdxS, IdxE, curr);
}

/*
 * function_identifier: Changes the data value of a given node, according to a given function
 *						Automatically repositions the node in the red-black tree
 *						Does nothing if the node is not from this tree
 * parameters: 			A node from the current tree, and function that returns a data value (with key and data parameters)
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcData(rbtNode<K,D> *curr, D (*someFunc)(K, D)) {
		// Initializes values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxE;

		// Gets all nodes
	IdxS = rbtNodes = rbt_getAllNodes(); IdxE = IdxS + size;

		// Cycles through all nodes of this red-black tree
	while(true) {
			// If the node is not found in this red-black tree, just return
		if (IdxS == IdxE) {
			delete [] rbtNodes;
			return;
		}
			// Else, if node is found in this red-black tree, begin repositioning
		if (*IdxS == curr)
			break;
			// Otherwise, check another node
		IdxS++;
	}

		// Sets the node's data and begins repositioning
	curr->data = someFunc(curr->key, curr->data);
	nodeReposition(rbtNodes, IdxS, IdxE, curr);
}

/*
 * function_identifier: Changes the data value of a given node, according to a given function
 *						Automatically repositions the node in the red-black tree
 *						Does nothing if the node is not from this tree
 * parameters: 			A node from the current tree, and function that returns a data value (with key parameter)
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcDataK(rbtNode<K,D> *curr, D (*someFunc)(K)) {
		// Initializes values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxE;

		// Gets all nodes
	IdxS = rbtNodes = rbt_getAllNodes(); IdxE = IdxS + size;

		// Cycles through all nodes of this red-black tree
	while(true) {
			// If the node is not found in this red-black tree, just return
		if (IdxS == IdxE) {
			delete [] rbtNodes;
			return;
		}
			// Else, if node is found in this red-black tree, begin repositioning
		if (*IdxS == curr)
			break;
			// Otherwise, check another node
		IdxS++;
	}

		// Sets the node's data and begins repositioning
	curr->data = someFunc(curr->key);
	nodeReposition(rbtNodes, IdxS, IdxE, curr);
}

/*
 * function_identifier: Changes the data value of a given node, according to a given function
 *						Automatically repositions the node in the red-black tree
 *						Does nothing if the node is not from this tree
 * parameters: 			A node from the current tree, and function that returns a data value (with data parameter)
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcDataD(rbtNode<K,D> *curr, D (*someFunc)(D)) {
		// Initializes values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxE;

		// Gets all nodes
	IdxS = rbtNodes = rbt_getAllNodes(); IdxE = IdxS + size;

		// Cycles through all nodes of this red-black tree
	while(true) {
			// If the node is not found in this red-black tree, just return
		if (IdxS == IdxE) {
			delete [] rbtNodes;
			return;
		}
			// Else, if node is found in this red-black tree, begin repositioning
		if (*IdxS == curr)
			break;
			// Otherwise, check another node
		IdxS++;
	}

		// Sets the node's data and begins repositioning
	curr->data = someFunc(curr->data);
	nodeReposition(rbtNodes, IdxS, IdxE, curr);
}

/*
 * function_identifier: Changes the data value of all nodes, in the red-black tree
 *						Automatically repositions the nodes in the red-black tree
 * parameters: 			A data value
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_setAllData(D data) {
		// Return if red-black tree is empty
	if (!size)
		return;

		// Initializes values
	rbtNode<K,D> **IdxS, **IdxE;

		// Gets all nodes
	IdxS = rbt_getAllNodes(); IdxE = IdxS + size;

		// Cycles through all nodes of this red-black tree
	while(IdxS != IdxE)
			// Changes the data of all values
		(*IdxS++)->data = data;

		// Repositions all nodes
	IdxS -= size;
	fooPtr<K,D> = keyDataCompair;
	mergeSortCallerBase(IdxS, 0, size - 1);

		// Clears array of nodes
	delete [] IdxS;
}

/*
 * function_identifier: Changes the data value of all nodes, in the red-black tree, according to a given function
 *						Automatically repositions the nodes in the red-black tree
 * parameters: 			A function that returns a data value
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcAllData(D (*someFunc)()) {
		// Return if red-black tree is empty
	if (!size)
		return;

		// Initializes values
	rbtNode<K,D> **IdxS, **IdxE;

		// Gets all nodes
	IdxS = rbt_getAllNodes(); IdxE = IdxS + size;

		// Cycles through all nodes of this red-black tree
	while(IdxS != IdxE)
			// Changes the data of all values
		(*IdxS++)->data = someFunc();

		// Repositions all nodes
	IdxS -= size;
	fooPtr<K,D> = keyDataCompair;
	mergeSortCallerBase(IdxS, 0, size - 1);

		// Clears array of nodes
	delete [] IdxS;
}

/*
 * function_identifier: Changes the data value of all nodes, in the red-black tree, according to a given function
 *						Automatically repositions the nodes in the red-black tree
 * parameters: 			A function that returns a data value (with key and data parameters)
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcAllData(D (*someFunc)(K, D)) {
		// Return if red-black tree is empty
	if (!size)
		return;

		// Initializes values
	rbtNode<K,D> **IdxS, **IdxE;

		// Gets all nodes
	IdxS = rbt_getAllNodes(); IdxE = IdxS + size;

		// Cycles through all nodes of this red-black tree
	while(IdxS != IdxE) {
			// Changes the data of all values
		(*IdxS)->data = someFunc((*IdxS)->key, (*IdxS)->data);
		IdxS++;
	}

		// Repositions all nodes
	IdxS -= size;
	fooPtr<K,D> = keyDataCompair;
	mergeSortCallerBase(IdxS, 0, size - 1);

		// Clears array of nodes
	delete [] IdxS;
}

/*
 * function_identifier: Changes the data value of all nodes, in the red-black tree, according to a given function
 *						Automatically repositions the nodes in the red-black tree
 * parameters: 			A function that returns a data value (with key parameter)
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcAllDataK(D (*someFunc)(K)) {
		// Return if red-black tree is empty
	if (!size)
		return;

		// Initializes values
	rbtNode<K,D> **IdxS, **IdxE;

		// Gets all nodes
	IdxS = rbt_getAllNodes(); IdxE = IdxS + size;

		// Cycles through all nodes of this red-black tree
	while(IdxS != IdxE) {
			// Changes the data of all values
		(*IdxS)->data = someFunc((*IdxS)->key);
		IdxS++;
	}

		// Repositions all nodes
	IdxS -= size;
	fooPtr<K,D> = keyDataCompair;
	mergeSortCallerBase(IdxS, 0, size - 1);

		// Clears array of nodes
	delete [] IdxS;
}

/*
 * function_identifier: Changes the data value of all nodes, in the red-black tree, according to a given function
 *						Automatically repositions the nodes in the red-black tree
 * parameters: 			A function that returns a data value (with data parameter)
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcAllDataD(D (*someFunc)(D)) {
		// Return if red-black tree is empty
	if (!size)
		return;

		// Initializes values
	rbtNode<K,D> **IdxS, **IdxE;

		// Gets all nodes
	IdxS = rbt_getAllNodes(); IdxE = IdxS + size;

		// Cycles through all nodes of this red-black tree
	while(IdxS != IdxE) {
			// Changes the data of all values
		(*IdxS)->data = someFunc((*IdxS)->data);
		IdxS++;
	}

		// Repositions all nodes
	IdxS -= size;
	fooPtr<K,D> = keyDataCompair;
	mergeSortCallerBase(IdxS, 0, size - 1);

		// Clears array of nodes
	delete [] IdxS;
}

#endif /* _rbt_VALUE */