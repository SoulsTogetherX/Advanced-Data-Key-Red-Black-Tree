#ifndef _rbt_SET_OPERATIONS
#define _rbt_SET_OPERATIONS
#include	"RBT_main.h"
#include	"RBT_setops.h"
#include	"RBT_traversal.h"
#include	"RBT_sort.h"

/*	============================================================================  */
/* |                                                                            | */
/* |                      TREE UNION AND INTERSECTION                           | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: The base template for the or and xor opperations
 * parameters: 			A red-black tree and a function to copy over nodes
 * return value:		A red-black tree
*/
template <typename K, typename D>
template <typename F1, typename F2>
inline rbTree<K,D> rbTree<K,D>::orBase(const rbTree<K,D> &rbt, F1 foo, F2 bar, bool data, bool _xor) const {
		// If one or both of the trees are empty, or if the same tree is given, do the trival case
	if (!rbt.size || &rbt == this) {
		if (_xor && &rbt == this)
			return rbTree<K,D>();

		rbTree<K,D> ret = rbTree<K,D>(*this);
		(ret.*bar)();
		return ret;
	}
	if (!size) {
		rbTree<K,D> ret = rbTree<K,D>(rbt);
		(ret.*bar)();
		return ret;
	}

		// Initializes the needed values
	size_t sizeNew;
	rbtNode<K,D> **rbtNodes, **IdxS1, **IdxE1, **IdxS2, **IdxE2, **IdxM;

		// Calculates the maximum new tree size
	sizeNew = size + rbt.size;

		// Checks for node overflow
	if (sizeNew < size)
		throw std::overflow_error("overflow - red black tree(s) to large to perform opperation");

		// Initialize the needed array and indexes for the new tree
	rbtNodes = IdxM = new rbtNode<K,D> *[sizeNew];
	IdxS2 = IdxM + size; IdxE2 = IdxM + sizeNew;

		// Get all the nodes from both trees
	IdxS1 = rbt_getAllNodes(); IdxE1 = IdxS1 + size;
	rbt.rbt_getAllNodes(IdxS2);

		// If we are doing an 'or' based on data, sort nodes by data
	if (data) {
		fooPtr<K,D> = dataCompair;
		mergeSortCallerBase(IdxS1, 0, size - 1);
		mergeSortCallerBase(IdxS2, 0, rbt.size - 1);
	}

		// Cycles through the entire node array
	foo(IdxS1, IdxS2, IdxE1, IdxE2, IdxM);

		// Inserts any remaining nodes into the result array
	while(IdxS1 < IdxE1)
		*IdxM++ = *IdxS1++;
	while(IdxS2 < IdxE2)
		*IdxM++ = *IdxS2++;

		// Removes the array we don't need any more
	delete [] (IdxE1 - size);

		// Calculates the size of the new tree
	sizeNew = IdxM - rbtNodes;

		// Makes the tree
	rbTree<K,D> rbtNew = rbTree<K,D>();
	rbtNew.size = sizeNew;

	if (sizeNew) {
			// If we are doing an 'or' based on data, restore final nodes by key
		if (data) {
			fooPtr<K,D> = keyCompair;
			mergeSortCallerBase(rbtNodes, 0, sizeNew - 1);
		}

			// If there are remaining nodes, place them into the new tree
		rbtNew.treeify(0, sizeNew - 1, rbtNodes, &rbtNew.root);
	}

		// Deletes the remaining array
	delete [] rbtNodes;

		// Returns the finished tree
	return rbtNew;
}

/*
 * function_identifier: The base template for the orI and xorI opperations
 * parameters: 			A red-black tree and a function to copy over nodes
 * return value:		This object
*/
template <typename K, typename D>
template <typename F1, typename F2>
inline rbTree<K,D> rbTree<K,D>::orBaseI(const rbTree<K,D> &rbt, F1 foo, F2 bar, bool data, bool _xor) {
		// If one or both of the trees are empty, or if the same tree is given, do the trival case
	if (!rbt.size || &rbt == this) {
		if (_xor && &rbt == this)
			rbt_clear();
		else
			(this->*bar)();
		return *this;
	}
	if (!size) {
		size = rbt.size;
		copyTree(rbt.root, &(this->root));
		(this->*bar)();
		return *this;
	}

		// Calculates the maximum new tree size
	size_t sizeNew;
	rbtNode<K,D> **rbtNodes, **IdxS1, **IdxE1, **IdxS2, **IdxE2, **IdxM;

		// Calculates the new tree size.
	sizeNew = size + rbt.size;

		// Checks for node overflow
	if (sizeNew < size)
		throw std::overflow_error("overflow - red black tree(s) to large to perform opperation");

		// Initialize the needed arrays and indexes
	rbtNodes = IdxM = new rbtNode<K,D> *[sizeNew];
	IdxS2 = IdxM + size; IdxE2 = IdxM + sizeNew;

		// Get all the nodes from both trees
	IdxS1 = rbt_getAllNodes(); IdxE1 = IdxS1 + size;
	rbt.rbt_getAllNodes(IdxS2);

		// If we are doing an 'or' based on data, sort nodes by data
	if (data) {
		fooPtr<K,D> = dataCompair;
		mergeSortCallerBase(IdxS1, 0, size - 1);
		mergeSortCallerBase(IdxS2, 0, rbt.size - 1);
	}

		// Cycles through the entire node array
	foo(IdxS1, IdxS2, IdxE1, IdxE2, IdxM);

		// Inserts any remaining nodes into the result array
	while(IdxS1 < IdxE1)
		*IdxM++ = *IdxS1++;
	while(IdxS2 < IdxE2)
		*IdxM++ = new rbtNode<K,D>(*IdxS2++);

		// Removes the array we don't need any more
	delete [] (IdxE1 - size);

		// Calculates the size of the new tree
	size = IdxM - rbtNodes;

		// Makes the tree
	if (size) {
			// If we are doing an 'and' based on data, restore final nodes by key
		if (data) {
			fooPtr<K,D> = keyCompair;
			mergeSortCallerBase(rbtNodes, 0, size - 1);
		}

		treeifyShallow(0, size - 1, rbtNodes, &root);
		root->parent = NULL;
	} else
		root = NULL;

		// Deletes the remaining array
	delete [] rbtNodes;

		// Returns the finished tree
	return *this;
}

/*
 * function_identifier: Takes all nodes from nodes from two rbTrees and process them into a new rbTree
 *						For nodes with unique key-data pair unique between the trees, add them like normal. If two trees share
 *						the same key-data pair in or more of their nodes, whichever tree has the least number of the
 *						duplicates have precedence.
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::operator|(const rbTree<K,D> &rbt) const {
	return orBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes are equal
				// If so, save it and continues saving as long as their more similar nodes in both trees. Skip the remaining
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
					do {
						(*IdxM++) = *IdxS1;
					} while(((++IdxS1 < IdxE1) & (++IdxS2 < IdxE2)) && (*IdxS1)->equal(*IdxS2));

					IdxS1--;
					while(IdxS2 < IdxE2 && (*IdxS1)->equal(*IdxS2))
						IdxS2++;
					IdxS2--; IdxS1++;
					while(IdxS1 < IdxE1 && (*IdxS1)->equal(*IdxS2))
						IdxS1++;
					IdxS2++;
				} else if ((*IdxS1)->data < (*IdxS2)->data)
					*IdxM++ = *IdxS1++;
				else
					*IdxM++ = *IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = *IdxS2++;
    }, &rbTree<K,D>::dummy);
}

/*
 * function_identifier: Takes all nodes from nodes from two rbTrees and process them into a new rbTree to replace the current one
 *						For nodes with unique key-data pair unique between the trees, add them like normal. If two trees share
 *						the same key-data pair in or more of their nodes, whichever tree has the least number of the
 *						duplicates have precedence.
 * parameters: 			A red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::operator|=(const rbTree<K,D> &rbt) {
	return orBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes are equal
				// If so, save it and continues saving as long as their more similar nodes in both trees. Skip the remaining
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
					do {
						(*IdxM++) = *IdxS1;
					} while(((++IdxS1 < IdxE1) & (++IdxS2 < IdxE2)) && (*IdxS1)->equal(*IdxS2));

					IdxS1--;
					while(IdxS2 < IdxE2 && (*IdxS1)->equal(*IdxS2))
						IdxS2++;
					IdxS2--; IdxS1++;
					while(IdxS1 < IdxE1 && (*IdxS1)->equal(*IdxS2))
						delete *IdxS1++;
					IdxS2++;
				} else if ((*IdxS1)->data < (*IdxS2)->data)
					*IdxM++ = *IdxS1++;
				else
					(*IdxM++) = new rbtNode<K,D>(*IdxS2++);
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				(*IdxM++) = new rbtNode<K,D>(*IdxS2++);
    }, &rbTree<K,D>::dummy);
}

/*
 * function_identifier: The alias for the | operator
 * parameters: 			A red-black tree with compatible key and data to the current red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
inline rbTree<K,D> rbTree<K,D>::rbt_or(const rbTree<K,D> &rbt) const {
	return *this | rbt;
}

/*
 * function_identifier: The alias for the |= operator
 * parameters: 			A red-black tree with compatible key and data to the current red-black tree
 * return value:		This object
*/
template <typename K, typename D>
inline rbTree<K,D> rbTree<K,D>::rbt_orI(const rbTree<K,D> &rbt) {
	return *this |= rbt;
}

