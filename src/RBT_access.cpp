#ifndef _rbt_ACCESS
#define _rbt_ACCESS
#include	"RBT_main.h"
#include	"RBT_access.h"
#include	"RBT_sort.h"
#include	<stdexcept>

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
	inline size_t *widths;

		// External From 'RBT_sort.cpp'
	template <typename K, typename D>
	extern inline bool(*fooPtr)(rbtNode<K,D> *, rbtNode<K,D> *);
}

/*	============================================================================  */
/* |                                                                            | */
/* |                     	       TREE STATS                                   | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: Returns the height of the current red-black tree (used by rbt_getHeightExact())
 * parameters: 			A node pointer to recursively count from
 * return value:		The red-black tree's height
*/
template <typename K, typename D>
size_t rbTree<K,D>::getHeight(rbtNode<K,D> *curr) const {
		// Checks if current node has left branch
	if(curr->left) {
			// If so, checks if current node has right branch
		if(curr->right) {
			// If so, computes height of both branches and returns the larger one

				// Finds the height of the left branch through recursion
			size_t lheight = getHeight(curr->left);
				// Finds the height of the right branch through recursion
			size_t rheight = getHeight(curr->right);

				// Returns the larger height between the two branches
			return (lheight > rheight ? lheight : rheight)+1;
		} else
				// Else, return the height of the left branch
			return getHeight(curr->left);
	}
		// Else, checks if current node has right branch
	else if(curr->right)
		// If so, return the height of the right branch
		return getHeight(curr->right);

		// Else, this is a leaf node. Thus, return 1
	else
		return 1;
}

/*
 * function_identifier: Returns the height of the current red-black tree
 * parameters: 			N/A
 * return value:		The red-black tree's height
*/
template <typename K, typename D>
size_t rbTree<K,D>::rbt_getHeightExact() const {
	return root ? getHeight(root) : 0;
}

/*
 * function_identifier: Returns the maximum/estimated height of the current red-black tree (faster)
 * parameters: 			N/A
 * return value:		The red-black tree's maximum height
*/
template <typename K, typename D>
size_t rbTree<K,D>::rbt_getHeight() const {
		// Initializes values
	size_t check = size, height = 0;

		// Finds the value of log_2(size)
	while(check) {
		height++;
		check >>= 1;
	}

		// Returns the estimated height
	return height << 1;
}

/*
 * function_identifier: Returns the width of all levels in the current red-black tree in an array (caller's responsibility to clear it)
 * parameters: 			Variable to return the length of the outputted array in
 * return value:		An array of size_t widths, for each level on the red-black tree
*/
template <typename K, typename D>
size_t *rbTree<K,D>::rbt_getWidths(size_t &len) const {
		// Checks if the root exists
	if (root) {
			// If so, get the widths on all levels
		len = rbt_getHeightExact();
		widths = new size_t[len]();
		getWidth(root, 0);

			// Returns the array of widths
		return widths;
	} else
			// If not, return NULL instead of an array
		return NULL;
}

/*
 * function_identifier: Returns the width of the current red-black tree (used by rbt_getWidthExact())
 * parameters: 			A node pointer to recursively count from and the current level
 * return value:		The red-black tree's width
*/
template <typename K, typename D>
void rbTree<K,D>::getWidth(rbtNode<K,D> *curr, size_t level) const {
		// Recursively calls the function again on the left child, if it exists
	if(curr->left) getWidth(curr->left, level + 1);
		// Increases the recorded width, of this level, in the global variable
	widths[level]++;
		// Recursively calls the function again on the right child, if it exists
	if(curr->right) getWidth(curr->right, level + 1);
}

