#ifndef _rbt_VALUE
#define _rbt_VALUE
#include	"RBT_main.h"
#include	"RBT_sort.h"

	/*	 GLOBAL VARIABLES	*/
namespace {
		// External From 'RBT_sort.cpp'
	template <typename K, typename D>
	extern inline bool(*fooPtr)(rbtNode<K,D> *, rbtNode<K,D> *);

		// External From 'def.h'
	extern inline void *rbt__otherData__;
}

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

/*
 * function_identifier: The base template for all single node set functions
 * parameters: 			A node to alter and a function that changes the node
 * return value:		N/A
*/
template <typename K, typename D>
template <typename F>
void rbTree<K,D>::rbt_setBase(rbtNode<K,D> *curr, F foo) {
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

		// Sets the node's relevant value(s) and begins repositioning
	foo(curr);
	nodeReposition(rbtNodes, IdxS, IdxE, curr);
}

/*
 * function_identifier: The base template for all full-tree set functions
 * parameters: 			A function that changes all nodes
 * return value:		N/A
*/
template <typename K, typename D>
template <typename F>
inline void rbTree<K,D>::rbt_funcAllBase(F foo) {
		// Return if red-black tree is empty
	if (!size)
		return;

		// Initializes values
	rbtNode<K,D> **IdxS, **IdxE;

		// Gets all nodes
	IdxS = rbt_getAllNodes(); IdxE = IdxS + size;

		// Reassigns all relevant values
	foo(IdxS, IdxE);

		// Repositions all nodes
	IdxS -= size;
	fooPtr<K,D> = keyDataCompair;
	mergeSortCallerBase(IdxS, 0, size - 1);
	treeifyShallow(0, size - 1, IdxS, &root);

		// Clears array of nodes
	delete [] IdxS;
}