/*
 * function_identifier: Places all the nodes in both trees into a single resulting tree. No duplicate data/keys
 *						pairs.
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_orLeast(const rbTree<K,D> &rbt) const {
		// Calls the or template function
	return orBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes are equal
				// If so, save it. Skip the remaining
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
					(*IdxM++) = *IdxS1;

					IdxS2++;
					while(IdxS2 < IdxE2 && (*IdxS1)->equal(*IdxS2))
						IdxS2++;
					IdxS2--; IdxS1++;
					while(IdxS1 < IdxE1 && (*IdxS1)->equal(*IdxS2))
						IdxS1++;
					IdxS2++;
				} else if ((*IdxS1)->data < (*IdxS2)->data)
					*IdxM++ = *IdxS1++;
				else
					*IdxM++ = *IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = *IdxS2++;
    }, &rbTree<K,D>::rbt_removeDuplicates);
}

/*
 * function_identifier: Places all the nodes in the given trees into the current tree. No duplicate data/keys
 *						pairs in the final result.
 * parameters: 			A red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_orLeastI(const rbTree<K,D> &rbt) {
		// Calls the orI template function
	return orBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes are equal
				// If so, save it. Skip the remaining
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
					(*IdxM++) = *IdxS1;

					IdxS2++;
					while(IdxS2 < IdxE2 && (*IdxS1)->equal(*IdxS2))
						IdxS2++;
					IdxS2--; IdxS1++;
					while(IdxS1 < IdxE1 && (*IdxS1)->equal(*IdxS2))
						delete *IdxS1++;
					IdxS2++;

				} else if ((*IdxS1)->data < (*IdxS2)->data)
					*IdxM++ = *IdxS1++;
				else
					(*IdxM++) = new rbtNode<K,D>(*IdxS2++);
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				(*IdxM++) = new rbtNode<K,D>(*IdxS2++);
    }, &rbTree<K,D>::rbt_removeDuplicates);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree
 *						For nodes with unique key-data pair between both trees, add them like normal. If two trees share
 *						nodes with the same key-data pair in or more of their nodes, whichever tree has the most number of
 *						duplicates takes precedence.
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_orMost(const rbTree<K,D> &rbt) const {
		// Calls the or template function
	return orBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes are equal
				// If so, save it until no more similar nodes are found in either tree
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
					(*IdxM++) = *IdxS1++; IdxS2++;
				} else if ((*IdxS1)->data < (*IdxS2)->data)
					*IdxM++ = *IdxS1++;
				else
					*IdxM++ = *IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = *IdxS2++;
    }, &rbTree<K,D>::dummy);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree to replace the current one
 *						For nodes with unique key-data pair between both trees, add them like normal. If two trees share
 *						nodes with the same key-data pair in or more of their nodes, whichever tree has the most number of
 *						duplicates takes precedence.
 * parameters: 			A red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_orMostI(const rbTree<K,D> &rbt) {
		// Calls the orI template function
	return orBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes are equal
				// If so, save it until no more similar nodes are found in either tree
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
					(*IdxM++) = *IdxS1++; IdxS2++;
				} else if ((*IdxS1)->data < (*IdxS2)->data)
					*IdxM++ = *IdxS1++;
				else
					(*IdxM++) = new rbtNode<K,D>(*IdxS2++);
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				(*IdxM++) = new rbtNode<K,D>(*IdxS2++);
    }, &rbTree<K,D>::dummy);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree
 *						For nodes with keys unique between the trees, add them like normal. If two trees share
 *						the same key in or more of their nodes, whichever tree has the least number of the
 *						duplicates have precedence.
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_orKey(const rbTree<K,D> &rbt) const {
	return orBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, save it and continues saving as long as their more similar nodes in both trees. Skip the remaining
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				do {
					(*IdxM++) = *IdxS1;
				} while(((++IdxS1 < IdxE1) & (++IdxS2 < IdxE2)) && (*IdxS1)->key == (*IdxS2)->key);

				IdxS1--;
				while(IdxS2 < IdxE2 && (*IdxS1)->key == (*IdxS2)->key)
					IdxS2++;
				IdxS2--; IdxS1++;
				while(IdxS1 < IdxE1 && (*IdxS1)->key == (*IdxS2)->key)
					IdxS1++;
				IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = *IdxS2++;
    }, &rbTree<K,D>::dummy);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree to replace the current one
 *						For nodes with keys unique between the trees, add them like normal. If two trees share
 *						the same key in or more of their nodes, whichever tree has the least number of the
 *						duplicates have precedence.
 * parameters: 			A red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_orKeyI(const rbTree<K,D> &rbt) {
	return orBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, save it and continues saving as long as their more similar nodes in both trees. Skip the remaining
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				do {
					(*IdxM++) = *IdxS1;
				} while(((++IdxS1 < IdxE1) & (++IdxS2 < IdxE2)) && (*IdxS1)->key == (*IdxS2)->key);

				IdxS1--;
				while(IdxS2 < IdxE2 && (*IdxS1)->key == (*IdxS2)->key)
					IdxS2++;
				IdxS2--; IdxS1++;
				while(IdxS1 < IdxE1 && (*IdxS1)->key == (*IdxS2)->key)
					delete *IdxS1++;
				IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				(*IdxM++) = new rbtNode<K,D>(*IdxS2++);
    }, &rbTree<K,D>::dummy);
}

/*
 * function_identifier: Places all the nodes in both trees into a single resulting tree. No duplicate keys
 *						pairs.
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_orKeyLeast(const rbTree<K,D> &rbt) const {
		// Calls the or template function
	return orBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, save it. Skip the remaining
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				(*IdxM++) = *IdxS1;

				IdxS2++;
				while(IdxS2 < IdxE2 && (*IdxS1)->key == (*IdxS2)->key)
					IdxS2++;
				IdxS2--; IdxS1++;
				while(IdxS1 < IdxE1 && (*IdxS1)->key == (*IdxS2)->key)
					IdxS1++;
				IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = *IdxS2++;
    }, &rbTree<K,D>::rbt_removeDuplicateKeys);
}

/*
 * function_identifier: Places all the nodes in the given trees into the current tree. No duplicate keys
 *						pairs in the final result.
 * parameters: 			A red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_orKeyLeastI(const rbTree<K,D> &rbt) {
		// Calls the orI template function
	return orBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, save it. Skip the remaining
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				(*IdxM++) = *IdxS1;

				IdxS2++;
				while(IdxS2 < IdxE2 && (*IdxS1)->key == (*IdxS2)->key)
					IdxS2++;
				IdxS2--; IdxS1++;
				while(IdxS1 < IdxE1 && (*IdxS1)->key == (*IdxS2)->key)
					delete *IdxS1++;
				IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				(*IdxM++) = new rbtNode<K,D>(*IdxS2++);
    }, &rbTree<K,D>::rbt_removeDuplicateKeys);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree
 *						For nodes with unique keys between both trees, add them like normal. If two trees share
 *						nodes with the same key in or more of their nodes, whichever tree has the most number
 *						of duplicates takes precedence.
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_orKeyMost(const rbTree<K,D> &rbt) const {
		// Calls the or template function
	return orBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, save it until no more similar nodes are found in either tree
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				(*IdxM++) = *IdxS1++; IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = *IdxS2++;
    }, &rbTree<K,D>::dummy);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree to replace the current one
 *						For nodes with unique keys between both trees, add them like normal. If two trees share
 *						nodes with the same key in or more of their nodes, whichever tree has the most number
 *						of duplicates takes precedence.
 * parameters: 			A red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_orKeyMostI(const rbTree<K,D> &rbt) {
		// Calls the orI template function
	return orBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, save it until no more similar nodes are found in either tree
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				(*IdxM++) = *IdxS1++; IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				(*IdxM++) = new rbtNode<K,D>(*IdxS2++);
    }, &rbTree<K,D>::dummy);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree
 *						For nodes with data unique between the trees, add them like normal. If two trees share
 *						the same data in or more of their nodes, whichever tree has the least number of the
 *						duplicates have precedence.
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_orData(const rbTree<K,D> &rbt) const {
	return orBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal data
				// If so, save it and continues saving as long as their more similar nodes in both trees. Skip the remaining
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
				do {
					(*IdxM++) = *IdxS1;
				} while(((++IdxS1 < IdxE1) & (++IdxS2 < IdxE2)) && (*IdxS1)->data == (*IdxS2)->data);

				IdxS1--;
				while(IdxS2 < IdxE2 && (*IdxS1)->data == (*IdxS2)->data)
					IdxS2++;
				IdxS2--; IdxS1++;
				while(IdxS1 < IdxE1 && (*IdxS1)->data == (*IdxS2)->data)
					IdxS1++;
				IdxS2++;
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = *IdxS2++;
    }, &rbTree<K,D>::dummy, true);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree to replace the current one
 *						For nodes with data unique between the trees, add them like normal. If two trees share
 *						the same data in or more of their nodes, whichever tree has the least number of the
 *						duplicates have precedence.
 * parameters: 			A red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_orDataI(const rbTree<K,D> &rbt) {
	return orBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal data
				// If so, save it and continues saving as long as their more similar nodes in both trees. Skip the remaining
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
				do {
					(*IdxM++) = *IdxS1;
				} while(((++IdxS1 < IdxE1) & (++IdxS2 < IdxE2)) && (*IdxS1)->data == (*IdxS2)->data);

				IdxS1--;
				while(IdxS2 < IdxE2 && (*IdxS1)->data == (*IdxS2)->data)
					IdxS2++;
				IdxS2--; IdxS1++;
				while(IdxS1 < IdxE1 && (*IdxS1)->data == (*IdxS2)->data)
					delete *IdxS1++;
				IdxS2++;
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				*IdxM++ = *IdxS1++;
			else
				(*IdxM++) = new rbtNode<K,D>(*IdxS2++);
    }, &rbTree<K,D>::dummy, true);
}

/*
 * function_identifier: Places all the nodes in both trees into a single resulting tree. No duplicate data
 *						pairs.
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_orDataLeast(const rbTree<K,D> &rbt) const {
		// Calls the or template function
	return orBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal data
				// If so, save it. Skip the remaining
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
				(*IdxM++) = *IdxS1;

				IdxS2++;
				while(IdxS2 < IdxE2 && (*IdxS1)->data == (*IdxS2)->data)
					IdxS2++;
				IdxS2--; IdxS1++;
				while(IdxS1 < IdxE1 && (*IdxS1)->data == (*IdxS2)->data)
					IdxS1++;
				IdxS2++;
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = *IdxS2++;
    }, &rbTree<K,D>::rbt_removeDuplicateData, true);
}

/*
 * function_identifier: Places all the nodes in the given trees into the current tree. No duplicate data
 *						pairs in the final result.
 * parameters: 			A red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_orDataLeastI(const rbTree<K,D> &rbt) {
		// Calls the orI template function
	return orBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal data
				// If so, save it. Skip the remaining
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
				(*IdxM++) = *IdxS1;

				IdxS2++;
				while(IdxS2 < IdxE2 && (*IdxS1)->data == (*IdxS2)->data)
					IdxS2++;
				IdxS2--; IdxS1++;
				while(IdxS1 < IdxE1 && (*IdxS1)->data == (*IdxS2)->data)
					delete *IdxS1++;
				IdxS2++;
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				*IdxM++ = *IdxS1++;
			else
				(*IdxM++) = new rbtNode<K,D>(*IdxS2++);
    }, &rbTree<K,D>::rbt_removeDuplicateData, true);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree
 *						For nodes with unique data between both trees, add them like normal. If two trees share
 *						nodes with the same data in or more of their nodes, whichever tree has the most number
 *						of duplicates takes precedence.
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_orDataMost(const rbTree<K,D> &rbt) const {
		// Calls the or template function
	return orBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal data
				// If so, save it until no more similar nodes are found in either tree
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
				(*IdxM++) = *IdxS1++; IdxS2++;
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = *IdxS2++;
    }, &rbTree<K,D>::dummy, true);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree to replace the current one
 *						For nodes with unique data between both trees, add them like normal. If two trees share
 *						nodes with the same data in or more of their nodes, whichever tree has the most number
 *						of duplicates takes precedence.
 * parameters: 			A red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_orDataMostI(const rbTree<K,D> &rbt) {
		// Calls the orI template function
	return orBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal data
				// If so, save it until no more similar nodes are found in either tree
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
				(*IdxM++) = *IdxS1++; IdxS2++;
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				*IdxM++ = *IdxS1++;
			else
				(*IdxM++) = new rbtNode<K,D>(*IdxS2++);
    }, &rbTree<K,D>::dummy, true);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree
 *						For nodes with the same key/data pair between both trees, disclude them. Else
 *						add them like normal. Each node, from one tree, may cancel at most one other
 *						node from another tree.
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::operator^(const rbTree<K,D> &rbt) const {
		// Calls the or template function
	return orBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal key/data
				// If so, skip them
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
						// Skips this node
					IdxS2++; IdxS1++;
				} else if ((*IdxS1)->data < (*IdxS2)->data)
					*IdxM++ = *IdxS1++;
				else
					*IdxM++ = *IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = *IdxS2++;
    }, &rbTree<K,D>::dummy, false, true);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree to replace the current one
 *						For nodes with the same key/data pair between both trees, disclude them. Else
 *						add them like normal. Each node, from one tree, may cancel at most one other
 *						node from another tree.
 * parameters: 			A red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::operator^=(const rbTree<K,D> &rbt) {
		// Calls the orI template function
	return orBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal key/data
				// If so, skip/delete them
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
						// Skips/deletes this node
					IdxS2++; delete *IdxS1++;
				} else if ((*IdxS1)->data < (*IdxS2)->data)
					*IdxM++ = *IdxS1++;
				else
					*IdxM++ = new rbtNode<K,D>(*IdxS2++);
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = new rbtNode<K,D>(*IdxS2++);
    }, &rbTree<K,D>::dummy, false, true);
}

/*
 * function_identifier: The alias for the ^ operator
 * parameters: 			A red-black tree with compatible key and data to the current red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
inline rbTree<K,D> rbTree<K,D>::rbt_xor(const rbTree<K,D> &rbt) const {
	return *this ^ rbt;
}

/*
 * function_identifier: The alias for the ^= operator
 * parameters: 			A red-black tree with compatible key and data to the current red-black tree
 * return value:		This object
*/
template <typename K, typename D>
inline rbTree<K,D> rbTree<K,D>::rbt_xorI(const rbTree<K,D> &rbt) {
	return *this ^= rbt;
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree
 *						For nodes with the same key/data pair between both trees, disclude them. Else
 *						add them like normal. Each node, from one tree, may cancel at most one other
 *						node from another tree. No duplicates
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_xorLeast(const rbTree<K,D> &rbt) const {
		// Calls the or template function
	return orBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal key/data
				// If so, skip them. If one tree has more duplicates than the other
				// tree, save one and skip the rest.
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
						// Saves the expected key/data pair
					K dupKey = (*IdxS1)->key; D dupData = (*IdxS1)->data;

						// Skips all nodes of same value. Only stops when no more nodes in
						// one of the two arrays, or if a different key/data pair is found.
					while(((++IdxS1 < IdxE1) & (++IdxS2 < IdxE2)) && (*IdxS1)->key == dupKey && (*IdxS1)->data == dupData && (*IdxS2)->key == dupKey && (*IdxS2)->data == dupData);

						// If more of the same key/data pair nodes found on IdxS1's nodes,
						// save the first and skip the rest
					if (IdxS1 < IdxE1 && (*IdxS1)->key == dupKey && (*IdxS1)->data == dupData) {
						*IdxM++ = *IdxS1;
						while(++IdxS1 < IdxE1 && (*IdxS1)->key == dupKey && (*IdxS1)->data == dupData);
					}
						// If more of the same key/data pair nodes found on IdxS2's nodes,
						// save the first and skip the rest
					else if (IdxS2 < IdxE2 && (*IdxS2)->key == dupKey && (*IdxS2)->data == dupData) {
						*IdxM++ = *IdxS2;
						while(++IdxS2 < IdxE2 && (*IdxS2)->key == dupKey && (*IdxS2)->data == dupData);
					}
				} else if ((*IdxS1)->data < (*IdxS2)->data)
					*IdxM++ = *IdxS1++;
				else
					*IdxM++ = *IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = *IdxS2++;
    }, &rbTree<K,D>::rbt_removeDuplicates, false, true);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree to replace the current one
 *						For nodes with the same key/data pair between both trees, disclude them. Else
 *						add them like normal. Each node, from one tree, may cancel at most one other
 *						node from another tree. No duplicates		
 * parameters: 			A red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_xorLeastI(const rbTree<K,D> &rbt) {
		// Calls the orI template function
	return orBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal key/data
				// If so, skip/delete them. If one tree has more duplicates than the other
				// tree, save one and skip/delete the rest.
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
						// Saves the expected key/data pair
					K dupKey = (*IdxS1)->key; D dupData = (*IdxS1)->data;

						// Skips all nodes of same value. Deletes them in IdxS1. Only stops
						// when no more nodes in one of the two arrays, or if a different
						// key/data pair is found.
					do {
						delete *IdxS1++;
					} while(((++IdxS1 < IdxE1) & (++IdxS2 < IdxE2)) && (*IdxS1)->key == dupKey && (*IdxS1)->data == dupData && (*IdxS2)->key == dupKey && (*IdxS2)->data == dupData);

						// If more of the same key/data pair nodes found on IdxS1's nodes,
						// save the first and delete the rest
					if (IdxS1 < IdxE1 && (*IdxS1)->key == dupKey && (*IdxS1)->data == dupData) {
						*IdxM++ = *IdxS1++;
						do {
							delete *IdxS1++;
						} while(IdxS1 < IdxE1 && (*IdxS1)->key == dupKey && (*IdxS1)->data == dupData);
					}
						// If more of the same key/data pair nodes found on IdxS2's nodes,
						// save the first and skip the rest
					else if (IdxS2 < IdxE2 && (*IdxS2)->key == dupKey && (*IdxS2)->data == dupData) {
						*IdxM++ = new rbtNode<K,D>(*IdxS2++);
						while(++IdxS2 < IdxE2 && (*IdxS2)->key == dupKey && (*IdxS2)->data == dupData);
					}
				} else if ((*IdxS1)->data < (*IdxS2)->data)
					*IdxM++ = *IdxS1++;
				else
					*IdxM++ = new rbtNode<K,D>(*IdxS2++);
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = new rbtNode<K,D>(*IdxS2++);
    }, &rbTree<K,D>::rbt_removeDuplicates, false, true);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree
 *						For nodes with the same key/data pair between both trees, disclude them. Else
 *						add them like normal. Each node, from one tree, may cancel any number of nodes
 *						from another tree.
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_xorMost(const rbTree<K,D> &rbt) const {
		// Calls the or template function
	return orBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal key/data
				// If so, skip all similar nodes in both trees
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
						// Skips all nodes, in IdxS2, duplicate to node found
						// in IdxS1
					while(++IdxS2 < IdxE2 && (*IdxS1)->equal(*IdxS2));
					IdxS2--;
						// Skips all nodes, in IdxS1, duplicate to node found
						// in IdxS2
					while(++IdxS1 < IdxE1 && (*IdxS1)->equal(*IdxS2));
					IdxS2++;
				} else if ((*IdxS1)->data < (*IdxS2)->data)
					*IdxM++ = *IdxS1++;
				else
					*IdxM++ = *IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = *IdxS2++;
    }, &rbTree<K,D>::dummy, false, true);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree to replace the current one
 *						For nodes with the same key/data pair between both trees, disclude them. Else
 *						add them like normal. Each node, from one tree, may cancel any number of nodes
 *						from another tree.
 * parameters: 			A red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_xorMostI(const rbTree<K,D> &rbt) {
		// Calls the orI template function
	return orBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal key/data
				// If so, skip/delete all similar nodes in both trees
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
						// Skips all nodes, in IdxS2, duplicate to node found
						// in IdxS1
					while(++IdxS2 < IdxE2 && (*IdxS1)->equal(*IdxS2));
					IdxS2--;
						// Deletes all nodes, in IdxS1, duplicate to node found
						// in IdxS2
					do {
						delete *IdxS1++;
					} while(IdxS1 < IdxE1 && (*IdxS1)->equal(*IdxS2));
					IdxS2++;
				} else if ((*IdxS1)->data < (*IdxS2)->data)
					*IdxM++ = *IdxS1++;
				else
					*IdxM++ = new rbtNode<K,D>(*IdxS2++);
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = new rbtNode<K,D>(*IdxS2++);
    }, &rbTree<K,D>::dummy, false, true);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree
 *						For nodes with the same keys between both trees, disclude them. Else
 *						add them like normal. Each node, from one tree, may cancel at most one other
 *						node from another tree.
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_xorKey(const rbTree<K,D> &rbt) const {
		// Calls the or template function
	return orBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, skip them
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
					// Skips this node
				IdxS2++; IdxS1++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = *IdxS2++;
    }, &rbTree<K,D>::dummy, false, true);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree to replace the current one
 *						For nodes with the same keys between both trees, disclude them. Else
 *						add them like normal. Each node, from one tree, may cancel at most one other
 *						node from another tree.
 * parameters: 			A red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_xorKeyI(const rbTree<K,D> &rbt) {
		// Calls the orI template function
	return orBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, skip/delete them
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
					// Skips/deletes this node
				IdxS2++; delete *IdxS1++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = new rbtNode<K,D>(*IdxS2++);
    }, &rbTree<K,D>::dummy, false, true);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree
 *						For nodes with the same keys between both trees, disclude them. Else
 *						add them like normal. Each node, from one tree, may cancel at most one other
 *						node from another tree. No duplicates
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_xorKeyLeast(const rbTree<K,D> &rbt) const {
		// Calls the or template function
	return orBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, skip them. If one tree has more duplicates than the other
				// tree, save one and skip the rest.
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
					// Saves the expected key
				K dupKey = (*IdxS1)->key;

					// Skips all nodes of same value. Only stops when no more nodes in
					// one of the two arrays, or if a different key is found.
				while(((++IdxS1 < IdxE1) & (++IdxS2 < IdxE2)) && (*IdxS1)->key == dupKey && (*IdxS2)->key == dupKey);

					// If more nodes with the same key found on IdxS1's nodes,
					// save the first and skip the rest
				if (IdxS1 < IdxE1 && (*IdxS1)->key == dupKey) {
					*IdxM++ = *IdxS1;
					while(++IdxS1 < IdxE1 && (*IdxS1)->key == dupKey);
				}
					// If more nodes with the same key found on IdxS2's nodes,
					// save the first and skip the rest
				else if (IdxS2 < IdxE2 && (*IdxS2)->key == dupKey) {
					*IdxM++ = *IdxS2;
					while(++IdxS2 < IdxE2 && (*IdxS2)->key == dupKey);
				}
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = *IdxS2++;
    }, &rbTree<K,D>::rbt_removeDuplicates, false, true);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree to replace the current one
 *						For nodes with the same keys between both trees, disclude them. Else
 *						add them like normal. Each node, from one tree, may cancel at most one other
 *						node from another tree. No duplicates		
 * parameters: 			A red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_xorKeyLeastI(const rbTree<K,D> &rbt) {
		// Calls the orI template function
	return orBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, skip/delete them. If one tree has more duplicates than the other
				// tree, save one and skip/delete the rest.
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
					// Saves the expected key
				K dupKey = (*IdxS1)->key;;

					// Skips all nodes of same value. Deletes them in IdxS1. Only stops
					// when no more nodes in one of the two arrays, or if a different
					// key is found.
				do {
					delete *IdxS1;
				} while(((++IdxS1 < IdxE1) & (++IdxS2 < IdxE2)) && (*IdxS1)->key == dupKey && (*IdxS2)->key == dupKey);

					// If more nodes with the same key found on IdxS1's nodes,
					// save the first and delete the rest
				if (IdxS1 < IdxE1 && (*IdxS1)->key == dupKey) {
					*IdxM++ = *IdxS1++;
					do {
						delete *IdxS1++;
					} while(IdxS1 < IdxE1 && (*IdxS1)->key == dupKey);
				}
					// If more nodes with the same key found on IdxS2's nodes,
					// save the first and skip the rest
				else if (IdxS2 < IdxE2 && (*IdxS2)->key == dupKey) {
					*IdxM++ = new rbtNode<K,D>(*IdxS2++);
					while(++IdxS2 < IdxE2 && (*IdxS2)->key == dupKey);
				}
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = new rbtNode<K,D>(*IdxS2++);
    }, &rbTree<K,D>::rbt_removeDuplicates, false, true);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree
 *						For nodes with the same keys between both trees, disclude them. Else
 *						add them like normal. Each node, from one tree, may cancel any number of nodes
 *						from another tree.
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_xorKeyMost(const rbTree<K,D> &rbt) const {
		// Calls the or template function
	return orBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, skip all similar nodes in both trees
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
					// Skips all nodes, in IdxS2, with duplicate key to
					// node found in IdxS1
				while(++IdxS2 < IdxE2 && (*IdxS1)->key == (*IdxS2)->key);
				IdxS2--;
					// Skips all nodes, in IdxS1, with duplicate key to
					// node found in IdxS2
				while(++IdxS1 < IdxE1 && (*IdxS1)->key == (*IdxS2)->key);
				IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = *IdxS2++;
    }, &rbTree<K,D>::dummy, false, true);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree to replace the current one
 *						For nodes with the same keys between both trees, disclude them. Else
 *						add them like normal. Each node, from one tree, may cancel any number of nodes
 *						from another tree.
 * parameters: 			A red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_xorKeyMostI(const rbTree<K,D> &rbt) {
		// Calls the orI template function
	return orBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, skip/delete all similar nodes in both trees
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
					// Skips all nodes, in IdxS2, with duplicate key to
					// node found in IdxS1
				while(++IdxS2 < IdxE2 && (*IdxS1)->key == (*IdxS2)->key);
				IdxS2--;
					// Deletes all nodes, in IdxS2, with duplicate key to
					// node found in IdxS1
				do {
					delete *IdxS1++;
				} while(IdxS1 < IdxE1 && (*IdxS1)->key == (*IdxS2)->key);
				IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = new rbtNode<K,D>(*IdxS2++);
    }, &rbTree<K,D>::dummy, false, true);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree
 *						For nodes with the same data between both trees, disclude them. Else
 *						add them like normal. Each node, from one tree, may cancel at most one other
 *						node from another tree.
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_xorData(const rbTree<K,D> &rbt) const {
		// Calls the or template function
	return orBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal data
				// If so, skip them
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
					// Skips this node
				IdxS2++; IdxS1++;
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = *IdxS2++;
    }, &rbTree<K,D>::dummy, true, true);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree to replace the current one
 *						For nodes with the same data between both trees, disclude them. Else
 *						add them like normal. Each node, from one tree, may cancel at most one other
 *						node from another tree.
 * parameters: 			A red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_xorDataI(const rbTree<K,D> &rbt) {
		// Calls the orI template function
	return orBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal data
				// If so, skip/delete them
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
					// Skips/deletes this node
				IdxS2++; delete *IdxS1++;
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = new rbtNode<K,D>(*IdxS2++);
    }, &rbTree<K,D>::dummy, true, true);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree
 *						For nodes with the same data between both trees, disclude them. Else
 *						add them like normal. Each node, from one tree, may cancel at most one other
 *						node from another tree. No duplicates
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_xorDataLeast(const rbTree<K,D> &rbt) const {
		// Calls the or template function
	return orBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal data
				// If so, skip them. If one tree has more duplicates than the other
				// tree, save one and skip the rest.
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
					// Saves the expected data
				D dupData = (*IdxS1)->data;

					// Skips all nodes of same value. Only stops when no more nodes in
					// one of the two arrays, or if a different data is found.
				while(((++IdxS1 < IdxE1) & (++IdxS2 < IdxE2)) && (*IdxS1)->data == dupData && (*IdxS2)->data == dupData);

					// If more nodes with the same data found on IdxS1's nodes,
					// save the first and skip the rest
				if (IdxS1 < IdxE1 && (*IdxS1)->data == dupData) {
					*IdxM++ = *IdxS1;
					while(++IdxS1 < IdxE1 && (*IdxS1)->data == dupData);
				}
					// If more nodes with the same data found on IdxS2's nodes,
					// save the first and skip the rest
				else if (IdxS2 < IdxE2 && (*IdxS2)->data == dupData) {
					*IdxM++ = *IdxS2;
					while(++IdxS2 < IdxE2 && (*IdxS2)->data == dupData);
				}
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = *IdxS2++;
    }, &rbTree<K,D>::rbt_removeDuplicates, true, true);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree to replace the current one
 *						For nodes with the same data between both trees, disclude them. Else
 *						add them like normal. Each node, from one tree, may cancel at most one other
 *						node from another tree. No duplicates		
 * parameters: 			A red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_xorDataLeastI(const rbTree<K,D> &rbt) {
		// Calls the orI template function
	return orBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal data
				// If so, skip/delete them. If one tree has more duplicates than the other
				// tree, save one and skip/delete the rest.
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
					// Saves the expected data
				D dupData = (*IdxS1)->data;;

					// Skips all nodes of same value. Deletes them in IdxS1. Only stops
					// when no more nodes in one of the two arrays, or if a different
					// data is found.
				do {
					delete *IdxS1;
				} while(((++IdxS1 < IdxE1) & (++IdxS2 < IdxE2)) && (*IdxS1)->data == dupData && (*IdxS2)->data == dupData);

					// If more nodes with the same data found on IdxS1's nodes,
					// save the first and delete the rest
				if (IdxS1 < IdxE1 && (*IdxS1)->data == dupData) {
					*IdxM++ = *IdxS1++;
					do {
						delete *IdxS1++;
					} while(IdxS1 < IdxE1 && (*IdxS1)->data == dupData);
				}
					// If more nodes with the same data found on IdxS2's nodes,
					// save the first and skip the rest
				else if (IdxS2 < IdxE2 && (*IdxS2)->data == dupData) {
					*IdxM++ = new rbtNode<K,D>(*IdxS2++);
					while(++IdxS2 < IdxE2 && (*IdxS2)->data == dupData);
				}
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = new rbtNode<K,D>(*IdxS2++);
    }, &rbTree<K,D>::rbt_removeDuplicates, true, true);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree
 *						For nodes with the same data between both trees, disclude them. Else
 *						add them like normal. Each node, from one tree, may cancel any number of nodes
 *						from another tree.
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_xorDataMost(const rbTree<K,D> &rbt) const {
		// Calls the or template function
	return orBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal data
				// If so, skip all similar nodes in both trees
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
					// Skips all nodes, in IdxS2, with duplicate data to
					// node found in IdxS1
				while(++IdxS2 < IdxE2 && (*IdxS1)->data == (*IdxS2)->data);
				IdxS2--;
					// Skips all nodes, in IdxS1, with duplicate data to
					// node found in IdxS2
				while(++IdxS1 < IdxE1 && (*IdxS1)->data == (*IdxS2)->data);
				IdxS2++;
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = *IdxS2++;
    }, &rbTree<K,D>::dummy, true, true);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree to replace the current one
 *						For nodes with the same data between both trees, disclude them. Else
 *						add them like normal. Each node, from one tree, may cancel any number of nodes
 *						from another tree.
 * parameters: 			A red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_xorDataMostI(const rbTree<K,D> &rbt) {
		// Calls the orI template function
	return orBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal data
				// If so, skip/delete all similar nodes in both trees
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
					// Skips all nodes, in IdxS2, with duplicate data to
					// node found in IdxS1
				while(++IdxS2 < IdxE2 && (*IdxS1)->data == (*IdxS2)->data);
				IdxS2--;
					// Deletes all nodes, in IdxS2, with duplicate data to
					// node found in IdxS1
				do {
					delete *IdxS1++;
				} while(IdxS1 < IdxE1 && (*IdxS1)->data == (*IdxS2)->data);
				IdxS2++;
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = new rbtNode<K,D>(*IdxS2++);
    }, &rbTree<K,D>::dummy, true, true);
}

