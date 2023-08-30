#ifndef _rbt_TRAVERSAL
#define _rbt_TRAVERSAL
#include	"RBT_main.h"
#include	"RBT_traversal.h"

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
	inline void **nodePtr;
	inline std::byte *basePtr;
}

/*	============================================================================  */
/* |                                                                            | */
/* |                     	         PRE ORDER                                  | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: Returns an array of all rbtNodes, in the given red-black tree, via a preorder traversal
 * parameters: 			A pointer to a node in the red-black tree
 * return value:		An array of rbtNodes (return by reference [global variable])
*/
template <typename K, typename D>
void rbTree<K,D>::nodesPre(rbtNode<K,D> *curr) const {
		// Saves the node at the current index
	*nodePtr = curr;
		// Increments the global pointer index
	nodePtr++;

		// Performs preorder traversal
	if(curr->left) nodesPre(curr->left);
	if(curr->right) nodesPre(curr->right);
}

/*
 * function_identifier: Returns an array of all rbtNodes that have the given key, in the given red-black tree, via an preorder traversal
 * parameters: 			A pointer to a node in the red-black tree
 * return value:		An array of rbtNodes that have the given key (return by reference [global variable])
*/
template <typename K, typename D>
void rbTree<K,D>::nodesPreWithKey(rbtNode<K,D> *curr, K key) const {
		// Checks if the current tree has the correct key
	if (key == curr->key) {
			// Saves the node at the current index
		*nodePtr = curr;
			// Increments the global pointer index
		nodePtr++;
	}

		// Performs preorder traversal
	if(curr->left) nodesPreWithKey(curr->left, key);
	if(curr->right) nodesPreWithKey(curr->right, key);
}

/*
 * function_identifier: Returns an array of all rbtNodes that have the given data, in the given red-black tree, via an preorder traversal
 * parameters: 			A pointer to a node in the red-black tree
 * return value:		An array of rbtNodes that have the given data (return by reference [global variable])
*/
template <typename K, typename D>
void rbTree<K,D>::nodesPreWithData(rbtNode<K,D> *curr, D data) const {
		// Checks if the current tree has the correct data
	if (data == curr->data) {
			// Saves the node at the current index
		*nodePtr = curr;
			// Increments the global pointer index
		nodePtr++;
	}

		// Performs preorder traversal
	if(curr->left) nodesPreWithData(curr->left, data);
	if(curr->right) nodesPreWithData(curr->right, data);
}

/*
 * function_identifier: Returns an array of all rbtNodes that have the given data and key, in the given red-black tree, via an preorder traversal
 * parameters: 			A pointer to a node in the red-black tree
 * return value:		An array of rbtNodes that have the given data (return by reference [global variable])
*/
template <typename K, typename D>
void rbTree<K,D>::nodesPreWithDataKey(rbtNode<K,D> *curr, D data, K key) const {
		// Checks if the current tree has the correct data
	if (data == curr->data) {
			// Saves the node at the current index
		*nodePtr = curr;
			// Increments the global pointer index
		nodePtr++;
	}

		// Performs preorder traversal on nodes with the correct key
	if(curr->left && curr->left->key == key) nodesPreWithDataKey(curr->left, data, key);
	if(curr->right && curr->right->key == key) nodesPreWithDataKey(curr->right, data, key);
}

/*	============================================================================  */
/* |                                                                            | */
/* |                     	         IN  ORDER                                  | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: Returns an array of all rbtNodes, in the given red-black tree, via an inorder traversal
 * parameters: 			A pointer to a node in the red-black tree
 * return value:		An array of rbtNodes (return by reference [global variable])
*/
template <typename K, typename D>
void rbTree<K,D>::nodesIn(rbtNode<K,D> *curr) const {
		// Performs inorder traversal
	if(curr->left) nodesIn(curr->left);
		// Saves the node at the current index
	*nodePtr = curr;
		// Increments the global pointer index
	nodePtr++;
	if(curr->right) nodesIn(curr->right);
}

/*
 * function_identifier: Returns an array of all rbtNodes that have the given key, in the given red-black tree, via an inorder traversal
 * parameters: 			A pointer to a node in the red-black tree
 * return value:		An array of rbtNodes that have the given key (return by reference [global variable])
*/
template <typename K, typename D>
void rbTree<K,D>::nodesInWithKey(rbtNode<K,D> *curr, K key) const {
		// Performs inorder traversal
	if(curr->left) nodesInWithKey(curr->left, key);
		// Checks if the current tree has the correct kay
	if (key == curr->key) {
			// Saves the node at the current index
		*nodePtr = curr;
			// Increments the global pointer index
		nodePtr++;
	}
	if(curr->right) nodesInWithKey(curr->right, key);
}

/*
 * function_identifier: Returns an array of all rbtNodes that have the given data, in the given red-black tree, via an inorder traversal
 * parameters: 			A pointer to a node in the red-black tree
 * return value:		An array of rbtNodes that have the given data (return by reference [global variable])
*/
template <typename K, typename D>
void rbTree<K,D>::nodesInWithData(rbtNode<K,D> *curr, D data) const {
		// Performs inorder traversal
	if(curr->left) nodesInWithData(curr->left, data);
		// Checks if the current tree has the correct data
	if (data == curr->data) {
			// Saves the node at the current index
		*nodePtr = curr;
			// Increments the global pointer index
		nodePtr++;
	}
	if(curr->right) nodesInWithData(curr->right, data);
}