/*
 * function_identifier: Returns the width of the current red-black tree
 * parameters: 			N/A
 * return value:		The red-black tree's maximum width
*/
template <typename K, typename D>
size_t rbTree<K,D>::rbt_getWidthExact() const {
	size_t len;
		// Gets an array of size_t widths, for each level on the red-black tree
	if (!rbt_getWidths(len))
		// returns 0 if no array was possible to recreate
		return 0;

		// Gets the maximum width from the array
	size_t max = 0;
	for(size_t i = 0; i < len; i++)
		if (max < widths[i])
			max = widths[i];

		// Deletes the now-unneeded array
	delete [] widths;

		// Returns the max width
	return max;
}

/*
 * function_identifier: Returns the maximum/estimated width of the current red-black tree (faster)
 * parameters: 			N/A
 * return value:		The red-black tree's maximum width
*/
template <typename K, typename D>
size_t rbTree<K,D>::rbt_getWidth() const {
	return (size >> 1) + 1;
}


/*
 * function_identifier: Returns the leaf node count of the current red-black tree (used by rbt_getLeafNodes())
 * parameters: 			A node pointer to recursively count from
 * return value:		The leaf node count from the provided curr node pointer
*/
template <typename K, typename D>
size_t rbTree<K,D>::getLeafNodes(rbtNode<K,D> *curr) const {
		// Checks if current node has left branch
	if(curr->left) {
			// If so, checks if current node has right branch
		if(curr->right)
				// If so, add together the total children on each of the two branches and return
			return getLeafNodes(curr->left) + getLeafNodes(curr->right);
		else
				// Else, return the total children on the left branch
			return getLeafNodes(curr->left);
	}
		// Else, return the total children on the right branch
	else if(curr->right)
		return getLeafNodes(curr->right);
		// Else, this is a leaf node. Thus, return 1
	else return 1;
}

/*
 * function_identifier: Returns the leaf node count of the current red-black tree
 * parameters: 			N/A
 * return value:		The red-black tree's leaf node count
*/
template <typename K, typename D>
size_t rbTree<K,D>::rbt_getLeafNodes() const {
	return root ? getLeafNodes(root) : 0;
}

/*
 * function_identifier: Returns the inner node count of the current red-black tree
 * parameters: 			N/A
 * return value:		The red-black tree's inner node count
*/
template <typename K, typename D>
size_t rbTree<K,D>::rbt_getInnerNodes() const {
	return root ? size - getLeafNodes(root) : 0;
}

/*	============================================================================  */
/* |                                                                            | */
/* |                     	       SINGLE NODE                                  | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: The base function used in finding the index, of a node, in its related red-black tree
 * parameters: 			A node pointer
 * return value:		The idex of the node
*/
template <typename K, typename D>
inline size_t rbTree<K,D>::findIdxBase(rbtNode<K,D> *curr) const {
		// If the node doesn't exist, give an error
	if (!curr)
		throw std::out_of_range("node with given key and data is not within Red-Black Tree'");

		// Gets the starting index to build on
	size_t idx = curr->left ? curr->left->descendants + 1 : 0;

		// Moves up through the tree, until reaches the root node
	while(curr->parent) {
			// Checks if the current node is a right child
		if (curr->parent->right == curr)
				// If so, add to the index the number of descendants on the left side of the parent
			idx += curr->parent->descendants - curr->descendants;
		curr = curr->parent;
	}

		// Return the index
	return idx;
}

/*
 * function_identifier: Finds the index of the node, closet to the top of the red-black tree, with the given key and data
 * parameters: 			Key and data values
 * return value:		The index value of a node in the red-black tree
*/
template <typename K, typename D>
inline size_t rbTree<K,D>::rbt_findIdx(K key, D data) const {
	return findIdxBase(rbt_search(key, data));
}

/*
 * function_identifier: Finds the index of the node, closet to the top of the red-black tree, equal to the given node
 * parameters: 			A node
 * return value:		The index value of a node in the red-black tree
*/
template <typename K, typename D>
inline size_t rbTree<K,D>::rbt_findIdx(rbtNode<K,D> *target) const {
	return findIdxBase(rbt_search(target->key, target->data));
}