/*
 * function_identifier: The base template for the and opperations
 * parameters: 			A red-black tree and a function to copy over nodes
 * return value:		A red-black tree
*/
template <typename K, typename D>
template <typename F1, typename F2>
inline rbTree<K,D> rbTree<K,D>::andBase(const rbTree<K,D> &rbt, F1 foo, F2 bar, bool data) const {
		// Ignores the trival case
	if (!size || !rbt.size)
		return rbTree<K,D>();
	if (&rbt == this) {
		rbTree<K,D> ret = *this;
		(ret.*bar)();
		return ret;
	}

		// Initializes the needed values
	rbtNode<K,D> **IdxS1, **IdxS2, **IdxE1, **IdxE2, **IdxM;

		// Allocates the arrays and get all the nodes from both trees
	IdxS1 = new rbtNode<K,D> *[size >= rbt.size ? size : rbt.size];
	IdxM = rbt_getAllNodes(IdxS1); IdxS2 = rbt.rbt_getAllNodes();
	IdxE1 = IdxS1 + size; IdxE2 = IdxS2 + rbt.size; 

		// If we are doing an 'and' based on data, sort nodes by data
	if (data) {
		fooPtr<K,D> = dataCompair;
		mergeSortCallerBase(IdxS1, 0, size - 1);
		mergeSortCallerBase(IdxS2, 0, rbt.size - 1);
	}

		// Cycles through the entire node array
	foo(IdxS1, IdxS2, IdxE1, IdxE2, IdxM);

		// Clears the second tree node array
	delete [] (IdxE2 - rbt.size);

		// Updates IdxS1 to start again for easy access
	IdxS1 = IdxE1 - size;

		// Calculates the new size
	size_t sizeNew = (IdxM - IdxS1);

		// Create a tree from nodes
	rbTree<K,D> rbtNew = rbTree<K,D>();
	rbtNew.size = sizeNew;

	if (sizeNew) {
			// If we are doing an 'and' based on data, restore final nodes by key
		if (data) {
			fooPtr<K,D> = keyCompair;
			mergeSortCallerBase(IdxS1, 0, size - 1);
		}

			// If there are remaining nodes, place them into the new tree
		rbtNew.treeify(0, sizeNew-1, IdxS1, &rbtNew.root);
	}

		// Deletes the remaining array
	delete [] IdxS1;

		// Return the finished tree
	return rbtNew;
}