/*
 * function_identifier: Returns an array of all rbtNodes that have the given data and key, in the given red-black tree, via an inorder traversal
 * parameters: 			A pointer to a node in the red-black tree
 * return value:		An array of rbtNodes that have the given data (return by reference [global variable])
*/
template <typename K, typename D>
void rbTree<K,D>::nodesInWithDataKey(rbtNode<K,D> *curr, D data, K key) const {
		// Performs inorder traversal on nodes with the correct key
	if(curr->left && curr->left->key == key) nodesInWithDataKey(curr->left, data, key);
		// Checks if the current tree has the correct data
	if (data == curr->data) {
			// Saves the node at the current index
		*nodePtr = curr;
			// Increments the global pointer index
		nodePtr++;
	}
	if(curr->right && curr->right->key == key) nodesInWithDataKey(curr->right, data, key);
}

/*	============================================================================  */
/* |                                                                            | */
/* |                     	        POST ORDER                                  | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: Returns an array of all rbtNodes, in the given red-black tree, via a postorder traversal
 * parameters: 			A pointer to a red-black tree
 * return value:		An array of rbtNodes (return by reference [global variable])
*/
template <typename K, typename D>
void rbTree<K,D>::nodesPost(rbtNode<K,D> *curr) const {
		// Performs postorder traversal
	if(curr->left) nodesPost(curr->left);
	if(curr->right) nodesPost(curr->right);
		// Saves the node at the current index
	*nodePtr = curr;
		// Increments the global pointer index
	nodePtr++;
}

/*
 * function_identifier: Returns an array of all rbtNodes that have the given key, in the given red-black tree, via an postorder traversal
 * parameters: 			A pointer to a node in the red-black tree
 * return value:		An array of rbtNodes that have the given key (return by reference [global variable])
*/
template <typename K, typename D>
void rbTree<K,D>::nodesPostWithKey(rbtNode<K,D> *curr, K key) const {
		// Performs postorder traversal
	if(curr->left) nodesPostWithKey(curr->left, key);
	if(curr->right) nodesPostWithKey(curr->right, key);
		// Checks if the current tree has the correct key
	if (key == curr->key) {
			// Saves the node at the current index
		*nodePtr = curr;
			// Increments the global pointer index
		nodePtr++;
	}
}

/*
 * function_identifier: Returns an array of all rbtNodes that have the given data, in the given red-black tree, via an postorder traversal
 * parameters: 			A pointer to a node in the red-black tree
 * return value:		An array of rbtNodes that have the given data (return by reference [global variable])
*/
template <typename K, typename D>
void rbTree<K,D>::nodesPostWithData(rbtNode<K,D> *curr, D data) const {
		// Performs postorder traversal
	if(curr->left) nodesPostWithData(curr->left, data);
	if(curr->right) nodesPostWithData(curr->right, data);
		// Checks if the current tree has the correct data
	if (data == curr->data) {
			// Saves the node at the current index
		*nodePtr = curr;
			// Increments the global pointer index
		nodePtr++;
	}
}

/*
 * function_identifier: Returns an array of all rbtNodes that have the given data and key, in the given red-black tree, via an postorder traversal
 * parameters: 			A pointer to a node in the red-black tree
 * return value:		An array of rbtNodes that have the given data (return by reference [global variable])
*/
template <typename K, typename D>
void rbTree<K,D>::nodesPostWithDataKey(rbtNode<K,D> *curr, D data, K key) const {
		// Performs postorder traversal on nodes with the correct key
	if(curr->left && curr->left->key == key) nodesPostWithDataKey(curr->left, data, key);
	if(curr->right && curr->right->key == key) nodesPostWithDataKey(curr->right, data, key);
		// Checks if the current tree has the correct data
	if (data == curr->data) {
			// Saves the node at the current index
		*nodePtr = curr;
			// Increments the global pointer index
		nodePtr++;
	}
}

/*	============================================================================  */
/* |                                                                            | */
/* |                     	       LEVEL ORDER                                  | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: Returns an array of all rbtNodes, in the given red-black tree, via a levelorder traversal
 * parameters: 			N/A
 * return value:		An array of rbtNodes (return by reference [global variable])
*/
template <typename K, typename D>
void rbTree<K,D>::nodesLevel() const {
		// Initializes values
	rbtNode<K,D> **startPtr, **endPtr;
	startPtr = endPtr = (rbtNode<K,D> **)nodePtr;

		// Places the root in the queue
	*startPtr = root; endPtr++;

		// Saves all nodes, in the red-black tree, into the queue
	while(startPtr != endPtr) {
			// If left child exists, place it in the queue
		if((*startPtr)->left)
			*endPtr++ = (*startPtr)->left;
			// If right child exists, place it in the queue
		if((*startPtr)->right)
			*endPtr++ = (*startPtr)->right;
			// Moves forward in queue
		startPtr++;
	}
}

