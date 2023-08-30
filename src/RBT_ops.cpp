#ifndef _rbt_OPERATIONS
#define _rbt_OPERATIONS
#include	"RBT_main.h"
#include	"RBT_indel.h"
#include	"RBT_ops.h"
#include	"RBT_traversal.h"
#include	"RBT_sort.h"

/*==============================================================================\
 | Program:		Key-Data Red-Black Tree Implementation							|
 | AUTHOR:		Xavier Alvarez 													|
 | CREATE DATE:	15-January-2023 												|
 | COPYRIGHT:	apache-2.0														|
 | VERSION:		1.0																|
 | DESCRIPTION:	A red-black semi-balanced binary search tree. Provides a		|
 |				multitude of useful functions, including O(log(N)) lookup,		|
 |				insert and delete operations.									|
 \=============================================================================*/

	/*	 GLOBAL VARIABLES	*/
namespace {
		// External From 'RBT_sort.cpp'
	template <typename K, typename D>
	extern inline bool(*fooPtr)(rbtNode<K,D> *, rbtNode<K,D> *);
}

/*	============================================================================  */
/* |                                                                            | */
/* |                          TREE BASIC OPERATIONS                             | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: Combines the nodes of two red-black trees and outputs a new red-black tree
 * parameters: 			A red-black tree with compatible key and data to the current red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::operator+(const rbTree<K,D> &rbt) const {
		// Ignores the trival case
	if (!rbt.size)
		return rbTree<K,D>(*this);
	if (!size)
		return rbTree<K,D>(rbt);

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes;
	size_t sizeNew;

		// Calculates the new tree size.
	sizeNew = size + rbt.size;

		// Checks for node overflow
	if (sizeNew < size)
		throw std::overflow_error("overflow - red black tree(s) to large to perform opperation");

		// Initializes the needed array
	rbtNodes = new rbtNode<K,D> *[sizeNew];

		// Gets the needed nodes and indexes
	rbt_getAllNodes(rbtNodes);
	rbt.rbt_getAllNodes(rbtNodes + size);

		// Sorts the nodes
	fooPtr<K,D> = keyDataCompair;
	mergeSortBase(rbtNodes, 0, size - 1, sizeNew);

		// Create a tree from nodes
	rbTree<K,D> rbtNew = rbTree<K,D>();
	rbtNew.size = sizeNew;
	rbtNew.treeify(0, sizeNew - 1, rbtNodes, &rbtNew.root);

		// Clears accessed node array
	delete [] rbtNodes;

		// Return the finished tree
	return rbtNew;
}

/*
 * function_identifier: Combines a red-black tree's node into the current tree
 * parameters: 			A red-black tree with compatible key and data to the current red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::operator+=(const rbTree<K,D> &rbt) {
		// Ignores the trival case
	if (!rbt.size)
		return *this;
	if (!size) {
		size = rbt.size;
		copyTree(rbt.root, &(this->root));
		return *this;
	}

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS1, **IdxS2, **IdxE2;
	size_t sizeNew;

		// Calculates the new tree size.
	sizeNew = size + rbt.size;

		// Checks for node overflow
	if (sizeNew < size)
		throw std::overflow_error("overflow - red black tree(s) to large to perform opperation");

		// Gets the needed nodes and indexes
	rbtNodes = new rbtNode<K,D> *[sizeNew];
	rbt_getAllNodes(rbtNodes);
	IdxS2 = rbt.rbt_getAllNodes();
	IdxE2 = IdxS2 + rbt.size; IdxS1 = rbtNodes + size;

		// Deep copies the second tree's nodes
	while(IdxS2 != IdxE2) {
		*IdxS1 = new rbtNode<K,D>(*IdxS2);
		IdxS1++; IdxS2++;
	}

		// Clears second tree's accessed node array
	delete [] (IdxE2 - rbt.size);

		// Makes sure all the nodes are sorted
	fooPtr<K,D> = keyDataCompair;
	mergeSortBase(rbtNodes, 0, size - 1, sizeNew);

		// Create a tree from nodes
	size = sizeNew;
	treeifyShallow(0, sizeNew - 1, rbtNodes, &root);
	root->parent = NULL;

		// Clears this tree's accessed node array
	delete [] rbtNodes;

		// Return this object
	return *this;
}

/*
 * function_identifier: The alias for the + operator
 * parameters: 			A red-black tree with compatible key and data to the current red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
inline rbTree<K,D> rbTree<K,D>::rbt_add(const rbTree<K,D> &rbt) const {
	return *this + rbt;
}

/*
 * function_identifier: The alias for the += operator
 * parameters: 			A red-black tree with compatible key and data to the current red-black tree
 * return value:		This object		
*/
template <typename K, typename D>
inline rbTree<K,D> rbTree<K,D>::rbt_addI(const rbTree<K,D> &rbt) {
	return *this += rbt;
}