/*
 * function_identifier: Finds the index of the node, closet to the top of the red-black tree, with the given key
 * parameters: 			Key value
 * return value:		The index value of a node in the red-black tree
*/
template <typename K, typename D>
inline size_t rbTree<K,D>::rbt_findIdxKey(K key) const {
	return findIdxBase(rbt_searchKey(key));
}

/*
 * function_identifier: Finds the index of the node, closet to the top of the red-black tree, with the given data
 * parameters: 			Data value
 * return value:		The index value of a node in the red-black tree
*/
template <typename K, typename D>
inline size_t rbTree<K,D>::rbt_findIdxData(D data) const {
	return findIdxBase(rbt_searchData(data));
}

/*
 * function_identifier: Returns the node located at the given index in the red-black tree
 * parameters: 			The index of the node to locate
 * return value:		The node at the given index in the red-black tree
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::rbt_nodeAt(size_t indx) const {
		// If given an invaild index, throw error
	if (indx >= size)
		throw std::out_of_range("node index value '" + std::to_string(indx) + "' is not vaild in Red-Black Tree of size '" + std::to_string(size) + "'");

		// Sets the current rbtNode to be the red-black tree's root
	rbtNode<K,D> *curr = root;

		// Continues to loop until the requested value is found
	while (true) {
			// Checks if the left child exists
		if (curr->left) {
				// If the left child does exists, check if the target index is less than or equal to it's descendants
			if (indx <= curr->left->descendants) {
					// If so, move to the left child and repeat
				curr = curr->left; continue;
			}
				// Else, shrink the scope of our search to the current node and the right child only
			indx -= curr->left->descendants + 1;
		}
			// Returns the current node if it is the one we are looking for
		if (indx == 0) return curr;

			// Else, right child must exist and the target node much exist somewhere on the path
		curr = curr->right;
		indx--;
	}
}

/*	============================================================================  */
/* |                                                                            | */
/* |                     	       SEARCH NODE                                  | */
/* |                                                                            | */
/*	============================================================================  */

	/*		KEYS	  */

/*
 * function_identifier: Finds a node, close to the top of the red-black tree, with the given key
 * parameters: 			The requested key
 * return value:		A pointer to the node, with the given key value, in the given red-black tree
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::rbt_searchKey(K key) const {
	rbtNode<K,D> *curr = root;
	while(1) {
			// Return curr if either key found or no node with key is in red-black tree
		if (!curr || curr->key == key) return curr;
			// If key is less than current node's key, consider the current node's left child's key
		if (key < curr->key) {
			curr = curr->left;
			continue;
		}
			// If key is greater than current node's key, consider the current node's right child's key
		curr = curr->right;
	}
}

/*
 * function_identifier: Finds a node, close to the top of the red-black tree, with the given key
 * parameters: 			The node with a key value
 * return value:		A pointer to the node, with the given key value, in the given red-black tree
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::rbt_searchKey(rbtNode<K,D> *target) const {
	return rbt_searchKey(target->key);
}

/*
 * function_identifier: Finds the node, in a given red-black tree, with the largest key value
 * parameters: 			N/A
 * return value:		A pointer to the node, with the largest key value, in the given red-black tree
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::rbt_maxKey() const {
		// If root doesn't exit, return NULL
	if (!root)
		return NULL;

		// Initializes needed variable
	rbtNode<K,D> *curr = root;

		// Finds the node with largest key
	while(curr->right) curr = curr->right;

		// Returns the node with largest key
	return curr;
}

/*
 * function_identifier: Finds the node, in a given red-black tree, with the smallest key value
 * parameters: 			N/A
 * return value:		A pointer to the node, with the smallest key value, in the given red-black tree
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::rbt_minKey() const {
		// If root doesn't exit, return NULL
	if (!root)
		return NULL;

		// Initializes needed variable
	rbtNode<K,D> *curr = root;

		// Finds the node with smallest key
	while(curr->left) curr = curr->left;

		// Returns the node with smallest key
	return curr;
}

	/*		DATA	 */