/*
 * function_identifier: Returns an array of all rbtNodes that have the given key, in the given red-black tree, via a levelorder traversal
 * parameters: 			N/A
 * return value:		An array of rbtNodes that have the given key (return by reference [global variable])
*/
template <typename K, typename D>
void rbTree<K,D>::nodesLevelWithKey(K key) const {
		// Initializes the queue and shift values
	rbtNode<K,D> **queue = new rbtNode<K,D>*[size], **queueNext = queue + 1, **startPtr = (rbtNode<K,D> **)nodePtr;
	rbtNode<K,D> **queueSt = queue;

		// Places all nodes into queue
		// Only places nodes, with requested key, into the return array
	for(*queue = root; queueNext != queue; queue++) {
			// Checks if node has requested key
		if((*queue)->key == key)
				// If so, place it in the return array
			*startPtr++ = *queue;

			// Checks if left branch exists
		if((*queue)->left)
				// If so, place left child at next position in queue and update the next available position
			*queueNext++ = (*queue)->left;
			// Checks if right branch exists
		if((*queue)->right)
				// If so, place left child at next position in queue
			*queueNext++ = (*queue)->right;
	}

		// Clears the queue
	delete [] queueSt;
}

/*
 * function_identifier: Returns an array of all rbtNodes that have the given data, in the given red-black tree, via a levelorder traversal
 * parameters: 			N/A
 * return value:		An array of rbtNodes (return by reference [global variable])
*/
template <typename K, typename D>
void rbTree<K,D>::nodesLevelWithData(D data) const {
		// Initializes the queue and shift values
	rbtNode<K,D> **queue = new rbtNode<K,D>*[size], **queueNext = queue + 1, **startPtr = (rbtNode<K,D> **)nodePtr;
	rbtNode<K,D> **queueSt = queue;

		// Places all nodes into queue
		// Only places nodes, with requested data, into the return array
	for(*queue = root; queueNext != queue; queue++) {
			// Checks if node has requested data
		if((*queue)->data == data)
				// If so, place it in the return array
			*startPtr++ = *queue;

			// Checks if left branch exists
		if((*queue)->left)
				// If so, place left child at next position in queue and update the next available position
			*queueNext++ = (*queue)->left;
			// Checks if right branch exists
		if((*queue)->right)
				// If so, place left child at next position in queue
			*queueNext++ = (*queue)->right;
	}

		// Clears the queue
	delete [] queueSt;
}

/*
 * function_identifier: Returns an array of all rbtNodes that have the given data and key, in the given red-black tree, via a levelorder traversal
 * parameters: 			N/A
 * return value:		An array of rbtNodes (return by reference [global variable])
*/
template <typename K, typename D>
void rbTree<K,D>::nodesLevelWithDataKey(D data, K key) const {
		// Initializes the queue and shift values
	rbtNode<K,D> **queue = new rbtNode<K,D>*[size], **queueNext = queue + 1, **startPtr = (rbtNode<K,D> **)nodePtr;
	rbtNode<K,D> **queueSt = queue;

		// Places all nodes into queue
		// Only places nodes, with requested key and data pair, into the return array
	for(*queue = root; queueNext != queue; queue++) {
			// Checks if node has requested key and data pair
		if((*queue)->key == key && (*queue)->data == data)
				// If so, place it in the return array
			*startPtr++ = *queue;

			// Checks if left branch exists
		if((*queue)->left)
				// If so, place left child at next position in queue and update the next available position
			*queueNext++ = (*queue)->left;
			// Checks if right branch exists
		if((*queue)->right)
				// If so, place left child at next position in queue
			*queueNext++ = (*queue)->right;
	}

		// Clears the queue
	delete [] queueSt;
}

/*	============================================================================  */
/* |                                                                            | */
/* |                     	   SPECIFIED ORDER                                  | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: The base template for PRINT_LEVEL node access traversing
 * parameters: 			The level to access, a rbtNode with a key/data pair criteria, and a function to filter the level's nodes with
 * return value:		An array of rbtNodes (return by reference [global variable]) and the number of nodes (return by value)
*/
template <typename K, typename D>
template <typename F>
inline size_t rbTree<K,D>::nodesSpecifiedLevelBase(size_t level, rbtNode<K,D> filter, F foo) const {
		// Initializes the need variables
	rbtNode<K,D> **nodeE, **nodeS, **nodeM, **nodeArray = (rbtNode<K,D> **)nodePtr, *temp;
	size_t nodes = (1 << level), offset = (nodes >> 1), shift = 0;
	*nodeArray = root; nodeE = nodeArray + nodes;

		// Cycles through all nodes in the tree, level by level
	for(size_t i=0; i<level; i++, offset>>=1, nodeE -= shift, shift = 0) {
			// Resets the placement and access node indexes
		nodeM = nodeS = nodeArray;
		while(nodeS < nodeE) {
			temp = *nodeS;
				// If child, place it in the node array at a position that will not be overwriten during this pass
				// If no child, decrease the total node count by the appropriate amount
			if (temp->left) {
				*nodeM = temp->left;
				nodeM += offset;
			} else
				shift += offset;
			if (temp->right) {
				*nodeM = temp->right;
				nodeM += offset;
			} else
				shift += offset;

				// Increases the access index
			nodeS += (offset << 1);
		}
	}

		// Cycles through the last level
	foo(nodeE, nodeArray, filter);

		// Returns the accessed nodes
	return (nodeE - nodeArray);
}