/*
 * function_identifier: The base template for the andI opperations
 * parameters: 			A red-black tree and a function to copy over nodes
 * return value:		A red-black tree
*/
template <typename K, typename D>
template <typename F1, typename F2>
inline rbTree<K,D> rbTree<K,D>::andBaseI(const rbTree<K,D> &rbt, F1 foo, F2 bar, bool data) {
		// Ignores the trival case
	if (!size || !rbt.size) {
		rbt_clear();
		return *this;
	}
	if (&rbt == this) {
		(this->*bar)();
		return *this;
	}

		// Initializes the needed values
	rbtNode<K,D> **IdxM, **IdxS1, **IdxE1, **IdxS2, **IdxE2;

		// Allocates the arrays and get all the nodes from both trees
	IdxS1 = new rbtNode<K,D> *[size >= rbt.size ? size : rbt.size];
	IdxM = rbt_getAllNodes(IdxS1); IdxS2 = rbt.rbt_getAllNodes();
	IdxE1 = IdxS1 + size; IdxE2 = IdxS2 + rbt.size; 

		// If we are doing an 'and' based on data, sort nodes by data
	if (data) {
		fooPtr<K,D> = dataCompair;
		mergeSortCallerBase(IdxS1, 0, size - 1);
		mergeSortCallerBase(IdxS2, 0, rbt.size - 1);
	}

		// Cycles through the entire node array
	foo(IdxS1, IdxS2, IdxE1, IdxE2, IdxM);

		// Clears the second tree node array
	delete [] (IdxE2 - rbt.size);

		// Deletes any extra nodes
	while(IdxS1 < IdxE1) {
		delete *IdxS1;
		IdxS1++;
	}

		// Updates IdxS1 to start again for easy access
	IdxS1 = IdxE1 - size;

		// Calculates the new size
	size = (IdxM - IdxS1);

		// Makes the tree
	if (size) {
			// If we are doing an 'and' based on data, restore final nodes by key
		if (data) {
			fooPtr<K,D> = keyCompair;
			mergeSortCallerBase(IdxS1, 0, size - 1);
		}

		treeifyShallow(0, size - 1, IdxS1, &root);
		root->parent = NULL;
	} else
		root = NULL;

		// Deletes the remaining array
	delete [] IdxS1;

		// Returns this object
	return *this;
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree
 *						For nodes with the same key/data pair between both trees, include them. Else
 *						skip them. Each node, from one tree, may only be paired with one other
 *						from another tree
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::operator&(const rbTree<K,D> &rbt) const {
		// Calls the and template function
	return andBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2) {
				// Checks if both considered nodes have equal key/data
				// If so, save it and move forward in both arrays
				// If not, skip it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
					*IdxM = *IdxS1;
					IdxM++; IdxS1++; IdxS2++;
				} else if ((*IdxS1)->data < (*IdxS2)->data)
					IdxS1++;
				else
					IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				IdxS1++;
			else
				IdxS2++;
		}
    }, &rbTree<K,D>::dummy);
}
					