/*
 * function_identifier: The base template for the sub opperations
 * parameters: 			A red-black tree, a function to copy over nodes not subtracted
 * return value:		A red-black tree
*/
template <typename K, typename D>
inline rbTree<K,D> rbTree<K,D>::subBase(const rbTree<K,D> &rbt, void (*foo)(rbtNode<K,D> **&, rbtNode<K,D> **&, rbtNode<K,D> **&, rbtNode<K,D> **&, rbtNode<K,D> **&)) const {
		// Ignores the trival case
	if (!size)
		return rbTree<K,D>();
	if (!rbt.size)
		return rbTree<K,D>(*this);

		// Initializes the needed values
	rbtNode<K,D> **IdxS1, **IdxE1, **IdxS2, **IdxE2, **IdxM;

		// Gets the needed nodes and indexes
	IdxS1 = IdxM = rbt_getAllNodes(); IdxE1 = IdxS1 + size;
	IdxS2 = rbt.rbt_getAllNodes(); IdxE2 = IdxS2 + rbt.size;

		// Copies over only not-subtracted nodes
	foo(IdxS1, IdxM, IdxE1, IdxS2, IdxE2);

		// Checks if any nodes were ignored
	if (IdxM != IdxS1)
			// If some nodes were ignored, place the remaining nodes into the array
		while(IdxS1 != IdxE1)
			*IdxM++ = *IdxS1++;
	else
			// If no nodes deleted, just skip to the end
			// Everything is already fine how it is
		IdxM = IdxE1;

		// Moves the first index back to the start
	IdxS1 = IdxE1 - size;

		// Create a tree from nodes
	rbTree<K,D> newTree = rbTree<K,D>();
	newTree.size = IdxM - IdxS1;

	if (newTree.size)
			// If there are remaining nodes, place them into the new tree
		newTree.treeify(0, newTree.size - 1, IdxS1, &newTree.root);

		// Clears this accessed node arrays
	delete [] IdxS1;
	delete [] (IdxE2 - rbt.size);

		// Return the finished tree
	return newTree;
}

/*
 * function_identifier: Outputs a new red-black tree with all nodes, in this tree, that are not in the second provided tree
 * parameters: 			A red-black tree with compatible key and data to the current red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::operator-(const rbTree<K,D> &rbt) const {
	return subBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxM, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE2) -> void {
			// Cycles through the entire array of accessed nodes and data
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal keys and data
				// If so, ignore it and move forward
				// If not, save it, then move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
					IdxS1++; IdxS2++;
				} else if ((*IdxS1)->data < (*IdxS2)->data)
					*IdxM++ = *IdxS1++;
				else
					IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				IdxS2++;
	});
}

/*
 * function_identifier: Deletes all nodes in the current tree with the same data and key as a node in the second tree
 * parameters: 			A red-black tree with compatible key and data to the current red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::operator-=(const rbTree<K,D> &rbt) {
		// Gets the needed nodes
	auto delNodes = rbt.rbt_getAllNodes();
		// Deletes the nodes from current tree
	rbt_repDelete(rbt.size, delNodes);
		// Clears this accessed node array
	delete [] delNodes;
		// Return this object
	return *this;
}

/*
 * function_identifier: The alias for the - operator
 * parameters: 			A red-black tree with compatible key and data to the current red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
inline rbTree<K,D> rbTree<K,D>::rbt_sub(const rbTree<K,D> &rbt) const {
	return *this - rbt;
}

/*
 * function_identifier: The alias for the -= operator
 * parameters: 			A red-black tree with compatible key and data to the current red-black tree
 * return value:		This object		
*/
template <typename K, typename D>
inline rbTree<K,D> rbTree<K,D>::rbt_subI(const rbTree<K,D> &rbt) {
	return *this -= rbt;
}