/*
 * function_identifier: Returns an array of all rbtNodes, in the given red-black tree, at a specified level
 * parameters: 			The level to extract rbtNodes from
 * return value:		An array of rbtNodes (return by reference [global variable]) and the number of nodes (return by value)
*/
template <typename K, typename D>
size_t rbTree<K,D>::nodesSpecifiedLevel(size_t level) const {
	return nodesSpecifiedLevelBase(level, rbtNode<K,D>(K(), D()), [] (...) -> void {});
}

/*
 * function_identifier: Returns an array of all rbtNodes that have the given key, in the given red-black tree, at a specified level
 * parameters: 			The level to extract nodes from and a key criteria
 * return value:		An array of rbtNodes (return by reference [global variable]) and the number of nodes (return by value)
*/
template <typename K, typename D>
size_t rbTree<K,D>::nodesSpecifiedLevelWithKey(size_t level, K key) const {
	return nodesSpecifiedLevelBase(level, rbtNode<K,D>(key, D()), [] (rbtNode<K,D> **&nodeE, rbtNode<K,D> **nodeS, rbtNode<K,D> filter) -> void {
			// Initializes the need variables
		rbtNode<K,D> **nodeM = nodeS;
			// Cycles through all nodes in the current level
		while(nodeS < nodeE) {
				// If node exists and is of the desired key, place it in the next available spot
			if ((*nodeS) && (*nodeS)->key == filter.key)
				*nodeM++ = (*nodeS);
				// Increment the node index
			nodeS++;
		}
			// Changes the end node appropriately
		nodeE = nodeM;
	});
}

/*
 * function_identifier: Returns an array of all rbtNodes that have the given data, in the given red-black tree, at a specified level
 * parameters: 			The level to extract rbtNodes from and a data criteria
 * return value:		An array of rbtNodes (return by reference [global variable]) and the number of nodes (return by value)
*/
template <typename K, typename D>
size_t rbTree<K,D>::nodesSpecifiedLevelWithData(size_t level, D data) const {
	return nodesSpecifiedLevelBase(level, rbtNode<K,D>(K(), data), [] (rbtNode<K,D> **&nodeE, rbtNode<K,D> **nodeS, rbtNode<K,D> filter) -> void {
			// Initializes the need variables
		rbtNode<K,D> **nodeM = nodeS;
			// Cycles through all nodes in the current level
		while(nodeS < nodeE) {
				// If node exists and is of the desired key, place it in the next available spot
			if ((*nodeS) && (*nodeS)->data == filter.data)
				*nodeM++ = (*nodeS);
				// Increment the node index
			nodeS++;
		}
			// Changes the end node appropriately
		nodeE = nodeM;
	});
}

/*
 * function_identifier: Returns an array of all rbtNodes that have the given data/key pair, in the given red-black tree, at a specified level
 * parameters: 			The level to extract rbtNodes from and a data/key pair criteria
 * return value:		An array of rbtNodes (return by reference [global variable]) and the number of nodes (return by value)
*/
template <typename K, typename D>
size_t rbTree<K,D>::nodesSpecifiedLevelWithDataKey(size_t level, D data, K key) const {
	return nodesSpecifiedLevelBase(level, rbtNode<K,D>(key, data), [] (rbtNode<K,D> **&nodeE, rbtNode<K,D> **nodeS, rbtNode<K,D> filter) -> void {
			// Initializes the need variables
		rbtNode<K,D> **nodeM = nodeS;
			// Cycles through all nodes in the current level
		while(nodeS < nodeE) {
				// If node exists and is of the desired key, place it in the next available spot
			if ((*nodeS) && (*nodeS)->key == filter.key && (*nodeS)->data == filter.data)
				*nodeM++ = (*nodeS);
				// Increment the node index
			nodeS++;
		}
			// Changes the end node appropriately
		nodeE = nodeM;
	});
}

/*	============================================================================  */
/* |                                                                            | */
/* |                     	   GET NODE CONTROLLERS                             | */
/* |                                                                            | */
/*	============================================================================  */

	/*		HELPER		 */

/*
 * function_identifier: Resizes an array of rbtNodes to best fit the contents
 * parameters: 			An array of rbtNodes and the expected length
 * return value:		The resized array of rbtNodes
*/
template <typename K, typename D>
rbtNode<K,D> **rbTree<K,D>::resize_helper(rbtNode<K,D> **nodeArray, size_t nodes) const {
		// Allocates new memory
	rbtNode<K,D> **tempPlace = nodeArray, **endArray;
	nodeArray = new rbtNode<K,D>*[nodes]; endArray = nodeArray + nodes;
		// Shallow copies values over
	while(nodeArray != endArray)
		*nodeArray++ = *tempPlace++;
		// Deletes old array
	delete [] (rbtNode<K,D> **)(nodePtr - nodes);
		// Returns new array's address
	return nodeArray - nodes;
}

	/*		NO FILTER		 */