/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree to replace the current one
 *						For nodes with the same key/data pair between both trees, include them. Else
 *						skip them. Each node, from one tree, may only be paired with one other
 *						from another tree
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::operator&=(const rbTree<K,D> &rbt) {
		// Calls the andI template function
	return andBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal key/data
				// If so, save it and move forward in both arrays
				// If not, skip/delete it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
					*IdxM = *IdxS1;
					IdxM++; IdxS1++; IdxS2++;
				} else if ((*IdxS1)->data < (*IdxS2)->data) {
					delete *IdxS1;
					IdxS1++;
				} else
					IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key) {
				delete *IdxS1;
				IdxS1++;
			} else
				IdxS2++;
    }, &rbTree<K,D>::rbt_removeDuplicates);
}

/*
 * function_identifier: The alias for the & operator
 * parameters: 			A red-black tree with compatible key and data to the current red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
inline rbTree<K,D> rbTree<K,D>::rbt_and(const rbTree<K,D> &rbt) const {
	return (*this & rbt);
}

/*
 * function_identifier: The alias for the &= operator
 * parameters: 			A red-black tree with compatible key and data to the current red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
inline rbTree<K,D> rbTree<K,D>::rbt_andI(const rbTree<K,D> &rbt) {
	return (*this &= rbt);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree
 *						For nodes with the same key/data pair between both trees, include them. Else
 *						skip them. Each node, from one tree, may only be paired with one other
 *						from another tree. No duplicates
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andLeast(const rbTree<K,D> &rbt) const {
		// Calls the and template function
	return andBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal key/data
				// If so, save it and skip any following nodes similar key/data pairs
				// If not, skip it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
						// Saves the first node
					*IdxM++ = *IdxS1;

						// Skips all following similar nodes
					while(++IdxS2 < IdxE2 && (*IdxS2)->equal(*IdxS1));
					IdxS2--;
					while(++IdxS1 < IdxE1 && (*IdxS2)->equal(*IdxS1));
					IdxS2++;
				} else if ((*IdxS1)->data < (*IdxS2)->data)
					IdxS1++;
				else
					IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				IdxS1++;
			else
				IdxS2++;
    }, &rbTree<K,D>::dummy);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree to replace the current one
 *						For nodes with the same key/data pair between both trees, include them. Else
 *						skip them. Each node, from one tree, may only be paired with one other
 *						from another tree. No duplicates
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andLeastI(const rbTree<K,D> &rbt) {
		// Calls the andI template function
	return andBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal key/data
				// If so, save it and skip/delete any following nodes similar key/data pairs
				// If not, skip/delete it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
						// Saves the first node
					*IdxM++ = *IdxS1;

						// Skips/deletes all following similar nodes
					while(++IdxS2 < IdxE2 && (*IdxS2)->equal(*IdxS1));
					IdxS2--;
					while(++IdxS1 < IdxE1 && (*IdxS2)->equal(*IdxS1))
						delete *IdxS1;
					IdxS2++;
				} else if ((*IdxS1)->data < (*IdxS2)->data)
					delete *IdxS1++;
				else
					IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				delete *IdxS1++;
			else
				IdxS2++;
    }, &rbTree<K,D>::rbt_removeDuplicates);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree
 *						For nodes with the same key/data pair between both trees, include them. Else
 *						skip them. If two trees share nodes with the same key/data pair in one or more of their
 *						nodes, whichever tree has the most number of duplicates takes precedence.
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andMost(const rbTree<K,D> &rbt) const {
		// Calls the andMost template function
	return andBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				//
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
						// Saves template information
					K dupKey = (*IdxS1)->key; D dupData = (*IdxS1)->data;

						// Saves the first node
					*IdxM++ = *IdxS1;

						// Saves all following nodes so long as both nodes are the same as the template information
					while(((++IdxS1 < IdxE1) & (++IdxS2 < IdxE2)) && (*IdxS1)->key == dupKey && (*IdxS1)->data == dupData && (*IdxS2)->key == dupKey && (*IdxS2)->data == dupData)
						*IdxM++ = *IdxS1;
						// Continues to save nodes in first tree if there are still yet unsaved nodes with similar information as the template information
					while(IdxS1 < IdxE1 && (*IdxS1)->key == dupKey && (*IdxS1)->data == dupData)
						*IdxM++ = *IdxS1++;
						// Continues to save nodes in second tree if there are still yet unsaved nodes with similar information as the template information
					while(IdxS2 < IdxE2 && (*IdxS2)->key == dupKey && (*IdxS2)->data == dupData)
						*IdxM++ = *IdxS2++;
				} else if ((*IdxS1)->data < (*IdxS2)->data)
					IdxS1++;
				else
					IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				IdxS1++;
			else
				IdxS2++;
    }, &rbTree<K,D>::dummy, false);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree to replace the current one
 *						For nodes with the same key/data pair between both trees, include them. Else
 *						skip them. If two trees share nodes with the same key/data pair in one or more of their
 *						nodes, whichever tree has the most number of duplicates takes precedence.
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andMostI(const rbTree<K,D> &rbt) {
		// Calls the andMostI template function
	return andBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				//
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
						// Saves template information
					K dupKey = (*IdxS1)->key; D dupData = (*IdxS1)->data;

						// Saves the first node
					*IdxM++ = *IdxS1;

						// Saves all following nodes so long as both nodes are the same as the template information
					while(((++IdxS1 < IdxE1) & (++IdxS2 < IdxE2)) && (*IdxS1)->key == dupKey && (*IdxS1)->data == dupData && (*IdxS2)->key == dupKey && (*IdxS2)->data == dupData)
						*IdxM++ = *IdxS1;
						// Continues to save nodes in first tree if there are still yet unsaved nodes with similar information as the template information
					while(IdxS1 < IdxE1 && (*IdxS1)->key == dupKey && (*IdxS1)->data == dupData)
						*IdxM++ = *IdxS1++;
						// Continues to save nodes in second tree if there are still yet unsaved nodes with similar information as the template information
					while(IdxS2 < IdxE2 && (*IdxS2)->key == dupKey && (*IdxS2)->data == dupData)
						*IdxM++ = new rbtNode<K,D>(*IdxS2++);
				} else if ((*IdxS1)->data < (*IdxS2)->data) {
					delete *IdxS1;
					IdxS1++;
				} else
					IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key) {
				delete *IdxS1;
				IdxS1++;
			} else
				IdxS2++;
    }, &rbTree<K,D>::dummy, false);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree
 *						For nodes with the same keys between both trees, include them. Else
 *						skip them. Each node, from one tree, may only be paired with one other
 *						from another tree
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
inline rbTree<K,D> rbTree<K,D>::rbt_andKey(const rbTree<K,D> &rbt) const {
		// Calls the and template function
	return andBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2) {
				// Checks if both considered nodes have equal keys
				// If so, save it and move forward in both arrays
				// If not, skip it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				*IdxM = *IdxS1;
				IdxM++; IdxS1++; IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				IdxS1++;
			else
				IdxS2++;
		}
    }, &rbTree<K,D>::dummy);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree to replace the current one
 *						For nodes with the same keys between both trees, include them. Else
 *						skip them. Each node, from one tree, may only be paired with one other
 *						from another tree
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
inline rbTree<K,D> rbTree<K,D>::rbt_andKeyI(const rbTree<K,D> &rbt) {
		// Calls the andI template function
	return andBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, save it and move forward in both arrays
				// If not, skip/delete it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				*IdxM = *IdxS1;
				IdxM++; IdxS1++; IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key) {
				delete *IdxS1;
				IdxS1++;
			} else
				IdxS2++;
    }, &rbTree<K,D>::rbt_removeDuplicates);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree
 *						For nodes with the same keys between both trees, include them. Else
 *						skip them. Each node, from one tree, may only be paired with one other
 *						from another tree. No duplicates
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andKeyLeast(const rbTree<K,D> &rbt) const {
		// Calls the and template function
	return andBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, save it and skip any following nodes similar keys
				// If not, skip it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
					// Saves the first node
				*IdxM++ = *IdxS1;

					// Skips all following similar nodes
				while(++IdxS2 < IdxE2 && (*IdxS2)->key == (*IdxS1)->key);
				IdxS2--;
				while(++IdxS1 < IdxE1 && (*IdxS2)->key == (*IdxS1)->key);
				IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				IdxS1++;
			else
				IdxS2++;
    }, &rbTree<K,D>::dummy);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree to replace the current one
 *						For nodes with the same keys between both trees, include them. Else
 *						skip them. Each node, from one tree, may only be paired with one other
 *						from another tree. No duplicates
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andKeyLeastI(const rbTree<K,D> &rbt) {
		// Calls the andI template function
	return andBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, save it and skip/delete any following nodes similar keys
				// If not, skip/delete it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
					// Saves the first node
				*IdxM++ = *IdxS1;

					// Skips/deletes all following similar nodes
				while(++IdxS2 < IdxE2 && (*IdxS2)->key == (*IdxS1)->key);
				IdxS2--;
				while(++IdxS1 < IdxE1 && (*IdxS2)->key == (*IdxS1)->key)
					delete *IdxS1;
				IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				delete *IdxS1++;
			else
				IdxS2++;
    }, &rbTree<K,D>::rbt_removeDuplicates);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree
 *						For nodes with the same keys between both trees, include them. Else
 *						skip them. If two trees share nodes with the same keys in one or more of their
 *						nodes, whichever tree has the most number of duplicates takes precedence.
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andKeyMost(const rbTree<K,D> &rbt) const {
		// Calls the andMost template function
	return andBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, save it and the most similar following nodes possible to find in either tree
				// If not, skip it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
					// Saves template information
				K dupKey = (*IdxS1)->key;

					// Saves the first node
				*IdxM++ = *IdxS1;

					// Saves all following nodes so long as both nodes are the same as the template information
				while(((++IdxS1 < IdxE1) & (++IdxS2 < IdxE2)) && (*IdxS1)->key == dupKey && (*IdxS2)->key == dupKey)
					*IdxM++ = *IdxS1;
					// Continues to save nodes in first tree if there are still yet unsaved nodes with similar information as the template information
				while(IdxS1 < IdxE1 && (*IdxS1)->key == dupKey)
					*IdxM++ = *IdxS1++;
					// Continues to save nodes in second tree if there are still yet unsaved nodes with similar information as the template information
				while(IdxS2 < IdxE2 && (*IdxS2)->key == dupKey)
					*IdxM++ = *IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				IdxS1++;
			else
				IdxS2++;
    }, &rbTree<K,D>::dummy, false);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree to replace the current one
 *						For nodes with the same keys between both trees, include them. Else
 *						skip them. If two trees share nodes with the same keys in one or more of their
 *						nodes, whichever tree has the most number of duplicates takes precedence.
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andKeyMostI(const rbTree<K,D> &rbt) {
		// Calls the andMostI template function
	return andBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, save it and the most similar following nodes possible to find in either tree
				// If not, skip/delete it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
					// Saves template information
				K dupKey = (*IdxS1)->key;

					// Saves the first node
				*IdxM++ = *IdxS1;

					// Saves all following nodes so long as both nodes are the same as the template information
				while(((++IdxS1 < IdxE1) & (++IdxS2 < IdxE2)) && (*IdxS1)->key == dupKey && (*IdxS2)->key == dupKey)
					*IdxM++ = *IdxS1;
					// Continues to save nodes in first tree if there are still yet unsaved nodes with similar information as the template information
				while(IdxS1 < IdxE1 && (*IdxS1)->key == dupKey)
					*IdxM++ = *IdxS1++;
					// Continues to save nodes in second tree if there are still yet unsaved nodes with similar information as the template information
				while(IdxS2 < IdxE2 && (*IdxS2)->key == dupKey)
					*IdxM++ = new rbtNode<K,D>(*IdxS2++);
			} else if ((*IdxS1)->key < (*IdxS2)->key) {
				delete *IdxS1;
				IdxS1++;
			} else
				IdxS2++;
    }, &rbTree<K,D>::dummy, false);
}








/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree
 *						For nodes with the same data between both trees, include them. Else
 *						skip them. Each node, from one tree, may only be paired with one other
 *						from another tree
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
inline rbTree<K,D> rbTree<K,D>::rbt_andData(const rbTree<K,D> &rbt) const {
		// Calls the and template function
	return andBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2) {
				// Checks if both considered nodes have equal data
				// If so, save it and move forward in both arrays
				// If not, skip it and move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
				*IdxM = *IdxS1;
				IdxM++; IdxS1++; IdxS2++;
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				IdxS1++;
			else
				IdxS2++;
		}
    }, &rbTree<K,D>::dummy);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree to replace the current one
 *						For nodes with the same data between both trees, include them. Else
 *						skip them. Each node, from one tree, may only be paired with one other
 *						from another tree
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
inline rbTree<K,D> rbTree<K,D>::rbt_andDataI(const rbTree<K,D> &rbt) {
		// Calls the andI template function
	return andBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal data
				// If so, save it and move forward in both arrays
				// If not, skip/delete it and move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
				*IdxM = *IdxS1;
				IdxM++; IdxS1++; IdxS2++;
			} else if ((*IdxS1)->data < (*IdxS2)->data) {
				delete *IdxS1;
				IdxS1++;
			} else
				IdxS2++;
    }, &rbTree<K,D>::rbt_removeDuplicates);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree
 *						For nodes with the same data between both trees, include them. Else
 *						skip them. Each node, from one tree, may only be paired with one other
 *						from another tree. No duplicates
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andDataLeast(const rbTree<K,D> &rbt) const {
		// Calls the and template function
	return andBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal data
				// If so, save it and skip any following nodes similar data
				// If not, skip it and move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
					// Saves the first node
				*IdxM++ = *IdxS1;

					// Skips all following similar nodes
				while(++IdxS2 < IdxE2 && (*IdxS2)->data == (*IdxS1)->data);
				IdxS2--;
				while(++IdxS1 < IdxE1 && (*IdxS2)->data == (*IdxS1)->data);
				IdxS2++;
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				IdxS1++;
			else
				IdxS2++;
    }, &rbTree<K,D>::dummy);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree to replace the current one
 *						For nodes with the same data between both trees, include them. Else
 *						skip them. Each node, from one tree, may only be paired with one other
 *						from another tree. No duplicates
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andDataLeastI(const rbTree<K,D> &rbt) {
		// Calls the andI template function
	return andBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal data
				// If so, save it and skip/delete any following nodes similar data
				// If not, skip/delete it and move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
					// Saves the first node
				*IdxM++ = *IdxS1;

					// Skips/deletes all following similar nodes
				while(++IdxS2 < IdxE2 && (*IdxS2)->data == (*IdxS1)->data);
				IdxS2--;
				while(++IdxS1 < IdxE1 && (*IdxS2)->data == (*IdxS1)->data)
					delete *IdxS1;
				IdxS2++;
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				delete *IdxS1++;
			else
				IdxS2++;
    }, &rbTree<K,D>::rbt_removeDuplicates);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree
 *						For nodes with the same data between both trees, include them. Else
 *						skip them. If two trees share nodes with the same data in one or more of their
 *						nodes, whichever tree has the most number of duplicates takes precedence.
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andDataMost(const rbTree<K,D> &rbt) const {
		// Calls the andMost template function
	return andBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal data
				// If so, save it and the most similar following nodes possible to find in either tree
				// If not, skip it and move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
					// Saves template information
				K dupData = (*IdxS1)->data;

					// Saves the first node
				*IdxM++ = *IdxS1;

					// Saves all following nodes so long as both nodes are the same as the template information
				while(((++IdxS1 < IdxE1) & (++IdxS2 < IdxE2)) && (*IdxS1)->data == dupData && (*IdxS2)->data == dupData)
					*IdxM++ = *IdxS1;
					// Continues to save nodes in first tree if there are still yet unsaved nodes with similar information as the template information
				while(IdxS1 < IdxE1 && (*IdxS1)->data == dupData)
					*IdxM++ = *IdxS1++;
					// Continues to save nodes in second tree if there are still yet unsaved nodes with similar information as the template information
				while(IdxS2 < IdxE2 && (*IdxS2)->data == dupData)
					*IdxM++ = *IdxS2++;
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				IdxS1++;
			else
				IdxS2++;
    }, &rbTree<K,D>::dummy, false);
}

/*
 * function_identifier: Takes all nodes from two rbTrees and process them into a new rbTree to replace the current one
 *						For nodes with the same data between both trees, include them. Else
 *						skip them. If two trees share nodes with the same data in one or more of their
 *						nodes, whichever tree has the most number of duplicates takes precedence.
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andDataMostI(const rbTree<K,D> &rbt) {
		// Calls the andMostI template function
	return andBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal data
				// If so, save it and the most similar following nodes possible to find in either tree
				// If not, skip/delete it and move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
					// Saves template information
				K dupData = (*IdxS1)->data;

					// Saves the first node
				*IdxM++ = *IdxS1;

					// Saves all following nodes so long as both nodes are the same as the template information
				while(((++IdxS1 < IdxE1) & (++IdxS2 < IdxE2)) && (*IdxS1)->data == dupData && (*IdxS2)->data == dupData)
					*IdxM++ = *IdxS1;
					// Continues to save nodes in first tree if there are still yet unsaved nodes with similar information as the template information
				while(IdxS1 < IdxE1 && (*IdxS1)->data == dupData)
					*IdxM++ = *IdxS1++;
					// Continues to save nodes in second tree if there are still yet unsaved nodes with similar information as the template information
				while(IdxS2 < IdxE2 && (*IdxS2)->data == dupData)
					*IdxM++ = new rbtNode<K,D>(*IdxS2++);
			} else if ((*IdxS1)->data < (*IdxS2)->data) {
				delete *IdxS1;
				IdxS1++;
			} else
				IdxS2++;
    }, &rbTree<K,D>::dummy, false);
}

#endif /* _rbt_SET_OPERATIONS */