/*	============================================================================  */
/* |                                                                            | */
/* |                             KEY DATA FUNCTIONS                             | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: Changes the key and data values of a given node
 *						Automatically repositions the node in the red-black tree
 *						Does nothing if the node is not from this tree
 * parameters: 			A node from the current tree and new key and data values
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_set(rbtNode<K,D> *curr, K key, D data) {
	rbt__otherData__ = new std::byte **[2];
	((K **)rbt__otherData__)[0] = &key;
	((D **)rbt__otherData__)[1] = &data;

	rbt_setBase(curr, [] (rbtNode<K,D> *curr) -> void {
		curr->key = *((K **)rbt__otherData__)[0];
		curr->data = *((D **)rbt__otherData__)[1];
	});
	delete [] (std::byte **)rbt__otherData__;
}

/*
 * function_identifier: Changes the key and data values of a given node
 *						Automatically repositions the node in the red-black tree
 *						Does nothing if the node is not from this tree
 * parameters: 			A node from the current tree and a function that returns
 *						new key and data values by reference
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_func(rbtNode<K,D> *curr, void (*someFunc)(K &, D &)) {
	typedef void (*funcType)(K &, D &);

	rbt__otherData__ = &someFunc;
	rbt_setBase(curr, [] (rbtNode<K,D> *curr) -> void {
		(*((funcType *)rbt__otherData__))(curr->key, curr->data);
	});
}

/*
 * function_identifier: Changes the key and data values of all nodes, in the red-black tree
 *						Automatically repositions the nodes in the red-black tree
 * parameters: 			A key value and a data value
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_setAll(K key, D data) {
	rbt__otherData__ = new std::byte **[2];
	((K **)rbt__otherData__)[0] = &key;
	((D **)rbt__otherData__)[1] = &data;

	rbt_funcAllBase([] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **IdxE) -> void {
			// Cycles through all nodes of this red-black tree
		while(IdxS != IdxE) {
				// Changes the key and data of all values
			(*IdxS)->key = *((K **)rbt__otherData__)[0];
			(*IdxS)->data = *((D **)rbt__otherData__)[1];
			IdxS++;
		}
	});
	delete [] (std::byte **)rbt__otherData__;
}

/*
 * function_identifier: Changes the key and data values of all nodes, in the red-black tree
 *						Automatically repositions the nodes in the red-black tree
 * parameters: 			A function that returns new key and data values by reference
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcAll(void (*someFunc)(K &, D &)) {
	typedef void (*funcType)(K &, D &);

	rbt__otherData__ = &someFunc;
	rbt_funcAllBase([] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **IdxE) -> void {
			// Cycles through all nodes of this red-black tree
		while(IdxS != IdxE) {
				// Changes the key and data of all values
			(*((funcType *)rbt__otherData__))((*IdxS)->key, (*IdxS)->data);
			IdxS++;
		}
	});
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
	rbt__otherData__ = &key;
	rbt_setBase(curr, [] (rbtNode<K,D> *curr) -> void {
		curr->key = *((K *)rbt__otherData__);
	});
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
	typedef K (*funcType)();

	rbt__otherData__ = &someFunc;
	rbt_setBase(curr, [] (rbtNode<K,D> *curr) -> void {
		curr->key = (*((funcType *)rbt__otherData__))();
	});
}

/*
 * function_identifier: Changes the key value of a given node, according to a given function
 *						Automatically repositions the node in the red-black tree
 *						Does nothing if the node is not from this tree
 * parameters: 			A node from the current tree, and function that returns a key value (with key and data parameters)
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcKey(rbtNode<K,D> *curr, K (*someFunc)(K, D)) {
	typedef K (*funcType)(K, D);

	rbt__otherData__ = &someFunc;
	rbt_setBase(curr, [] (rbtNode<K,D> *curr) -> void {
		curr->key = (*((funcType *)rbt__otherData__))(curr->key, curr->data);
	});
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
	typedef K (*funcType)(K);

	rbt__otherData__ = &someFunc;
	rbt_setBase(curr, [] (rbtNode<K,D> *curr) -> void {
		curr->key = (*((funcType *)rbt__otherData__))(curr->key);
	});
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
	typedef K (*funcType)(D);

	rbt__otherData__ = &someFunc;
	rbt_setBase(curr, [] (rbtNode<K,D> *curr) -> void {
		curr->key = (*((funcType *)rbt__otherData__))(curr->data);
	});
}

/*
 * function_identifier: Changes the key value of all nodes, in the red-black tree
 *						Automatically repositions the nodes in the red-black tree
 * parameters: 			A key value
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_setAllKeys(K key) {
	rbt__otherData__ = &key;
	rbt_funcAllBase([] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **IdxE) -> void {
			// Cycles through all nodes of this red-black tree
		while(IdxS != IdxE)
				// Changes the key of all values
			(*IdxS++)->key = *((K *)rbt__otherData__);
	});
}

/*
 * function_identifier: Changes the key value of all nodes, in the red-black tree, according to a given function
 *						Automatically repositions the nodes in the red-black tree
 * parameters: 			A function that returns a key value
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcAllKeys(K (*someFunc)()) {
	typedef K (*funcType)();

	rbt__otherData__ = &someFunc;
	rbt_funcAllBase([] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **IdxE) -> void {
			// Cycles through all nodes of this red-black tree
		while(IdxS != IdxE)
				// Changes the key of all values
			(*IdxS++)->key = (*((funcType *)rbt__otherData__))();
	});
}

/*
 * function_identifier: Changes the key value of all nodes, in the red-black tree, according to a given function
 *						Automatically repositions the nodes in the red-black tree
 * parameters: 			A function that returns a key value (with kay and data parameters)
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcAllKeys(K (*someFunc)(K, D)) {
	typedef K (*funcType)(K, D);

	rbt__otherData__ = &someFunc;
	rbt_funcAllBase([] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **IdxE) -> void {
			// Cycles through all nodes of this red-black tree
		while(IdxS != IdxE) {
				// Changes the key of all values
			(*IdxS)->key = (*((funcType *)rbt__otherData__))((*IdxS)->key, (*IdxS)->data);
			IdxS++;
		}
	});
}

/*
 * function_identifier: Changes the key value of all nodes, in the red-black tree, according to a given function
 *						Automatically repositions the nodes in the red-black tree
 * parameters: 			A function that returns a key value (with key parameter)
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcAllKeysK(K (*someFunc)(K)) {
	typedef K (*funcType)(K);

	rbt__otherData__ = &someFunc;
	rbt_funcAllBase([] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **IdxE) -> void {
			// Cycles through all nodes of this red-black tree
		while(IdxS != IdxE) {
				// Changes the key of all values
			(*IdxS)->key = (*((funcType *)rbt__otherData__))((*IdxS)->key);
			IdxS++;
		}
	});
}

/*
 * function_identifier: Changes the key value of all nodes, in the red-black tree, according to a given function
 *						Automatically repositions the nodes in the red-black tree
 * parameters: 			A function that returns a key value (with data parameter)
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcAllKeysD(K (*someFunc)(D)) {
	typedef K (*funcType)(D);

	rbt__otherData__ = &someFunc;
	rbt_funcAllBase([] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **IdxE) -> void {
			// Cycles through all nodes of this red-black tree
		while(IdxS != IdxE) {
				// Changes the key of all values
			(*IdxS)->key = (*((funcType *)rbt__otherData__))((*IdxS)->data);
			IdxS++;
		}
	});
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
	rbt__otherData__ = &data;
	rbt_setBase(curr, [] (rbtNode<K,D> *curr) -> void {
		curr->data = (*((D *)rbt__otherData__));
	});
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
	typedef D (*funcType)();

	rbt__otherData__ = &someFunc;
	rbt_setBase(curr, [] (rbtNode<K,D> *curr) -> void {
		curr->data = (*((funcType *)rbt__otherData__))();
	});
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
	typedef D (*funcType)(K, D);

	rbt__otherData__ = &someFunc;
	rbt_setBase(curr, [] (rbtNode<K,D> *curr) -> void {
		curr->data = (*((funcType *)rbt__otherData__))(curr->key, curr->data);
	});
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
	typedef D (*funcType)(K);

	rbt__otherData__ = &someFunc;
	rbt_setBase(curr, [] (rbtNode<K,D> *curr) -> void {
		curr->data = (*((funcType *)rbt__otherData__))(curr->key);
	});
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
	typedef D (*funcType)(D);

	rbt__otherData__ = &someFunc;
	rbt_setBase(curr, [] (rbtNode<K,D> *curr) -> void {
		curr->data = (*((funcType *)rbt__otherData__))(curr->data);
	});
}

/*
 * function_identifier: Changes the data value of all nodes, in the red-black tree
 *						Automatically repositions the nodes in the red-black tree
 * parameters: 			A data value
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_setAllData(D data) {
	rbt__otherData__ = &data;
	rbt_funcAllBase([] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **IdxE) -> void {
			// Cycles through all nodes of this red-black tree
		while(IdxS != IdxE)
				// Changes the data of all values
			(*IdxS++)->data = (*((D *)rbt__otherData__));
	});
}

/*
 * function_identifier: Changes the data value of all nodes, in the red-black tree, according to a given function
 *						Automatically repositions the nodes in the red-black tree
 * parameters: 			A function that returns a data value
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcAllData(D (*someFunc)()) {
	typedef D (*funcType)();

	rbt__otherData__ = &someFunc;
	rbt_funcAllBase([] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **IdxE) -> void {
			// Cycles through all nodes of this red-black tree
		while(IdxS != IdxE)
				// Changes the data of all values
			(*IdxS++)->data = (*((funcType *)rbt__otherData__))();
	});
}

/*
 * function_identifier: Changes the data value of all nodes, in the red-black tree, according to a given function
 *						Automatically repositions the nodes in the red-black tree
 * parameters: 			A function that returns a data value (with key and data parameters)
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcAllData(D (*someFunc)(K, D)) {
	typedef D (*funcType)(K, D);

	rbt__otherData__ = &someFunc;
	rbt_funcAllBase([] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **IdxE) -> void {
			// Cycles through all nodes of this red-black tree
		while(IdxS != IdxE) {
				// Changes the data of all values
			(*IdxS)->data = (*((funcType *)rbt__otherData__))((*IdxS)->key, (*IdxS)->data);
			IdxS++;
		}
	});
}

/*
 * function_identifier: Changes the data value of all nodes, in the red-black tree, according to a given function
 *						Automatically repositions the nodes in the red-black tree
 * parameters: 			A function that returns a data value (with key parameter)
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcAllDataK(D (*someFunc)(K)) {
	typedef D (*funcType)(K);

	rbt__otherData__ = &someFunc;
	rbt_funcAllBase([] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **IdxE) -> void {
			// Cycles through all nodes of this red-black tree
		while(IdxS != IdxE) {
				// Changes the data of all values
			(*IdxS)->data = (*((funcType *)rbt__otherData__))((*IdxS)->key);
			IdxS++;
		}
	});
}

/*
 * function_identifier: Changes the data value of all nodes, in the red-black tree, according to a given function
 *						Automatically repositions the nodes in the red-black tree
 * parameters: 			A function that returns a data value (with data parameter)
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_funcAllDataD(D (*someFunc)(D)) {
	typedef D (*funcType)(D);

	rbt__otherData__ = &someFunc;
	rbt_funcAllBase([] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **IdxE) -> void {
			// Cycles through all nodes of this red-black tree
		while(IdxS != IdxE) {
				// Changes the data of all values
			(*IdxS)->data = (*((funcType *)rbt__otherData__))((*IdxS)->data);
			IdxS++;
		}
	});
}

#endif /* _rbt_VALUE */