/*
 * function_identifier: Finds a node, in the red-black tree, with the given data (used by rbt_searchData())
 * parameters: 			The currently considered node and the requested data
 * return value:		A pointer to the node, with the given data value, in the given red-black tree
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::searchData(rbtNode<K,D> *curr, D data) const {
		// Returns curr node if data is found
	if (curr->data == data)
		return curr;

		// Initializes needed variables
	rbtNode<K,D> *curr2;

	// Checks curr node's branches
		// Checks left branch
	if (curr->left && (curr2 = searchData(curr->left, data)))
			// If node with data found, return
		return curr2;

		// If data still not found, search the right branch
	if (curr->right)
			// If node with data found, return
		return searchData(curr->right, data);

		// If neither branch found a node with the given data, return NULL
	return NULL;
}

/*
 * function_identifier: Finds a node, in the red-black tree, with the given data
 * parameters: 			The requested data
 * return value:		A pointer to the node, with the given data value, in the given red-black tree
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::rbt_searchData(D data) const {
		// Return search result for the data, or NULL if the red-black tree is empty
	return root ? searchData(root, data) : NULL;
}

/*
 * function_identifier: Finds a node, in the red-black tree, with the given data
 * parameters: 			A node with data 
 * return value:		A pointer to the node, with the given data value, in the given red-black tree
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::rbt_searchData(rbtNode<K,D> *target) const {
	return rbt_searchData(target->data);
}

/*
 * function_identifier: Finds the node, in a given red-black tree, with the largest data value (used by rbt_maxData())
 * parameters: 			The currently considered node
 * return value:		A pointer to the node, with the largest data value, in the given red-black tree
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::dataMax(rbtNode<K,D> *curr) const {
		// Initializes needed variables
	rbtNode<K,D> *maxNode = curr;

		// Checks if left branch exists
	if (curr->left) {
			// If so, compair the largest data value, on the left branch, with the current node's data value
		maxNode = dataMax(curr->left);
		if (maxNode->data < curr->data)
				// If maxNode < curr, set the maxNode to be curr
			maxNode = curr;
		else
				// Else, set curr for next check
			curr = maxNode;
	}
		// Checks if right branch exists
	if (curr->right) {
			// If so, compair the largest data value, on the left branch, with the current node's data value
		maxNode = dataMax(curr->right);
		if (maxNode->data < curr->data)
				// If maxNode < curr, set the maxNode to be curr
			maxNode = curr;
	}

		// Finally, return the maximum
	return maxNode;
}

/*
 * function_identifier: Finds the node, in a given red-black tree, with the largest data value
 * parameters: 			N/A
 * return value:		A pointer to the node, with the largest data value, in the given red-black tree
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::rbt_maxData() const {
		// Return max-data node, or NULL if the red-black tree is empty
	return root ? dataMax(root) : NULL;
}

/*
 * function_identifier: Finds the node, in a given red-black tree, with the smallest data value (used by rbt_minData())
 * parameters: 			The currently considered node
 * return value:		A pointer to the node, with the smallest data value, in the given red-black tree
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::dataMin(rbtNode<K,D> *curr) const {
		// Initializes needed variables
	rbtNode<K,D> *minNode = curr;

		// Checks if left branch exists
	if (curr->left) {
			// If so, compair the smallest data value, on the left branch, with the current node's data value
		minNode = dataMin(curr->left);
		if (minNode->data > curr->data)
				// If minNode > curr, set the minNode to be curr
			minNode = curr;
		else
				// Else, set curr for next checking
			curr = minNode;
	}
		// Checks if right branch exists
	if (curr->right) {
			// If so, compair the smallest data value, on the left branch, with the current node's data value
		minNode = dataMin(curr->right);
		if (minNode->data > curr->data)
				// If minNode > curr, set the minNode to be curr
			minNode = curr;
	}

		// Finally, return the minimum
	return minNode;
}

/*
 * function_identifier: Finds the node, in a given red-black tree, with the smallest data value
 * parameters: 			N/A
 * return value:		A pointer to the node, with the smallest data value, in the given red-black tree
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::rbt_minData() const {
		// Return min-data node, or NULL if the red-black tree is empty
	return root ? dataMin(root) : NULL;
}

	/*		DATA KEYS	  */