/*
 * function_identifier: Returns an array of all rbtNodes, in the given red-black tree, via a traversal (no TEST_ORDER or PRINT_LEVEL)
 * parameters: 			A pointer to a red-black tree
 * return value:		An array of rbtNodes (caller's responsibility to clear it)
*/
template <typename K, typename D>
rbtNode<K,D> **rbTree<K,D>::rbt_getAllNodes(enum rbtraversal traversal) const {
		// Returns NULL if invaild
	if(!root || traversal == TEST_ORDER || traversal == PRINT_LEVEL)
		return NULL;
		// Allocates enough memory to hold all nodes in
	rbtNode<K,D> **nodeArray = new rbtNode<K,D>*[size];
	nodePtr = (void **)nodeArray;
		// Writes all the nodes into nodeArray, via an the given traversal
	switch(traversal) {
		case PRE_ORDER:			nodesPre(root); break;
		case IN_ORDER:			nodesIn(root); break;
		case POST_ORDER:		nodesPost(root); break;
		case LEVEL_ORDER:		nodesLevel(); break;
		default:;
	}
		// Returns the nodeArray
	return nodeArray;
}

/*
 * function_identifier: Returns an array of all rbtNodes, in the given red-black tree, via a traversal (no TEST_ORDER or PRINT_LEVEL)
 *						It uses the array given and does not resize (your fault if the array doesn't have enough space)
 * parameters: 			A pointer to a red-black tree
 * return value:		An array of rbtNodes (caller's responsibility to clear it)
*/
template <typename K, typename D>
rbtNode<K,D> **rbTree<K,D>::rbt_getAllNodes(rbtNode<K,D> **nodeArray, enum rbtraversal traversal) const {
		// Returns NULL if invaild
	if(!root || traversal == TEST_ORDER || traversal == PRINT_LEVEL)
		return NULL;
		// Coverts the array into void pointer
	nodePtr = (void **)nodeArray;
		// Writes all the nodes into nodeArray, via an the given traversal
	switch(traversal) {
		case PRE_ORDER:			nodesPre(root); break;
		case IN_ORDER:			nodesIn(root); break;
		case POST_ORDER:		nodesPost(root); break;
		case LEVEL_ORDER:		nodesLevel(); break;
		default:;
	}
		// Returns the nodeArray
	return nodeArray;
}

/*
 * function_identifier: Returns an array of all rbtNodes, in the given red-black tree, at a specified level
 * parameters: 			The level to extract data from and an address to return the number of nodes copied (return by reference)
 * return value:		An array of rbtNodes (caller's responsibility to clear it)
*/
template <typename K, typename D>
rbtNode<K,D> **rbTree<K,D>::rbt_getAllNodesAtLevel(size_t &nodes, size_t level) const {
		// Returns NULL if invaild
	if(!root) return NULL;
		// Allocates enough memory to hold all nodes in
	rbtNode<K,D> **nodeArray = new rbtNode<K,D>*[1 << level];
	nodePtr = (void **)nodeArray;
		// Writes all the nodes into nodeArray
	nodes = nodesSpecifiedLevel(level);

		// Checks if resize is needed, if so, resize array to match nodes copied
	if (nodes < ((size_t)1 << level))
		return resize_helper(nodeArray, nodes);

		// Returns the nodeArray
	return nodeArray;
}

/*
 * function_identifier: Returns an array of all rbtNodes, in the given red-black tree, at a specified level
 *						It uses the array given and does not resize (your fault if the array doesn't have enough space)
 * parameters: 			The level to extract data from and an address to return the number of nodes copied (return by reference)
 * return value:		An array of rbtNodes (caller's responsibility to clear it)
*/
template <typename K, typename D>
rbtNode<K,D> **rbTree<K,D>::rbt_getAllNodesAtLevel(rbtNode<K,D> **nodeArray, size_t &nodes, size_t level) const {
		// Returns NULL if invaild
	if(!root) return 0;
		// Coverts the array into void pointer
	nodePtr = (void **)nodeArray;
		// Writes all the nodes into nodeArray

	nodes = nodesSpecifiedLevel(level);

		// Returns the nodeArray
	return nodeArray;
}

	/*		KEYS	 */

/*
 * function_identifier: Returns an array of all rbtNodes that have the given key, in the given red-black tree, via a traversal (no TEST_ORDER)
 * parameters: 			A pointer to a red-black tree
 * return value:		An array of rbtNodes that have the given key (caller's responsibility to clear it)
*/
template <typename K, typename D>
rbtNode<K,D> **rbTree<K,D>::rbt_getAllNodesWithKey(K key, size_t &nodes, enum rbtraversal traversal, size_t level) const {
		// Returns NULL if invaild
	if(!root || traversal == TEST_ORDER)
		return NULL;
		// Branches off if PRINT_LEVEL
	if (traversal == PRINT_LEVEL) {
			// Allocates enough memory to hold all nodes in
		rbtNode<K,D> **nodeArray = new rbtNode<K,D>*[1 << level];
		nodePtr = (void **)nodeArray;
			// Writes all the nodes into nodeArray
		nodes = nodesSpecifiedLevelWithKey(level, key);
			// Checks if resize is needed, if so, resize array to match nodes copied
		if (nodes < ((size_t)1 << level))
			return resize_helper(nodeArray, nodes);
			// Returns the nodeArray
		return nodeArray;
	}
		// Allocates enough memory to hold all nodes in
	rbtNode<K,D> **nodeArray = new rbtNode<K,D>*[size];
	nodePtr = (void **)nodeArray;
		// Writes all the nodes into nodeArray, via an the given traversal
	switch(traversal) {
		case PRE_ORDER:			nodesPreWithKey(root, key); break;
		case IN_ORDER:			nodesInWithKey(root, key); break;
		case POST_ORDER:		nodesPostWithKey(root, key); break;
		case LEVEL_ORDER:		nodesLevelWithKey(key); break;
		default:;
	}
		// Calculate the number of nodes put into the return array
	nodes = (nodePtr - (void **)nodeArray);
		// Checks if resize is needed, if so, resize array to match nodes copied
	if (nodes < size)
		return resize_helper(nodeArray, nodes);
		// Returns the nodeArray
	return nodeArray;
}