/*
 * function_identifier: Outputs a new red-black tree with all nodes, in this tree, with keys that are not in the second provided tree
 * parameters: 			A red-black tree with compatible key and data to the current red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_subKeys(const rbTree<K,D> &rbt) const {
	return subBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxM, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE2) -> void {
			// Cycles through the entire array of accessed nodes and data
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, ignore it and move forward
				// If not, save it, then move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				IdxS1++; IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				IdxS2++;
	});
}

/*
 * function_identifier: Deletes all nodes in the current tree with the same keys as a node in the second tree
 * parameters: 			A red-black tree with compatible keys to the current red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_subKeysI(const rbTree<K,D> &rbt) {
		// Gets the needed keys
	auto keys = rbt.rbt_getAllKeys();
		// Deletes the nodes from current tree
	rbt_repDeleteKey(rbt.size, keys);
		// Clears this accessed key array
	delete [] keys;
		// Return this object
	return *this;
}

/*
 * function_identifier: Outputs a new red-black tree with all nodes, in this tree, with data values that are not in the second provided tree
 * parameters: 			A red-black tree with compatible key and data to the current red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_subData(const rbTree<K,D> &rbt) const {
	return subBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxM, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE2) -> void {
			// Cycles through the entire array of accessed nodes and data
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal data
				// If so, ignore it and move forward
				// If not, save it, then move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
				IdxS1++; IdxS2++;
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				*IdxM++ = *IdxS1++;
			else
				IdxS2++;
	});
}

/*
 * function_identifier: Deletes all nodes in the current tree with the same data values as a node in the second tree
 * parameters: 			A red-black tree with compatible data values to the current red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_subDataI(const rbTree<K,D> &rbt) {
		// Gets the needed data
	auto data = rbt.rbt_getAllData();
		// Deletes the nodes from current tree
	rbt_repDeleteData(rbt.size, data);
		// Clears this accessed data array
	delete [] data;
		// Return this object
	return *this;
}

/*
 * function_identifier: Creates and returns a red-black tree with all the nodes in the current tree duplicated
 *						a certain number of times
 * parameters: 			The number of times each node should be duplicated
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::operator*(const size_t &val) const {
		// Ignores the trival case
	if (val <= 1) {
		if (val == 0)
			return rbTree<K,D>();
		else
			return rbTree<K,D>(*this);
	}

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxE;
	size_t sizeNew;

		// Calculates the new tree size.
	sizeNew = size * val;

		// Checks for node overflow
	if (sizeNew < size)
		throw std::overflow_error("overflow - red black tree(s) too large to perform opperation");

		// Initializes array while geting the needed nodes and indexes
	rbtNodes = new rbtNode<K,D> *[sizeNew];
	rbt_getAllNodes(rbtNodes);
	IdxS = rbtNodes + size - 1; IdxE = rbtNodes + sizeNew - 1; 

		// Duplicates each node, in the current tree, a certain number of times
	while(IdxS >= rbtNodes) {
		for(size_t i=0; i<val; i++)
			*IdxE-- = new rbtNode<K,D>(*IdxS);
		IdxS--;
	}

		// Creates a tree from the duplicated nodes
	rbTree<K,D> newTree = rbTree<K,D>();
	newTree.size = sizeNew;
	newTree.treeifyShallow(0, sizeNew - 1, rbtNodes, &newTree.root);

		// Clears accessed node array
	delete [] rbtNodes;

		// Return the finished tree
	return newTree;
}

/*
 * function_identifier: Duplicates all nodes in the red-black tree a certain number of times
 * parameters: 			The number of times each node should be duplicated
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::operator*=(const size_t &val) {
		// Ignores the trival case
	if (val <= 1) {
		if (val == 0)
			rbt_clear();
		return *this;
	}

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxE;
	size_t sizeNew;

		// Calculates the new tree size.
	sizeNew = size * val;

		// Checks for node overflow
	if (sizeNew < size)
		throw std::overflow_error("overflow - red black tree(s) to large to perform opperation");

		// Initializes array while geting the needed nodes and indexes
	rbtNodes = new rbtNode<K,D> *[sizeNew];
	rbt_getAllNodes(rbtNodes);
	IdxS = rbtNodes + size - 1; IdxE = rbtNodes + sizeNew - 1; 

		// Duplicates each node, in the current tree, a certain number of times
	while(IdxS >= rbtNodes) {
		*IdxE-- = *IdxS;
		for(size_t i=1; i<val; i++)
			*IdxE-- = new rbtNode<K,D>(*IdxS);
		IdxS--;
	}

		// Sets the current tree to one made from the duplicated nodes
	size = sizeNew;
	treeifyShallow(0, size - 1, rbtNodes, &root);

		// Clears accessed node array
	delete [] rbtNodes;

		// Return this object
	return *this;
}

/*
 * function_identifier: The alias for the * operator
 * parameters: 			The number of times each node should be duplicated
 * return value:		A red-black tree
*/
template <typename K, typename D>
inline rbTree<K,D> rbTree<K,D>::rbt_mul(const size_t &val) const {
	return *this * val;
}

