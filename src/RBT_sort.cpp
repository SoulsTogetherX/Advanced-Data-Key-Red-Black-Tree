#ifndef _rbt_SORT
#define _rbt_SORT
#include	"RBT_main.h"

	/*	 GLOBAL VARIABLES	*/
template <typename K, typename D>
inline static bool(*fooPtr)(rbtNode<K,D> *, rbtNode<K,D> *);

/*	============================================================================  */
/* |                                                                            | */
/* |                                QUICK SORT                                  | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: Swaps the values at two addresses (used by partitionPair() and partition())
 * parameters: 			The addresses of two values of the same dataType
 * return value:		N/S
*/
template <typename T>
inline void swap(T *key1, T *key2) {
	T tmp = *key1;
	*key1 = *key2;
	*key2 = tmp;
}

/*
 * function_identifier: The partition step of quickSortPair(). Ensures to keep the values in both arrays in
 *						the same relative order
 * parameters: 			Two arrays of same length, as well as the start and end indexes that should be considered in the lists
 * return value:		The partition index
*/
template <typename T, typename U>
inline size_t partitionPair(T *type1, U *type2, size_t st, size_t ed) {
		// Sets up the pivot
	T pivot = *(type1 + ed), *temp;
	size_t i = st;

		// Loops through the array and sorts
	for(size_t j = st; j < ed; j++) {
		temp = type1 + j;
		if(*temp <= pivot) {
			swap((type1 + i), temp);
			swap((type2 + i), (type2 + j));
			i++;
		}
	}

		// Swaps back the pivot
	swap((type1 + i), (type1 + ed));
	swap((type2 + i), (type2 + ed));
	return i;
}

/*
 * function_identifier: Sorts two array, while preserving their relative order, in ascending order, via the quick sort algorithm
 * parameters: 			Two arrays of same length, as well as the start and end indexes that should be considered in the lists
 * return value:		N/A
*/
template <typename T, typename U>
inline void quickSortPair(T *type1, U *type2, size_t st, size_t ed) {
	if(st < ed) {
		size_t p = partitionPair(type1, type2, st, ed);
		quickSortPair(type1, type2, st, p - 1);
		quickSortPair(type1, type2, p + 1, ed);
	}
}

/*
 * function_identifier: The partition step of quickSort().
 * parameters: 			The start and end pointers of a singular array
 * return value:		The partition pointer
*/
template <typename T>
inline T *partition(T *st, T *ed) {
		// Sets up the pivot
	T pivot = *ed, *mi = st;

		// Loops through the array and sorts
	while(st != ed) {
		if(*st <= pivot)
			swap(mi++, st);
		st++;
	}

		// Swaps back the pivot
	swap(mi, ed);
	return mi;
}

/*
 * function_identifier: Sorts an array in ascending order, via the quick sort algorithm
 * parameters: 			The start and end pointers of a singular array
 * return value:		N/A
*/
template <typename T>
inline void quickSort(T *st, T *ed) {
	if(st < ed) {
		T *p = partition(st, ed);
		quickSort(st, p - 1);
		quickSort(p + 1, ed);
	}
}

/*	============================================================================  */
/* |                                                                            | */
/* |                              NODE COMPAIR                                  | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: Compairs the keys of two given node pointers
 * parameters: 			Two node pointers
 * return value:		A boolean (True only if the first node has key less than second node)
*/
template <typename K, typename D>
bool rbTree<K,D>::keyCompair(rbtNode<K,D> *IdxS, rbtNode<K,D> *IdxM) {
	return (IdxS->key < IdxM->key);
}

/*
 * function_identifier: Compairs the keys of two given node pointers (reversed version of keyCompair())
 * parameters: 			Two node pointers
 * return value:		A boolean (True only if the first node has key less than second node)
*/
template <typename K, typename D>
bool rbTree<K,D>::keyCompairR(rbtNode<K,D> *IdxS, rbtNode<K,D> *IdxM) {
	return (IdxS->key > IdxM->key);
}

/*
 * function_identifier: Compairs the data of two given node pointers
 * parameters: 			Two node pointers
 * return value:		A boolean (True only if the first node has data less than second node)
*/
template <typename K, typename D>
bool rbTree<K,D>::dataCompair(rbtNode<K,D> *IdxS, rbtNode<K,D> *IdxM) {
	return (IdxS->data < IdxM->data);
}

/*
 * function_identifier: Compairs the data of two given node pointers (reversed version of dataCompair())
 * parameters: 			Two node pointers
 * return value:		A boolean (True only if the first node has data less than second node)
*/
template <typename K, typename D>
bool rbTree<K,D>::dataCompairR(rbtNode<K,D> *IdxS, rbtNode<K,D> *IdxM) {
	return (IdxS->data > IdxM->data);
}

/*
 * function_identifier: Compairs the key and data of two given node pointers
 * parameters: 			Two node pointers
 * return value:		A boolean (True only if the first node has key less than second node or if nodes have equal keys and
 *						first node has data less than second node)
*/
template <typename K, typename D>
bool rbTree<K,D>::keyDataCompair(rbtNode<K,D> *IdxS, rbtNode<K,D> *IdxM) {
	return ((IdxS->key < IdxM->key) || ((IdxS->key == IdxM->key) && (IdxS->data < IdxM->data)));
}

