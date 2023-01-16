#ifndef _rbt_SET_OPERATIONS
#define _rbt_SET_OPERATIONS
#include	"RBT_main.h"
#include	"RBT_setops.h"
#include	"RBT_traversal.h"

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
template <typename F>
rbTree<K,D> rbTree<K,D>::orBase(const rbTree<K,D> &rbt, F foo) const {
		// If one or both of the trees are empty, do the trival case
	if (!rbt.size)
		return rbTree<K,D>(*this);
	if (!size)
		return rbTree<K,D>(rbt);

		// Initializes the needed values
	size_t sizeNew;
	rbtNode<K,D> **rbtNodes, **IdxS1, **IdxE1, **IdxS2, **IdxE2, **IdxM;

		// Calculates the maximum new tree size
	sizeNew = size + rbt.size;

		// Checks for node overflow
	if (sizeNew < size)
		throw std::overflow_error("overflow - red black tree(s) to large to perform opperation");

		// Initialize the needed array and indexes for the new tree
	rbtNodes = IdxS1 = IdxM = new rbtNode<K,D> *[sizeNew];
	IdxS2 = IdxE1 = IdxS1 + size;
	IdxE2 = IdxS1 + sizeNew;

		// Get all the nodes from both trees
	rbt_getAllNodes(IdxS1);
	rbt.rbt_getAllNodes(IdxS2);

		// Cycles through the entire node array
	foo(IdxS1, IdxS2, IdxE1, IdxE2, IdxM);

		// Inserts any remaining nodes into the result array
	while(IdxS1 < IdxE1)
		*IdxM++ = *IdxS1++;
	while(IdxS2 < IdxE2)
		*IdxM++ = *IdxS2++;

		// Calculates the size of the new tree
	sizeNew = IdxM - rbtNodes;

		// Makes the tree
	rbTree<K,D> rbtNew = rbTree<K,D>();
	rbtNew.size = sizeNew;
	rbtNew.treeify(0, sizeNew - 1, rbtNodes, &rbtNew.root);

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
template <typename F>
rbTree<K,D> rbTree<K,D>::orBaseI(const rbTree<K,D> &rbt, F foo) {
		// If one or both of the trees are empty, do the trival case
	if (!rbt.size)
		return *this;
	if (!size) {
		size = rbt.size;
		copyTree(rbt.root, &(this->root));
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
	IdxS1 = new rbtNode<K,D> *[sizeNew];
	rbtNodes = IdxM = new rbtNode<K,D> *[sizeNew];
	IdxS2 = IdxE1 = IdxS1 + size;
	IdxE2 = IdxS1 + sizeNew;

		// Get all the nodes from both trees
	rbt_getAllNodes(IdxS1);
	rbt.rbt_getAllNodes(IdxS2);

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
	treeifyShallow(0, size - 1, rbtNodes, &root);
	root->parent = NULL;

		// Deletes the remaining array
	delete [] rbtNodes;

		// Returns the finished tree
	return *this;
}

/*
 * function_identifier: Takes all node of two trees and return a tree with those nodes
 *						The resulting tree ignores duplicate by number
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::operator|(const rbTree<K,D> &rbt) const {
		// Calls the or template function
	return orBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes are equal
				// If so, save it and skip all other similarly equal nodes in the sequence
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
						// Saves nodes
					(*IdxM++) = *IdxS1;

						// Skips all similarly equal nodes
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
    });
}

/*
 * function_identifier: Inserts all nodes in provided tree into the current tree
 *						The resulting tree ignores duplicate by number
 * parameters: 			A red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::operator|=(const rbTree<K,D> &rbt) {
		// Calls the orI template function
	return orBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes are equal
				// If so, save it and skip all other similarly equal nodes in the sequence
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
						// Saves nodes
					(*IdxM++) = *IdxS1;

						// Skips all similarly equal nodes
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
    });
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
 * function_identifier: Takes all node of two trees and return a tree with those nodes
 *						If a node with the same key and data pair is in both trees, ignore it instead
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::operator^(const rbTree<K,D> &rbt) const {
		// Calls the or template function
	return orBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes are equal
				// If so, save it and skip all other similarly equal nodes in the sequence
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
    });
}

/*
 * function_identifier: Inserts all nodes in provided tree into the current tree
 *						If a node with the same key and data pair is in both trees, ignore it instead
 * parameters: 			A red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::operator^=(const rbTree<K,D> &rbt) {
		// Calls the orI template function
	return orBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes are equal
				// If so, skip all other similarly equal nodes in the sequence
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
    });
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
 * function_identifier: Takes all node of two trees and return a tree with those nodes
 *						If a node with the same key and data pair is in both trees, ignore all such nodes instead
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_xorLeast(const rbTree<K,D> &rbt) const {
		// Calls the or template function
	return orBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes are equal
				// If so, save it and skip all other similarly equal nodes in the sequence
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
						// Skips all similarly equal nodes
					while(++IdxS2 < IdxE2 && (*IdxS1)->equal(*IdxS2));
					IdxS2--;
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
    });
}

/*
 * function_identifier: Inserts all nodes in provided tree into the current tree
 *						If a node with the same key and data pair is in both trees, ignore all such nodes instead
 * parameters: 			A red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_xorLeastI(const rbTree<K,D> &rbt) {
		// Calls the orI template function
	return orBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes are equal
				// If so, skip all other similarly equal nodes in the sequence
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
						// Skips/deletes all similarly equal nodes
					while(++IdxS2 < IdxE2 && (*IdxS1)->equal(*IdxS2));
					IdxS2--;
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
    });
}

/*
 * function_identifier: Takes all node of two trees and return a tree with those nodes
 *						If a node with the same key is in both trees, ignore it instead
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
				// If so, skip all other similarly equal-key nodes in the sequence
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
					// Skips this node
				IdxS2++; IdxS1++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = *IdxS2++;
    });
}

/*
 * function_identifier: Inserts all nodes in provided tree into the current tree
 *						If a node with the same key is in both trees, ignore it instead
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
				// If so, skip all other similarly equal-key nodes in the sequence
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
					// Skips/deletes this node
				IdxS2++; delete *IdxS1++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = new rbtNode<K,D>(*IdxS2++);
    });
}

/*
 * function_identifier: Takes all node of two trees and return a tree with those nodes
 *						If a node with the same key pair is in both trees, ignore all such nodes instead
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
				// If so, skip all other similarly equal-key nodes in the sequence
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
					// Skips all similarly equal nodes
				while(++IdxS2 < IdxE2 && (*IdxS1)->key == (*IdxS2)->key);
				IdxS2--;
				while(++IdxS1 < IdxE1 && (*IdxS1)->key == (*IdxS2)->key);
				IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = *IdxS2++;
    });
}

/*
 * function_identifier: Inserts all nodes in provided tree into the current tree
 *						If a node with the same key is in both trees, ignore all such nodes instead
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
				// If so, skip all other similarly equal-key nodes in the sequence
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
					// Skips all similarly equal nodes
				while(++IdxS2 < IdxE2 && (*IdxS1)->key == (*IdxS2)->key);
				IdxS2--;
				do {
					delete *IdxS1++;
				} while(IdxS1 < IdxE1 && (*IdxS1)->key == (*IdxS2)->key);
				IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = new rbtNode<K,D>(*IdxS2++);
    });
}

/*
 * function_identifier: Takes all node of two trees and return a tree with those nodes
 *						If a node with the same data is in both trees, ignore it instead
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
				// If so, skip all other similarly equal-data nodes in the sequence
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
					// Skips this node
				IdxS2++; IdxS1++;
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = *IdxS2++;
    });
}

/*
 * function_identifier: Inserts all nodes in provided tree into the current tree
 *						If a node with the same data is in both trees, ignore it instead
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
				// If so, skip all other similarly equal-data nodes in the sequence
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
					// Skips this node
				IdxS2++; delete *IdxS1++;
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = new rbtNode<K,D>(*IdxS2++);
    });
}

/*
 * function_identifier: Takes all node of two trees and return a tree with those nodes
 *						If a node with the same data pair is in both trees, ignore all such nodes instead
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_xorDataLeast(const rbTree<K,D> &rbt) const {
		// Calls the or template function
	return orBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, skip all other similarly equal-data nodes in the sequence
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
					// Skips all similarly equal nodes
				while(++IdxS2 < IdxE2 && (*IdxS1)->data == (*IdxS2)->data);
				IdxS2--;
				while(++IdxS1 < IdxE1 && (*IdxS1)->data == (*IdxS2)->data);
				IdxS2++;
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = *IdxS2++;
    });
}

/*
 * function_identifier: Inserts all nodes in provided tree into the current tree
 *						If a node with the same data is in both trees, ignore all such nodes instead
 * parameters: 			A red-black tree
 * return value:		This object
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_xorDataLeastI(const rbTree<K,D> &rbt) {
		// Calls the orI template function
	return orBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 < IdxE1 && IdxS2 < IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, skip all other similarly equal-data nodes in the sequence
				// If not, save it and move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
					// Skips all similarly equal nodes
				while(++IdxS2 < IdxE2 && (*IdxS1)->data == (*IdxS2)->data);
				IdxS2--;
				do {
					delete *IdxS1++;
				} while(IdxS1 < IdxE1 && (*IdxS1)->data == (*IdxS2)->data);
				IdxS2++;
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				*IdxM++ = *IdxS1++;
			else
				*IdxM++ = new rbtNode<K,D>(*IdxS2++);
    });
}

/*
 * function_identifier: The base template for the and opperations
 * parameters: 			A red-black tree and a function to copy over nodes
 * return value:		A red-black tree
*/
template <typename K, typename D>
template <typename F>
rbTree<K,D> rbTree<K,D>::andBase(const rbTree<K,D> &rbt, F foo) const {
		// Ignores the trival case
	if (!size || !rbt.size)
		return rbTree<K,D>();

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS1, **IdxS2, **IdxE1, **IdxE2, **IdxM;

		// Gets the needed nodes and indexes
	rbtNodes = IdxM = IdxS1 = rbt_getAllNodes(); IdxS2 = rbt.rbt_getAllNodes();
	IdxE1 = IdxS1 + size; IdxE2 = IdxS2 + rbt.size; 

		// Cycles through the entire node array
	foo(IdxS1, IdxS2, IdxE1, IdxE2, IdxM);

		// Clears the second tree node array
	delete [] (IdxE2 - rbt.size);

		// Calculates the new size
	size_t sizeNew = (IdxM - rbtNodes);

		// Create a tree from nodes
	rbTree<K,D> rbtNew = rbTree<K,D>();
	rbtNew.size = sizeNew;

	if (sizeNew)
			// If there are remaining nodes, place them into the new tree
		rbtNew.treeify(0, sizeNew-1, rbtNodes, &rbtNew.root);

		// Deletes the remaining array
	delete [] rbtNodes;

		// Return the finished tree
	return rbtNew;
}

/*
 * function_identifier: The base template for the andI opperations
 * parameters: 			A red-black tree and a function to copy over nodes
 * return value:		A red-black tree
*/
template <typename K, typename D>
template <typename F>
rbTree<K,D> rbTree<K,D>::andBaseI(const rbTree<K,D> &rbt, F foo) {
		// Ignores the trival case
	if (!size || !rbt.size) {
		rbt_clear();
		return *this;
	}

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxM, **IdxS1, **IdxE1, **IdxS2, **IdxE2;

		// Get all the nodes from both trees
	rbtNodes = IdxM = IdxS1 = rbt_getAllNodes(); IdxS2 = rbt.rbt_getAllNodes();
	IdxE1 = IdxS1 + size; IdxE2 = IdxS2 + rbt.size; 

		// Cycles through the entire node array
	foo(IdxS1, IdxS2, IdxE1, IdxE2, IdxM);

		// Clears the second tree node array
	delete [] (IdxE2 - rbt.size);

		// Deletes any extra nodes
	while(IdxS1 != IdxE1) {
		delete *IdxS1;
		IdxS1++;
	}

		// Calculates the new size
	size = (IdxM - rbtNodes);

		// Makes the tree
	if (size) {
		treeifyShallow(0, size - 1, rbtNodes, &root);
		root->parent = NULL;
	} else
		root = NULL;

		// Deletes the remaining array
	delete [] rbtNodes;

		// Returns this object
	return *this;
}

/*
 * function_identifier: The base template for the andMost opperations
 * parameters: 			A red-black tree and a function to copy over nodes
 * return value:		A red-black tree
*/
template <typename K, typename D>
template <typename F>
rbTree<K,D> rbTree<K,D>::andMostBase(const rbTree<K,D> &rbt, F foo) const {
		// Ignores the trival case
	if (!size || !rbt.size)
		return rbTree<K,D>();

		// Initializes the needed values
	size_t sizeNew;
	rbtNode<K,D> **rbtNodes, **IdxS1, **IdxE1, **IdxS2, **IdxE2, **IdxM;

		// Calculates the new tree size.
	sizeNew = size + rbt.size;

		// Checks for node overflow
	if (sizeNew < size)
		throw std::overflow_error("overflow - red black tree(s) to large to perform opperation");

		// Gets the needed nodes and indexes
	rbtNodes = IdxM = new rbtNode<K,D> *[sizeNew];
	IdxS1 = rbtNodes + rbt.size; IdxE1 = rbtNodes + sizeNew; rbt_getAllNodes(IdxS1);
	IdxS2 = rbt.rbt_getAllNodes(); IdxE2 = IdxS2 + rbt.size;

		// Cycles through the entire node array
	foo(IdxS1, IdxS2, IdxE1, IdxE2, IdxM);

		// Clears the second tree node array
	delete [] (IdxE2 - rbt.size);

		// Calculates the size of the new tree
	sizeNew = IdxM - rbtNodes;

		// Makes the tree
	rbTree<K,D> rbtNew = rbTree<K,D>();
	if (sizeNew) {
		rbtNew.size = sizeNew;
		rbtNew.treeify(0, sizeNew - 1, rbtNodes, &rbtNew.root);
	}

		// Deletes the remaining array
	delete [] rbtNodes;

		// Returns the finished tree
	return rbtNew;
}

/*
 * function_identifier: The base template for the andMostI opperations
 * parameters: 			A red-black tree and a function to copy over nodes
 * return value:		A red-black tree
*/
template <typename K, typename D>
template <typename F>
rbTree<K,D> rbTree<K,D>::andMostBaseI(const rbTree<K,D> &rbt, F foo) {
		// Ignores the trival case
	if (!size)
		return *this;
	if (!rbt.size) {
		rbt_clear();
		return *this;
	}

		// Initializes the needed values
	size_t sizeNew;
	rbtNode<K,D> **rbtNodes, **IdxS1, **IdxE1, **IdxS2, **IdxE2, **IdxM;

		// Calculates the new tree size.
	sizeNew = size + rbt.size;

		// Checks for node overflow
	if (sizeNew < size)
		throw std::overflow_error("overflow - red black tree(s) to large to perform opperation");

		// Gets the needed nodes and indexes
	rbtNodes = IdxM = new rbtNode<K,D> *[sizeNew];
	IdxS1 = rbtNodes + rbt.size; IdxE1 = rbtNodes + sizeNew; rbt_getAllNodes(IdxS1);
	IdxS2 = rbt.rbt_getAllNodes(); IdxE2 = IdxS2 + rbt.size;

		// Cycles through the entire node array
	foo(IdxS1, IdxS2, IdxE1, IdxE2, IdxM);

		// Clears the second tree node array
	delete [] (IdxE2 - rbt.size);

		// Deletes any extra nodes
	while(IdxS1 != IdxE1)
		delete *IdxS1++;

		// Calculates the size of the new tree
	sizeNew = IdxM - rbtNodes;

		// Makes the tree
	if (sizeNew) {
		size = sizeNew;
		treeifyShallow(0, sizeNew - 1, rbtNodes, &root);
		root->parent = NULL;
	}

		// Deletes the remaining array
	delete [] rbtNodes;

		// Returns the finished tree
	return *this;
}

/*
 * function_identifier: Takes all nodes, in both trees, and returns them in a new tree
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::operator&(const rbTree<K,D> &rbt) const {
		// Calls the and template function
	return andBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2) {
				// Checks if both considered nodes have equal keys and data
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
    });
}
					
/*
 * function_identifier: Deletes any nodes, in the current tree, with a key and data pair different from any other node in the provided tree
 * parameters: 			A red-black tree
 * return value:		A red-black tree
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::operator&=(const rbTree<K,D> &rbt) {
		// Calls the andI template function
	return andBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal keys and data
				// If so, save it and move forward in both arrays
				// If not, skip/delete it, then move forward in one of the arrays
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
    });
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
 * function_identifier: 
 * parameters: 			
 * return value:		
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andLeast(const rbTree<K,D> &rbt) const {
		// Calls the and template function
	return andBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal keys and data
				// If so, save it and skip all other similarly equal nodes
				// If not, skip it, then move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
					*IdxM++ = *IdxS1;
					rbtNode<K,D> *temp = *IdxS1++;

					while(IdxS1 < IdxE1 && temp->equal(*IdxS1))
						IdxS1++;
					IdxS2++;
					while(IdxS2 < IdxE2 && temp->equal(*IdxS2))
						IdxS2++;
				} else if ((*IdxS1)->data < (*IdxS2)->data)
					IdxS1++;
				else
					IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				IdxS1++;
			else
				IdxS2++;
    });
}

/*
 * function_identifier: 
 * parameters: 			
 * return value:		
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andLeastI(const rbTree<K,D> &rbt) {
		// Calls the andI template function
	return andBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal keys and data
				// If so, save it and skip/delete all similarly equal nodes
				// If not, skip/delete it, then move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
					*IdxM++ = *IdxS1;
					rbtNode<K,D> *temp = *IdxS1++;

					while(IdxS1 < IdxE1 && temp->equal(*IdxS1))
						delete *IdxS1++;
					IdxS2++;
					while(IdxS2 < IdxE2 && temp->equal(*IdxS2))
						IdxS2++;
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
    });
}

/*
 * function_identifier: 
 * parameters: 			
 * return value:		
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andMost(const rbTree<K,D> &rbt) const {
		// Calls the andMost template function
	return andMostBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal keys and data
				// If so, save all similarly equal nodes
				// If not, skip it, then move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
					rbtNode<K,D> *temp = *IdxS1;

					do {
						*IdxM++ = *IdxS1++; IdxS2++;
					} while(IdxS1 < IdxE1 && IdxS2 < IdxE2 && (*IdxS1)->equal(*IdxS2));
					while(IdxS1 < IdxE1 && temp->equal(*IdxS1))
						*IdxM++ = *IdxS1++;
					while(IdxS2 < IdxE2 && temp->equal(*IdxS2))
						*IdxM++ = *IdxS2++;
				} else if ((*IdxS1)->data < (*IdxS2)->data)
					IdxS1++;
				else
					IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				IdxS1++;
			else
				IdxS2++;
    });
}

/*
 * function_identifier: 
 * parameters: 			
 * return value:		
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andMostI(const rbTree<K,D> &rbt) {
		// Calls the andMostI template function
	return andMostBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal keys and data
				// If so, save all similarly equal nodes
				// If not, skip/delete it, then move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				if ((*IdxS1)->data == (*IdxS2)->data) {
					rbtNode<K,D> *temp = *IdxS1;

					do {
						*IdxM++ = *IdxS1++; IdxS2++;
					} while(IdxS1 < IdxE1 && IdxS2 < IdxE2 && (*IdxS1)->equal(*IdxS2));
					while(IdxS1 < IdxE1 && temp->equal(*IdxS1))
						*IdxM++ = *IdxS1++;
					while(IdxS2 < IdxE2 && temp->equal(*IdxS2))
						*IdxM++ = new rbtNode<K,D>(*IdxS2++);
				} else if ((*IdxS1)->data < (*IdxS2)->data)
					delete *IdxS1++;
				else
					IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				delete *IdxS1++;
			else
				IdxS2++;
    });
}

/*
 * function_identifier: 
 * parameters: 			
 * return value:		
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andKey(const rbTree<K,D> &rbt) const {
		// Calls the and template function
	return andBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, save it and move forward
				// If not, skip it, then move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				*IdxM = *IdxS1;
				IdxM++; IdxS1++; IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				IdxS1++;
			else
				IdxS2++;
    });
}

/*
 * function_identifier: 
 * parameters: 			
 * return value:		
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andKeyI(const rbTree<K,D> &rbt) {
		// Calls the andI template function
	return andBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2) {
				// Checks if both considered nodes have equal keys
				// If so, save it and move forward
				// If not, skip/delete it, then move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				*IdxM = *IdxS1;
				IdxM++; IdxS1++; IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				delete *IdxS1++;
			else
				IdxS2++;
		}
    });
}

/*
 * function_identifier: 
 * parameters: 			
 * return value:		
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andKeyLeast(const rbTree<K,D> &rbt) const {
		// Calls the and template function
	return andBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, save it and skip other similarly equal nodes
				// If not, skip it, then move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				*IdxM++ = *IdxS1;
				K temp = (*IdxS1++)->key;

				while(IdxS1 < IdxE1 && (*IdxS1)->key == temp)
					IdxS1++;
				IdxS2++;
				while(IdxS2 < IdxE2 && (*IdxS2)->key == temp)
					IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				IdxS1++;
			else
				IdxS2++;
    });
}

/*
 * function_identifier: 
 * parameters: 			
 * return value:		
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andKeyLeastI(const rbTree<K,D> &rbt) {
		// Calls the andI template function
	return andBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, save it and skip/delete other similarly equal nodes
				// If not, skip/delete it, then move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				*IdxM++ = *IdxS1;
				K temp = (*IdxS1++)->key;

				while(IdxS1 < IdxE1 && (*IdxS1)->key == temp)
					delete *IdxS1++;
				IdxS2++;
				while(IdxS2 < IdxE2 && (*IdxS2)->key == temp)
					IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key) {
				delete (*IdxS1);
				IdxS1++;
			} else
				IdxS2++;
    });
}

/*
 * function_identifier: 
 * parameters: 			
 * return value:		
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andKeyMost(const rbTree<K,D> &rbt) const {
		// Calls the andMost template function
	return andMostBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, save all similarly equal nodes
				// If not, skip it, then move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				K temp = (*IdxS1)->key;

				do {
					*IdxM++ = *IdxS1++; IdxS2++;
				} while(IdxS1 < IdxE1 && IdxS2 < IdxE2 && (*IdxS1)->key == (*IdxS2)->key);
				while(IdxS1 < IdxE1 && (*IdxS1)->key == temp)
					*IdxM++ = *IdxS1++;
				while(IdxS2 < IdxE2 && (*IdxS2)->key == temp)
					*IdxM++ = *IdxS2++;
			} else if ((*IdxS1)->key < (*IdxS2)->key)
				IdxS1++;
			else
				IdxS2++;
    });
}

/*
 * function_identifier: 
 * parameters: 			
 * return value:		
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andKeyMostI(const rbTree<K,D> &rbt) {
		// Calls the andMostI template function
	return andMostBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal keys
				// If so, save all similarly equal nodes
				// If not, skip/delete it, then move forward in one of the arrays
			if ((*IdxS1)->key == (*IdxS2)->key) {
				K temp = (*IdxS1)->key;

				do {
					*IdxM++ = *IdxS1++; IdxS2++;
				} while(IdxS1 < IdxE1 && IdxS2 < IdxE2 && (*IdxS1)->key == (*IdxS2)->key);
				while(IdxS1 < IdxE1 && (*IdxS1)->key == temp)
					*IdxM++ = *IdxS1++;
				while(IdxS2 < IdxE2 && (*IdxS2)->key == temp)
					*IdxM++ = new rbtNode<K,D>(*IdxS2++);
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				delete *IdxS1++;
			else
				IdxS2++;
    });
}

/*
 * function_identifier: 
 * parameters: 			
 * return value:		
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andData(const rbTree<K,D> &rbt) const {
		// Calls the and template function
	return andBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal data
				// If so, save it and move forward
				// If not, skip it, then move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
				*IdxM = *IdxS1;
				IdxM++; IdxS1++; IdxS2++;
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				IdxS1++;
			else
				IdxS2++;
    });
}

/*
 * function_identifier: 
 * parameters: 			
 * return value:		
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andDataI(const rbTree<K,D> &rbt) {
		// Calls the andI template function
	return andBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal data
				// If so, save it and move forward
				// If not, skip/delete it, then move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
				*IdxM = *IdxS1;
				IdxM++; IdxS1++; IdxS2++;
			} else if ((*IdxS1)->data < (*IdxS2)->data) {
				delete *IdxS1;
				IdxS1++;
			} else
				IdxS2++;
    });
}

/*
 * function_identifier: 
 * parameters: 			
 * return value:		
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andDataLeast(const rbTree<K,D> &rbt) const {
		// Calls the and template function
	return andBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal data
				// If so, save it and skip other similarly equal nodes
				// If not, skip it, then move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
				*IdxM++ = *IdxS1;
				D temp = (*IdxS1++)->data;

				while(IdxS1 < IdxE1 && (*IdxS1)->data == temp)
					IdxS1++;
				IdxS2++;
				while(IdxS2 < IdxE2 && (*IdxS2)->data == temp)
					IdxS2++;
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				IdxS1++;
			else
				IdxS2++;
    });
}

/*
 * function_identifier: 
 * parameters: 			
 * return value:		
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andDataLeastI(const rbTree<K,D> &rbt) {
		// Calls the andI template function
	return andBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal data
				// If so, save it and skip/delete other similarly equal nodes
				// If not, skip/delete it, then move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
				*IdxM++ = *IdxS1;
				K temp = (*IdxS1++)->key;

				while(IdxS1 < IdxE1 && (*IdxS1)->key == temp)
					delete *IdxS1++;
				IdxS2++;
				while(IdxS2 < IdxE2 && (*IdxS2)->key == temp)
					IdxS2++;
			} else if ((*IdxS1)->data < (*IdxS2)->data) {
				delete (*IdxS1);
				IdxS1++;
			} else
				IdxS2++;
    });
}

/*
 * function_identifier: 
 * parameters: 			
 * return value:		
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andDataMost(const rbTree<K,D> &rbt) const {
		// Calls the andMost template function
	return andMostBase(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal data
				// If so, save all similarly equal nodes
				// If not, skip it, then move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
				D temp = (*IdxS1)->data;

				do {
					*IdxM++ = *IdxS1++; IdxS2++;
				} while(IdxS1 < IdxE1 && IdxS2 < IdxE2 && (*IdxS1)->data == (*IdxS2)->data);
				while(IdxS1 < IdxE1 && (*IdxS1)->data == temp)
					*IdxM++ = *IdxS1++;
				while(IdxS2 < IdxE2 && (*IdxS2)->data == temp)
					*IdxM++ = *IdxS2++;
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				IdxS1++;
			else
				IdxS2++;
    });
}

/*
 * function_identifier: 
 * parameters: 			
 * return value:		
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_andDataMostI(const rbTree<K,D> &rbt) {
		// Calls the andMostI template function
	return andMostBaseI(rbt, [] (rbtNode<K,D> **&IdxS1, rbtNode<K,D> **&IdxS2, rbtNode<K,D> **&IdxE1, rbtNode<K,D> **&IdxE2, rbtNode<K,D> **&IdxM) -> void {
			// Loops through both sections of the array
		while(IdxS1 != IdxE1 && IdxS2 != IdxE2)
				// Checks if both considered nodes have equal data
				// If so, save all similarly equal nodes
				// If not, skip/delete it, then move forward in one of the arrays
			if ((*IdxS1)->data == (*IdxS2)->data) {
				D temp = (*IdxS1)->data;

				do {
					*IdxM++ = *IdxS1++; IdxS2++;
				} while(IdxS1 < IdxE1 && IdxS2 < IdxE2 && (*IdxS1)->data == (*IdxS2)->data);
				while(IdxS1 < IdxE1 && (*IdxS1)->data == temp)
					*IdxM++ = *IdxS1++;
				while(IdxS2 < IdxE2 && (*IdxS2)->data == temp)
					*IdxM++ = new rbtNode<K,D>(*IdxS2++);
			} else if ((*IdxS1)->data < (*IdxS2)->data)
				delete *IdxS1++;
			else
				IdxS2++;
    });
}

#endif /* _rbt_SET_OPERATIONS */