/*
 * function_identifier: Returns an array of all rbtNodes that have the given key, in the given red-black tree, via a traversal (no TEST_ORDER)
 *						It uses the array given and does not resize (your fault if the array doesn't have enough space)
 * parameters: 			A pointer to a red-black tree
 * return value:		An array of rbtNodes that have the given key (caller's responsibility to clear it)
*/
template <typename K, typename D>
rbtNode<K,D> **rbTree<K,D>::rbt_getAllNodesWithKey(rbtNode<K,D> **nodeArray, K key, size_t &nodes, enum rbtraversal traversal, size_t level) const {
		// Returns NULL if invaild
	if(!root || traversal == TEST_ORDER)
		return NULL;
		// Coverts the array into void pointer
	nodePtr = (void **)nodeArray;
		// Branches off if PRINT_LEVEL
	if (traversal == PRINT_LEVEL) {
			// Writes all the nodes into nodeArray
		nodes = nodesSpecifiedLevelWithKey(level, key);
			// Returns the nodeArray
		return nodeArray;
	}
		// Writes all the nodes into nodeArray, via an the given traversal
	switch(traversal) {
		case PRE_ORDER:			nodesPreWithKey(root, key); break;
		case IN_ORDER:			nodesInWithKey(root, key); break;
		case POST_ORDER:		nodesPostWithKey(root, key); break;
		case LEVEL_ORDER:		nodesLevelWithKey(key); break;
		default:;
	}
		// Calculate the number of nodes put into the return array
	nodes = (nodePtr - (void **)nodeArray);
		// Returns the nodeArray
	return nodeArray;
}

	/*		DATA	 */

/*
 * function_identifier: Returns an array of all rbtNodes that have the given data, in the given red-black tree, via a traversal (no TEST_ORDER)
 * parameters: 			A pointer to a red-black tree
 * return value:		An array of rbtNodes that have the given data (caller's responsibility to clear it)
*/
template <typename K, typename D>
rbtNode<K,D> **rbTree<K,D>::rbt_getAllNodesWithData(D data, size_t &nodes, enum rbtraversal traversal, size_t level) const {
		// Returns NULL if invaild
	if(!root || traversal == TEST_ORDER)
		return NULL;
		// Branches off if PRINT_LEVEL
	if (traversal == PRINT_LEVEL) {
			// Allocates enough memory to hold all nodes in
		rbtNode<K,D> **nodeArray = new rbtNode<K,D>*[1 << level];
		nodePtr = (void **)nodeArray;
			// Writes all the nodes into nodeArray
		nodes = nodesSpecifiedLevelWithData(level, data);
			// Checks if resize is needed, if so, resize array to match nodes copied
		if (nodes < ((size_t)1 << level))
			return resize_helper(nodeArray, nodes);
			// Returns the nodeArray
		return nodeArray;
	}
		// Allocates enough memory to hold all nodes in
	rbtNode<K,D> **nodeArray = new rbtNode<K,D>*[size];
	nodePtr = (void **)nodeArray;
		// Writes all the nodes into nodeArray, via an the given traversal
	switch(traversal) {
		case PRE_ORDER:			nodesPreWithData(root, data); break;
		case IN_ORDER:			nodesInWithData(root, data); break;
		case POST_ORDER:		nodesPostWithData(root, data); break;
		case LEVEL_ORDER:		nodesLevelWithData(data); break;
		default:;
	}
		// Calculate the number of nodes put into the return array
	nodes = (nodePtr - (void **)nodeArray);
		// Checks if resize is needed, if so, resize array to match nodes copied
	if (nodes < size)
		return resize_helper(nodeArray, nodes);
		// Returns the nodeArray
	return nodeArray;
}

/*
 * function_identifier: Returns an array of all rbtNodes that have the given data, in the given red-black tree, via a traversal (no TEST_ORDER)
 *						It uses the array given and does not resize (your fault if the array doesn't have enough space)
 * parameters: 			A pointer to a red-black tree
 * return value:		An array of rbtNodes that have the given data (caller's responsibility to clear it)
*/
template <typename K, typename D>
rbtNode<K,D> **rbTree<K,D>::rbt_getAllNodesWithData(rbtNode<K,D> **nodeArray, D data, size_t &nodes, enum rbtraversal traversal, size_t level) const {
		// Returns NULL if invaild
	if(!root || traversal == TEST_ORDER)
		return NULL;
		// Coverts the array into void pointer
	nodePtr = (void **)nodeArray;
		// Branches off if PRINT_LEVEL
	if (traversal == PRINT_LEVEL) {
			// Writes all the nodes into nodeArray
		nodes = nodesSpecifiedLevelWithData(level, data);
			// Returns the nodeArray
		return nodeArray;
	}
		// Writes all the nodes into nodeArray, via an the given traversal
	switch(traversal) {
		case PRE_ORDER:			nodesPreWithData(root, data); break;
		case IN_ORDER:			nodesInWithData(root, data); break;
		case POST_ORDER:		nodesPostWithData(root, data); break;
		case LEVEL_ORDER:		nodesLevelWithData(data); break;
		default:;
	}
		// Calculate the number of nodes put into the return array
	nodes = (nodePtr - (void **)nodeArray);
		// Returns the nodeArray
	return nodeArray;
}

	/*		DATAKEYS	 */