/*
 * function_identifier: Compairs the key and data of two given node pointers (reversed version of keyDataCompair())
 * parameters: 			Two node pointers
 * return value:		A boolean (True only if the first node has key less than second node or if nodes have equal keys and
 *						first node has data less than second node)
*/
template <typename K, typename D>
bool rbTree<K,D>::keyDataCompairR(rbtNode<K,D> *IdxS, rbtNode<K,D> *IdxM) {
	return ((IdxS->key > IdxM->key) || ((IdxS->key == IdxM->key) && (IdxS->data > IdxM->data)));
}

/*
 * function_identifier: Compairs the key and data of two given node pointers
 * parameters: 			Two node pointers
 * return value:		A boolean (True only if the first node has data less than second node or if nodes have equal data and
 *						first node has key less than second node)
*/
template <typename K, typename D>
bool rbTree<K,D>::dataKeyCompair(rbtNode<K,D> *IdxS, rbtNode<K,D> *IdxM) {
	return ((IdxS->data < IdxM->data) || ((IdxS->data == IdxM->data) && (IdxS->key < IdxM->key)));
}

/*
 * function_identifier: Compairs the key and data of two given node pointers (reversed version of dataKeyCompair())
 * parameters: 			Two node pointers
 * return value:		A boolean (True only if the first node has data less than second node or if nodes have equal data and
 *						first node has key less than second node)
*/
template <typename K, typename D>
bool rbTree<K,D>::dataKeyCompairR(rbtNode<K,D> *IdxS, rbtNode<K,D> *IdxM) {
	return ((IdxS->data > IdxM->data) || ((IdxS->data == IdxM->data) && (IdxS->key > IdxM->key)));
}



/*	============================================================================  */
/* |                                                                            | */
/* |                                NODE SORT                                   | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: Merges two sections in a list, using the provided function (called be "mergeSortCallerBase()")
 * parameters: 			An array of node pointers, the start pos of the section, the middle pos of the section, the end pos of the section, and
 *						the comparison function
 * return value:		N/A
*/
template <typename K, typename D>
void mergeSortBase(rbtNode<K,D> **nodes, size_t st, size_t mi, size_t ed) {
		// Sets up the needed arrays and pointers
	rbtNode<K,D> **memo = new rbtNode<K,D> *[ed-st];
	rbtNode<K,D> **IdxS = nodes + st, **IdxE1 = nodes + mi + 1;
	rbtNode<K,D> **IdxM = IdxE1, **IdxE2 = nodes + ed;

		// Sorts through the lists
	while(IdxS < IdxE1 && IdxM < IdxE2)
		if (fooPtr<K,D>(*IdxS, *IdxM))
			(*memo++) = (*IdxS++);
		else
			(*memo++) = (*IdxM++);

		// Include any remaining nodes not yet accounted for
	while(IdxS < IdxE1)
		(*memo++) = (*IdxS++);
	while(IdxM < IdxE2)
		(*memo++) = (*IdxM++);

		// Reinserts the sorted nodes
	IdxS = nodes + st;
	memo -= (ed - st);
	while(IdxS != IdxE2)
		(*IdxS++) = (*memo++);

		// Deletes the previously allocated memory
	delete [] (memo + st - ed);
}

/*
 * function_identifier: Sorts an array of tree node pointers, using the provided function (called be "rbt_nodeSort()")
 * parameters: 			An array of node pointers, the pos to start the sort, the pos to end the sort, and
 *						the comparison function
 * return value:		N/A
*/
template <typename K, typename D>
void mergeSortCallerBase(rbtNode<K,D> **nodes, size_t si, size_t ed) {
		// Checks if there is still more to sort
		// If so, recursively call this function again, with subdivided arrays
	if(si < ed) {
			// Calculates the mid point
		size_t mi = si + ((ed - si) >> 1);
			// Recursively call this function again for subdivisions of the given array
		fooPtr<K,D> = rbTree<K,D>::keyDataCompair;
		mergeSortCallerBase(nodes, si, mi);
		mergeSortCallerBase(nodes, mi + 1, ed);
			// Merges the two sorted subdivided of the given arrays
		mergeSortBase(nodes, si, mi, ed + 1);
	} 
}

/*
 * function_identifier: User's interface to the sort functions
 * parameters: 			A node array, the start index of the array, the end index of the array, and they sort type
 * return value:		N/A
*/
template <typename K, typename D>
void rbt_nodeSort(rbtNode<K,D> **nodes, size_t si, size_t ed, enum rbtsort sort = KEYDATA) {
		// Returns if there is nothing to do
	if(!nodes || ed <= si + 1) return;

		// Sort the nodes in the way requested
	switch(sort) {
		case KEY: 			fooPtr<K,D> = rbTree<K,D>::keyCompair; break;
		case KEY_R:			fooPtr<K,D> = rbTree<K,D>::keyCompairR; break;
		case DATA:			fooPtr<K,D> = rbTree<K,D>::dataCompair; break;
		case DATA_R:		fooPtr<K,D> = rbTree<K,D>::dataCompairR; break;
		case KEYDATA:		fooPtr<K,D> = rbTree<K,D>::keyDataCompair; break;
		case KEYDATA_R:		fooPtr<K,D> = rbTree<K,D>::keyDataCompairR; break;
		case DATAKEY:		fooPtr<K,D> = rbTree<K,D>::dataKeyCompair; break;
		case DATAKEY_R:		fooPtr<K,D> = rbTree<K,D>::dataKeyCompairR; break;
	}
	mergeSortCallerBase(nodes, si, ed - 1);
}