/*
 * function_identifier: The alias for the *= operator
 * parameters: 			The number of times each node should be duplicated
 * return value:		This object		
*/
template <typename K, typename D>
inline rbTree<K,D> rbTree<K,D>::rbt_mulI(const size_t &val) {
	return *this *= val;
}

/*	============================================================================  */
/* |                                                                            | */
/* |                        TREE ADVANCED OPERATIONS                            | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: Inverts the priority of all nodes in the tree (largest key swaps with smallest, second largest swaps with second smallest, etc.)
 * parameters: 			N/A
 * return value:		N/A
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_invert() {
		// Ignores the trival case
	if (!size)
		return *this;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxE, **IdxM;

		// Gets the needed nodes and indexes
	IdxS = rbtNodes = rbt_getAllNodes();
	IdxE = IdxS + size - 1;
	IdxM = new rbtNode<K,D> *[size];

		// Deep copies all the nodes over, reversing the priorities
	while(rbtNodes <= IdxE)
		*IdxM++ = new rbtNode<K,D>((*IdxS++)->key, (*IdxE--)->data);

		// Clears accessed node array
	delete [] rbtNodes;

		// Moves the pointer back to the start
	IdxM -= size;

		// Sorts the nodes
	fooPtr<K,D> = keyDataCompair;
	mergeSortCallerBase(IdxM, 0, size - 1);

		// Creates a tree from the duplicated nodes
	rbTree<K,D> newTree = rbTree<K,D>();
	newTree.size = size;
	newTree.treeifyShallow(0, size - 1, IdxM, &newTree.root);

		// Clears the new tree's node array
	delete [] IdxM;

		// Return the finished tree
	return newTree;
}

/*
 * function_identifier: Inverts the priority of all nodes in the tree (largest key swaps with smallest, second largest swaps with second smallest, etc.)
 * parameters: 			N/A
 * return value:		N/A
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_invertI() {
		// Ignores the trival case
	if (!size)
		return *this;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxE;
	D tmp;

		// Gets the needed nodes and indexes
	IdxS = rbtNodes = rbt_getAllNodes();
	IdxE = IdxS + size - 1;

		// Swaps the priorities
	while(IdxS < IdxE) {
		tmp = (*IdxS)->data;
		(*IdxS++)->data = (*IdxE)->data;
		(*IdxE--)->data = tmp;
	}

		// Sorts the nodes
	fooPtr<K,D> = keyDataCompair;
	mergeSortCallerBase(rbtNodes, 0, size - 1);

		// Clears accessed node array
	delete [] rbtNodes;

		// Return this object
	return *this;
}

/*
 * function_identifier: The base template for the distribute and scatter opperations
 * parameters: 			The number of subtrees to create, the traversal to divide by, a trival case
 *						function, the main distribute/scatter function, and either treeify or
 *						treeifyShallow
 * return value:		An array of smaller subtrees
*/
template <typename K, typename D>
inline rbTree<K,D> *rbTree<K,D>::divideBase(
	size_t div,
	enum rbtraversal traversal,
	rbTree<K,D> *(rbTree<K, D>::*foo)(),
	void (rbTree<K, D>::*bar)(size_t, rbtNode<K,D> **, rbTree<K,D> *, enum rbtraversal, size_t (rbTree<K, D>::*)(size_t, size_t, rbtNode<K,D> **, rbtNode<K,D> **)),
	size_t (rbTree<K, D>::*nar)(size_t, size_t, rbtNode<K,D> **, rbtNode<K,D> **))
{
		// Error or ignore the trival case
	if (div <= 1) {
		if (!div)
				// If div is 0, then obviously error; no dividing by 0!
			throw std::runtime_error("math error - attempted to distribute a tree into 0 subtrees");
		else
				// Returns the trival
			return (this->*foo)();
	}

		// Initializes the needed values
	rbTree<K,D> *TreeArray;
	rbtNode<K,D> **IdxS;
	fooPtr<K,D> = keyDataCompair;

		// Gets the needed tree, nodes, and indexes
	TreeArray = new rbTree<K,D>[div]();
	IdxS = rbt_getAllNodes(traversal);

		// Base values needed to determine how to distribute
	(this->*bar)(div, IdxS, TreeArray, traversal, nar);

		// Clears accessed node array
	delete [] IdxS;

		// Return the array of trees
	return TreeArray;
}