/*
 * function_identifier: Finds the node, in a given red-black tree, with the given key and data (used by rbt_search())
 * parameters: 			The currently considered node as well as the data and key values to search for
 * return value:		A pointer to the node, with the given key and data values, in the given red-black tree
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::search(rbtNode<K,D> *curr, K key, D data) const {
		// If this node has the data criteria, then return this node
	if (curr->data == data)
		return curr;

		// Else, initialize a temp variable
	rbtNode<K,D> *curr2 = NULL;

		// Checks if the left branch both exists and has the correct key
			// If so, search that branch for the data
	if (curr->left && curr->left->key == key)
		curr2 = search(curr->left, key, data);

		// If no node not yet found, then check if the right branch both exists and has the correct key
			// If so, search that branch for the data
	else if (curr->right && curr->right->key == key)
		curr2 = search(curr->right, key, data);
		// Return whatever is found
	return curr2;
}

/*
 * function_identifier: Finds the node, in a given red-black tree, with the given key and data
 * parameters: 			The data and key values to search for
 * return value:		A pointer to the node, with the given key and data values, in the given red-black tree
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::rbt_search(K key, D data) const {
		// Initializes needed variable
	rbtNode<K,D> *curr;

		// Returns NULL if no node with the requested key is in red-black tree
	if(!(curr = rbt_searchKey(key)))
		return NULL;

		// Finds the client-requested node
	return search(curr, key, data);
}

/*
 * function_identifier: Finds the node, in a given red-black tree, with the given key and data
 * parameters: 			A node with data and key values to search for
 * return value:		A pointer to the node, with the given key and data values, in the given red-black tree
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::rbt_search(rbtNode<K,D> *target) const {
	return rbt_search(target->key, target->data);
}

/*
 * function_identifier: Finds all nodes with the max key and then returns the node, among those, with
 *						the max data
 * parameters: 			N/A
 * return value:		An rbtNode pointer
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::rbt_maxKey_maxData() const {
	return rbt_maxKey();
}

/*
 * function_identifier: Checks all child of the current node, with the same provided key, and
 *						finds the first one with the min data value among them (called by rbt_maxKey_minData())
 * parameters: 			N/A
 * return value:		An rbtNode pointer
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::key_minData(rbtNode<K,D> *curr, K key) const {
	rbtNode<K,D> *curr2 = curr, *temp;

	if (curr->left){
		temp = key_minData(curr->left, key);
		if (temp->key == key && curr2->data > temp->data)
			curr2 = temp;
	}
	if (curr->right) {
		temp = key_minData(curr->right, key);
		if (temp->key == key && curr2->data > temp->data)
			curr2 = temp;
	}

	return curr2;
}

/*
 * function_identifier: Finds all nodes with the max key and then returns the node, among those, with
 *						the min data
 * parameters: 			N/A
 * return value:		An rbtNode pointer
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::rbt_maxKey_minData() const {
	rbtNode<K,D> *curr = root;

	while(curr->right)
		curr = curr->right;
	while(curr->parent && curr->parent->key == curr->key)
		curr = curr->parent;
	return key_minData(curr, curr->key);
}

/*
 * function_identifier: Checks all child of the current node, with the same provided key, and
 *						finds the first one with the max data value among them (called by rbt_minKey_maxData())
 * parameters: 			N/A
 * return value:		An rbtNode pointer
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::key_maxData(rbtNode<K,D> *curr, K key) const {
	rbtNode<K,D> *curr2 = curr, *temp;

	if (curr->left){
		temp = key_maxData(curr->left, key);
		if (temp->key == key && curr2->data < temp->data)
			curr2 = temp;
	}
	if (curr->right) {
		temp = key_maxData(curr->right, key);
		if (temp->key == key && curr2->data < temp->data)
			curr2 = temp;
	}

	return curr2;
}

/*
 * function_identifier: Finds all nodes with the min key and then returns the node, among those, with
 *						the max data
 * parameters: 			N/A
 * return value:		An rbtNode pointer
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::rbt_minKey_maxData() const {
	rbtNode<K,D> *curr = root;

	while(curr->left)
		curr = curr->left;
	while(curr->parent && curr->parent->key == curr->key)
		curr = curr->parent;
	return key_maxData(curr, curr->key);
}

/*
 * function_identifier: Finds all nodes with the min key and then returns the node, among those, with
 *						the min data
 * parameters: 			N/A
 * return value:		An rbtNode pointer
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::rbt_minKey_minData() const {
	return rbt_minKey();
}

/*
 * function_identifier: Finds all nodes with the max data and then returns the node, among those, with
 *						the max key
 * parameters: 			N/A
 * return value:		An rbtNode pointer
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::rbt_maxData_maxKey() const {
	rbtNode<K,D> *curr, **rbtNodes = rbt_getAllNodes();
	
	fooPtr<K,D> = dataKeyCompair;
	mergeSortCallerBase(rbtNodes, 0, size - 1);
	curr = rbtNodes[size - 1];

	delete [] rbtNodes;

	return curr;
}

/*
 * function_identifier: Finds all nodes with the min data and then returns the node, among those, with
 *						the max key
 * parameters: 			N/A
 * return value:		An rbtNode pointer
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::rbt_minData_maxKey() const {
	rbtNode<K,D> *curr, **rbtNodes = rbt_getAllNodes();
	
	fooPtr<K,D> = dataKeyCompair;
	mergeSortCallerBase(rbtNodes, 0, size - 1);

	curr = rbtNodes[0];
	for(size_t idx = 0; idx <= size; idx++)
		if (idx == size)
			curr = rbtNodes[idx];
		else if (curr->key != rbtNodes[idx]->key) {
			curr = rbtNodes[idx];
			break;
		}

	delete [] rbtNodes;

	return curr;
}

/*
 * function_identifier: Finds all nodes with the max data and then returns the node, among those, with
 *						the min key
 * parameters: 			N/A
 * return value:		An rbtNode pointer
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::rbt_maxData_minKey() const {
	rbtNode<K,D> *curr, **rbtNodes = rbt_getAllNodes();
	
	fooPtr<K,D> = dataKeyCompair;
	mergeSortCallerBase(rbtNodes, 0, size - 1);

	curr = rbtNodes[size - 1];
	for(size_t idx = size - 1; idx != (size_t)-1; idx--)
		if (idx == (size_t)-1)
			curr = rbtNodes[idx];
		else if (curr->key != rbtNodes[idx]->key) {
			curr = rbtNodes[idx];
			break;
		}

	delete [] rbtNodes;

	return curr;
}

/*
 * function_identifier: Finds all nodes with the min data and then returns the node, among those, with
 *						the min key
 * parameters: 			N/A
 * return value:		An rbtNode pointer
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::rbt_minData_minKey() const {
	rbtNode<K,D> *curr, **rbtNodes = rbt_getAllNodes();
	
	fooPtr<K,D> = dataKeyCompair;
	mergeSortCallerBase(rbtNodes, 0, size - 1);
	curr = rbtNodes[0];

	delete [] rbtNodes;

	return curr;
}

#endif /* _rbt_ACCESS */