/*
 * function_identifier: Returns an array of all rbtNodes that have the given data and key, in the given red-black tree, via a traversal (no TEST_ORDER)
 * parameters: 			A pointer to a red-black tree
 * return value:		An array of rbtNodes that have the given data (caller's responsibility to clear it)
*/
template <typename K, typename D>
rbtNode<K,D> **rbTree<K,D>::rbt_getAllNodesWithDataKey(D data, K key, size_t &nodes, enum rbtraversal traversal, size_t level) const {
		// Returns NULL if invaild
	if(traversal == TEST_ORDER)
		return NULL;
		// Branches off if PRINT_LEVEL
	if (traversal == PRINT_LEVEL) {
			// Allocates enough memory to hold all nodes in
		rbtNode<K,D> **nodeArray = new rbtNode<K,D>*[1 << level];
		nodePtr = (void **)nodeArray;
			// Writes all the nodes into nodeArray
		nodes = nodesSpecifiedLevelWithDataKey(level, data, key);
			// Checks if resize is needed, if so, resize array to match nodes copied
		if (nodes < ((size_t)1 << level))
			return resize_helper(nodeArray, nodes);
			// Returns the nodeArray
		return nodeArray;
	}
		// Find the key to start traversing from
	rbtNode<K,D> *curr;
	if(!(curr = rbt_searchKey(key)))
		return NULL;
		// Allocates enough memory to hold all nodes in
	rbtNode<K,D> **nodeArray = new rbtNode<K,D>*[size];
	nodePtr = (void **)nodeArray;
		// Writes all the nodes into nodeArray, via an the given traversal
	switch(traversal) {
		case PRE_ORDER:			nodesPreWithDataKey(curr, data, key); break;
		case IN_ORDER:			nodesInWithDataKey(curr, data, key); break;
		case POST_ORDER:		nodesPostWithDataKey(curr, data, key); break;
		case LEVEL_ORDER:		nodesLevelWithDataKey(data, key); break;
		default:;
	}
		// Calculate the number of nodes put into the return array
	nodes = (nodePtr - (void **)nodeArray);
		// Checks if resize is needed, if so, resize array to match nodes copied
	if (nodes < size)
		return resize_helper(nodeArray, nodes);
		// Returns the nodeArray
	return nodeArray;
}

/*
 * function_identifier: Returns an array of all rbtNodes that have the given data and key, in the given red-black tree, via a traversal (no TEST_ORDER)
 *						It uses the array given and does not resize (your fault if the array doesn't have enough space)
 * parameters: 			A pointer to a red-black tree
 * return value:		An array of rbtNodes that have the given data (caller's responsibility to clear it)
*/
template <typename K, typename D>
rbtNode<K,D> **rbTree<K,D>::rbt_getAllNodesWithDataKey(rbtNode<K,D> **nodeArray, D data, K key, size_t &nodes, enum rbtraversal traversal, size_t level) const {
		// Returns NULL if invaild
	if(traversal == TEST_ORDER)
		return NULL;
		// Coverts the array into void pointer
	nodePtr = (void **)nodeArray;
		// Branches off if PRINT_LEVEL
	if (traversal == PRINT_LEVEL) {
			// Writes all the nodes into nodeArray
		nodes = nodesSpecifiedLevelWithDataKey(level, data, key);
			// Returns the nodeArray
		return nodeArray;
	}
		// Find the key to start traversing from
	rbtNode<K,D> *curr;
	if(!(curr = rbt_searchKey(key)))
		return NULL;
		// Writes all the nodes into nodeArray, via an the given traversal
	switch(traversal) {
		case PRE_ORDER:			nodesPreWithDataKey(curr, data, key); break;
		case IN_ORDER:			nodesInWithDataKey(curr, data, key); break;
		case POST_ORDER:		nodesPostWithDataKey(curr, data, key); break;
		case LEVEL_ORDER:		nodesLevelWithDataKey(data, key); break;
		default:;
	}
		// Calculate the number of nodes put into the return array
	nodes = (nodePtr - (void **)nodeArray);
		// Returns the nodeArray
	return nodeArray;
}


/*	============================================================================  */
/* |                                                                            | */
/* |                     	     GET ALL VALUES                                 | */
/* |                                                                            | */
/*	============================================================================  */

	/*		KEYS	 */

/*
 * function_identifier: Places all the keys in the red-black tree, into a global array, via an inorder traversal
 * parameters: 			A pointer to a node in the red-black tree
 * return value:		An array of data values in void pointer format (caller's responsibility to clear it)
*/
template <typename K, typename D>
void rbTree<K,D>::getAllKeys(rbtNode<K,D> *curr) const {
		// Performs inorder traversal
	if(curr->left) getAllKeys(curr->left);
		// Saves the key at the current index
	*(K *)basePtr = curr->key;
		// Increments the global pointer index
	basePtr += sizeof(K);
	if(curr->right) getAllKeys(curr->right);
}