/*	============================================================================  */
/* |                                                                            | */
/* |                                 TREEIFY                                    | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: Converts an array of sorted nodes (primarily sorted by key, then by data) into a red-black tree.
 *						Deep copies the given nodes
 * parameters: 			The start and end indexes to consider, an array of nodes, and the current parent to branch from
 * return value:		Number of descendants the given parent has
*/
template <typename K, typename D>
size_t rbTree<K,D>::treeify(size_t st, size_t ed, rbtNode<K,D> **rbtNodes, rbtNode<K,D> **curr) {
		// Calculates the average of midpoint, without overflow
	size_t mid = st + ((ed - st + 1) >> 1);

		// Sets up the new node
	*curr = new rbtNode<K,D>(rbtNodes[mid]);
	(*curr)->color = false;

		// Check if a left child needs to be created
	if (st < mid) {
			// Creates a left child, adds it's descendants to the current node, and corrects the color
		(*curr)->descendants += treeify(st, mid-1, rbtNodes, &(*curr)->left);
		(*curr)->left->parent = *curr;
		(*curr)->color ^= true;
	}

		// Check if a right child needs to be created
	if (mid < ed) {
			// Creates a right child, adds it's descendants to the current node, and corrects the color
		(*curr)->descendants += treeify(mid+1, ed, rbtNodes, &(*curr)->right);
		(*curr)->right->parent = *curr;
		(*curr)->color ^= true;
	}

		// Returns the current node's number of descendants
	return (*curr)->descendants + 1;
}

/*
 * function_identifier: Converts an array of sorted nodes (primarily sorted by key, then by data) into a red-black tree.
 *						Shallow copies the given nodes
 * parameters: 			The start and end indexes to consider, an array of nodes, and the current parent to branch from
 * return value:		Number of descendants the given parent has
*/
template <typename K, typename D>
size_t rbTree<K,D>::treeifyShallow(size_t st, size_t ed, rbtNode<K,D> **rbtNodes, rbtNode<K,D> **curr) {
		// Calculates the average of midpoint, without overflow
	size_t mid = st + ((ed - st + 1) >> 1);

		// Sets up the current node
	*curr = rbtNodes[mid];
	(*curr)->color = false;
	(*curr)->descendants = 0;

		// Check if a left child needs to be created
	if (st < mid) {
			// Creates a left child, adds it's descendants to the current node, and corrects the color
		(*curr)->descendants += treeifyShallow(st, mid-1, rbtNodes, &(*curr)->left);
		(*curr)->left->parent = *curr;
		(*curr)->color ^= true;
	} else
			// If not, set it to be NULL
		(*curr)->left = NULL;

		// Check if a right child needs to be created
	if (mid < ed) {
			// Creates a right child, adds it's descendants to the current node, and corrects the color
		(*curr)->descendants += treeifyShallow(mid+1, ed, rbtNodes, &(*curr)->right);
		(*curr)->right->parent = *curr;
		(*curr)->color ^= true;
	} else
			// If not, set it to be NULL
		(*curr)->right = NULL;

		// Returns the current node's number of descendants
	return (*curr)->descendants + 1;
}


/*
 * function_identifier: Allows the user to convert an array of nodes into a red-black tree 
 * parameters: 			The length of the array, and the array of nodes
 * return value:		N/A
*/
template <typename K, typename D> 
void rbTree<K,D>::rbt_treeify(size_t len, rbtNode<K,D> **rbtNodes) {
		// Resets the tree
	rbt_free();
	size = len;

	if (len) {
			// If we are inserting nodes, sort and treeify them
		fooPtr<K,D> = keyDataCompair;
		mergeSortCallerBase(rbtNodes, 0, len - 1);
		treeify(0, len-1, rbtNodes, &root);
	} else
			// If no nodes to insert, set the root to be NULL
		root = NULL;
}

/*
 * function_identifier: The treeify constructor for the red-black tree
 * parameters: 			The length of an array, and the node array
 * return value:		N/A
*/
template <typename K, typename D> 
rbTree<K,D>::rbTree(size_t len, rbtNode<K,D> **&rbtNodes) {
		// Sets the length of the tree
	size = len;

	if (len) {
			// If we are inserting nodes, sort and treeify them
		fooPtr<K,D> = keyDataCompair;
		mergeSortCallerBase(rbtNodes, 0, len - 1);
		treeify(0, len-1, rbtNodes, &root);
	} else
			// If no nodes to insert, set the root to be NULL
		root = NULL;
}

#endif /* _rbt_SORT */