/*
 * function_identifier: The trival case for the deep copy distribute and scatter opperations
 * parameters: 			N/A
 * return value:		An array of one smaller subtree
*/
template <typename K, typename D>
inline rbTree<K,D> *rbTree<K,D>::deepDivideHelper() {
		// If div is 1, return the trival case
	rbTree<K,D> *TreeArray = new rbTree<K,D>[1];
	*TreeArray = rbTree<K,D>(*this);
	return TreeArray;
};

/*
 * function_identifier: The trival case for the shallow copy distribute and scatter opperations
 * parameters: 			N/A
 * return value:		An array of one smaller subtree
*/
template <typename K, typename D>
inline rbTree<K,D> *rbTree<K,D>::shallowDivideHelper() {
		// If div is 1, return the trival case
	rbTree<K,D> *TreeArray = new rbTree<K,D>[1];
	TreeArray->root = this->root;
	TreeArray->size = this->size;
	return TreeArray;
};

/*
 * function_identifier: The main distribute function for the base template to use
 *						Distributes the current tree into multiple subtrees, via blocks on a
 *						certain traversal
 * parameters: 			Number of subtrees to distribute into, the start and end of a list of
 *						nodes, an array of subtrees, the traversal to base the distribution on,
 *						and either treeify or treeifyShallow
 * return value:		N/A
*/
template <typename K, typename D>
inline void rbTree<K,D>::distributeDivideHelper(
	size_t div,
	rbtNode<K,D> **IdxS,
	rbTree<K,D> *TreeS,
	enum rbtraversal traversal,
	size_t (rbTree<K, D>::*foo)(size_t, size_t, rbtNode<K,D> **, rbtNode<K,D> **))
{
		// Initializes needed values
	bool check;
	size_t rem = size % div;
	div = size / div;
	rbtNode<K,D> **IdxE = IdxS + size;

		// Only copy the nodes; keep the current tree intact
		// Cycles through the entire array of accessed nodes
	while(IdxS < IdxE) {
			// If some remainder is remaining, add one more to the current subset of nodes
		check = (rem != 0);

			// Sorts the list of nodes if the given traversal didn't already keep them sorted
		if (traversal != IN_ORDER)
			mergeSortCallerBase(IdxS, 0, div - !check);

			// Converts this current subset of nodes into a new tree (deep copy)
		(TreeS->*foo)(0, div - !check, IdxS, &TreeS->root);
		TreeS++;

			// Moves forward to next subset of nodes
		IdxS += div + check;
		if (check)
			rem--; // Decrease remainder
	}
};