/*
 * function_identifier: Returns all the keys in the red-black tree via an inorder traversal
 * parameters: 			A keysArray (optional. If none provided, one is created)
 * return value:		An array of data values in void pointer format (caller's responsibility to clear it)
*/
template <typename K, typename D>
K *rbTree<K,D>::rbt_getAllKeys(K *keysArray) const {
		// Returns NULL if no nodes in tree
	if(!size)
		return NULL;

		// If no keysArray provided, allocates memory to a new key array
	if (!keysArray)
		keysArray = new K[size];
		// Aligns the basePtr to the keysArray
	basePtr = (std::byte *)keysArray;

		// Writes all keys into allocated memory
	getAllKeys(root);

		// Returns keys
	return keysArray;
}

	/*		DATA	 */

/*
 * function_identifier: Places all the keys in the red-black tree, into a global array, via an inorder traversal
 * parameters: 			A pointer to a node in the red-black tree
 * return value:		An array of data values in void pointer format (caller's responsibility to clear it)
*/
template <typename K, typename D>
void rbTree<K,D>::getAllData(rbtNode<K,D> *curr) const {
		// Performs inorder traversal
	if(curr->left) getAllData(curr->left);
		// Saves the key at the current index
	*(D *)basePtr = curr->data;
		// Increments the global pointer index
	basePtr += sizeof(D);
	if(curr->right) getAllData(curr->right);
}

/*
 * function_identifier: Returns all the keys in the red-black tree via an inorder traversal
 * parameters: 			N/A
 * return value:		An array of data values in void pointer format (caller's responsibility to clear it)
*/
template <typename K, typename D>
D *rbTree<K,D>::rbt_getAllData(D *dataArray) const {
		// Returns NULL if no nodes in tree
	if(!size)
		return NULL;

		// If no keysArray provided, allocates memory to a new key array
	if (!dataArray)
		dataArray = new K[size];
		// Aligns the basePtr to the dataArray
	basePtr = (std::byte *)dataArray;

		// Writes all data into allocated memory
	getAllData(root);

		// Returns data
	return dataArray;
}


/*	============================================================================  */
/* |                                                                            | */
/* |                     	       TREE STATS                                   | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: Counts all the nodes, in the red-black tree, that have the given key
 * parameters: 			A pointer the current considered node and the given key to check for
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::countKey(rbtNode<K,D> *curr, K key) const {
		// Performs inorder traversal
	if(curr->left) countKey(curr->left, key);
		// Checks if found node with requested key
	if (curr->key == key)
			// Increments the global pointer index
		basePtr++;
	if(curr->right) countKey(curr->right, key);
}

/*
 * function_identifier: Counts all the nodes, in the red-black tree, that have the given key
 * parameters: 			The given key to check for
 * return value:		The count of all rbtNodes, in the red-black tree, that have the given key
*/
template <typename K, typename D>
size_t rbTree<K,D>::rbt_countKey(K key) const {
		// Resets the counter
	basePtr = 0;
		// Counts the number of nodes with the given key
	countKey(root, key);
		// Returns the result
	return *((size_t*)(&basePtr));
}

/*
 * function_identifier: Counts all the nodes, in the red-black tree, that have the given data
 * parameters: 			A pointer the current considered node and the given data to check for
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::countData(rbtNode<K,D> *curr, D data) const {
		// Performs inorder traversal
	if(curr->left) countData(curr->left, data);
		// Checks if found node with requested data
	if (curr->data == data)
			// Increments the global pointer index
		basePtr++;
	if(curr->right) countData(curr->right, data);
}

/*		
 * function_identifier: Counts all the nodes, in the red-black tree, that have the given data
 * parameters: 			The given data to check for
 * return value:		The count of all rbtNodes, in the red-black tree, that have the given data
*/
template <typename K, typename D>
size_t rbTree<K,D>::rbt_countData(D data) const {
		// Resets the counter
	basePtr = 0;
		// Counts the number of nodes with the given data
	countData(root, data);
		// Returns the result
	return *((size_t*)(&basePtr));
}

/*
 * function_identifier: Counts all the nodes, in the red-black tree, that have the given key and data
 * parameters: 			A pointer the current considered node and the given key and data to check for
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::countKeyData(rbtNode<K,D> *curr, K key, D data) const {
		// Performs inorder traversal
	if(curr->left) countKeyData(curr->left, key, data);
		// Checks if found node with requested key and data
	if (curr->key == key && curr->data == data)
			// Increments the global pointer index
		basePtr++;
	if(curr->right) countKeyData(curr->right, key, data);
}

/*
 * function_identifier: Counts all the nodes, in the red-black tree, that have the given key and data
 * parameters: 			The given key and data to check for
 * return value:		The count of all rbtNodes, in the red-black tree, that have the given key and data
*/
template <typename K, typename D>
size_t rbTree<K,D>::rbt_countKeyData(K key, D data) const {
		// Resets the counter
	basePtr = 0;
		// Counts the number of nodes with the given key
	countKeyData(root, key, data);
		// Returns the result
	return *((size_t*)(&basePtr));
}

#endif /* _rbt_TRAVERSAL */