/*
 * function_identifier: The main distribute function for the base template to use
 *						Distributes the current tree into multiple subtrees, via slices on a
 *						certain traversal
 * parameters: 			Number of subtrees to distribute into, the start and end of a list of
 *						nodes, an array of subtrees, the traversal to base the distribution on,
 *						and either treeify or treeifyShallow
 * return value:		N/A
*/
template <typename K, typename D>
inline void rbTree<K,D>::scatterDivideHelper(
	size_t div,
	rbtNode<K,D> **IdxS,
	rbTree<K,D> *TreeS,
	enum rbtraversal traversal,
	size_t (rbTree<K, D>::*foo)(size_t, size_t, rbtNode<K,D> **, rbtNode<K,D> **))
{
	size_t len = size / div + ((size % div) > 0);
	div = div < size ? div : size;
	rbtNode<K,D> **IdxE = IdxS + size, **idxs, **st, **ed, **rbtNodes = new rbtNode<K,D> *[len];
	ed = rbtNodes + len;

	for(size_t i=0; i<div; i++) {
		for(idxs = IdxS + i, st = rbtNodes; st < ed && idxs < IdxE; st++, idxs += div)
			*st = *idxs;

		len = st - rbtNodes - 1;

			// Sorts the current subset of nodes if the given traversal didn't already keep them sorted
		if (traversal != IN_ORDER)
			mergeSortCallerBase(rbtNodes, 0, len);

		(TreeS->*foo)(0, len, rbtNodes, &TreeS->root);
		(TreeS++)->root->parent = NULL;
	}

	delete [] rbtNodes;
};

/*
 * function_identifier: Distributes the nodes, in this current red-black tree, a certain number of
 *						smaller trees, via blocks
 *						The original tree is kept intact during distribution
 * parameters: 			The number of trees to create and the traversal type to distribute by
 * return value:		An array of rbTrees
*/
template <typename K, typename D>
rbTree<K,D> *rbTree<K,D>::rbt_distribute(size_t div, enum rbtraversal traversal) {
		// Gets and returns the subtree array
	return divideBase(div, traversal, &rbTree<K, D>::deepDivideHelper, &rbTree<K,D>::distributeDivideHelper, &rbTree<K,D>::treeify);
}

/*
 * function_identifier: Distributes the nodes, in this current red-black tree, a certain number of
 *						smaller trees, via blocks
 *						The original tree is NOT kept intact during distribution
 * parameters: 			The number of trees to create and the traversal type to distribute by
 * return value:		An array of rbTrees
*/
template <typename K, typename D>
rbTree<K,D> *rbTree<K,D>::rbt_distribute_shallow(size_t div, enum rbtraversal traversal) {
		// Gets the subtree array
	rbTree<K,D> *TreeArray = divideBase(div, traversal, &rbTree<K, D>::shallowDivideHelper, &rbTree<K,D>::distributeDivideHelper, &rbTree<K,D>::treeifyShallow);

		// All nodes no longer belong to this tree, so size is 0 and root is NULL
	size = 0; root = NULL;

		// Returns the array
	return TreeArray;
}

/*
 * function_identifier: Distributes the nodes, in this current red-black tree, a certain number of
 *						smaller trees, via slices
 *						The original tree is kept intact during distribution
 * parameters: 			The number of trees to create and the traversal type to distribute by
 * return value:		An array of rbTrees
*/
template <typename K, typename D>
rbTree<K,D> *rbTree<K,D>::rbt_scatter(size_t div, enum rbtraversal traversal) {
		// Gets and returns the subtree array
	return divideBase(div, traversal, &rbTree<K, D>::deepDivideHelper, &rbTree<K,D>::scatterDivideHelper, &rbTree<K,D>::treeify);
}

/*
 * function_identifier: Distributes the nodes, in this current red-black tree, a certain number of
 *						smaller trees, via slices
 *						The original tree is NOT kept intact during distribution
 * parameters: 			The number of trees to create and the traversal type to distribute by
 * return value:		An array of rbTrees
*/
template <typename K, typename D>
rbTree<K,D> *rbTree<K,D>::rbt_scatter_shallow(size_t div, enum rbtraversal traversal) {
		// Gets the subtree array
	rbTree<K,D> *TreeArray = divideBase(div, traversal, &rbTree<K, D>::shallowDivideHelper, &rbTree<K,D>::scatterDivideHelper, &rbTree<K,D>::treeifyShallow);

		// All nodes no longer belong to this tree, so size is 0 and root is NULL
	size = 0; root = NULL;

		// Returns the array
	return TreeArray;
}

#endif /* _rbt_OPERATIONS */