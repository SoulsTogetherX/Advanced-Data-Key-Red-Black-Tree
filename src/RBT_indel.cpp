#ifndef _rbt_INSERTDELETE
#define _rbt_INSERTDELETE
#include	"RBT_main.h"
#include	"RBT_indel.h"
#include	"RBT_traversal.h"
#include	"RBT_sort.h"

	/*	 GLOBAL VARIABLES	*/
		// External From 'RBT_sort.cpp'
template <typename K, typename D>
extern inline bool(*fooPtr)(rbtNode<K,D> *, rbtNode<K,D> *);

/*	============================================================================  */
/* |                                                                            | */
/* |                                MULTI INSERT                                | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: Inserts multiple nodes into the red-black tree, according to the provided arrays
 * parameters: 			The length of the arrays, an array of keys, and an array of data values
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_repInsert(size_t arrayLength, const K *keys, const D *data) {
		// Ignores the trival case
	if (!arrayLength)
		return;

		// Checks for node overflow
	if (size > (size + arrayLength))
		throw std::overflow_error("overflow - red black tree(s) to large to perform opperation");

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxE;

		// Gets the needed nodes and indexes
	rbtNodes = new rbtNode<K,D> *[size + arrayLength];
	rbt_getAllNodes(rbtNodes);
	IdxS = rbtNodes + size;
	IdxE = IdxS + arrayLength;

		// Checks if given a data array to work with
	if (data == NULL) {
			// Checks if given a key array to work with
		if (keys == NULL) {
				// If not, then just use the default constructors for the key and data types
			while(IdxS != IdxE)
				*IdxS++ = new rbtNode<K,D>(K(), D());
		} else {
				// If so, then use the default constructor for the data type and the key array for the key values
			while(IdxS != IdxE)
				*IdxS++ = new rbtNode<K,D>(*keys++, D());
		}
	} else {
			// Checks if given a key array to work with
		if (keys == NULL) {
				// If not, then use the default constructor for the key type and the data array for the data values
			while(IdxS != IdxE)
				*IdxS++ = new rbtNode<K,D>(K(), *data++);
		} else {
				// If so, use the provided arrays for the key and data values
			while(IdxS != IdxE)
				*IdxS++ = new rbtNode<K,D>(*keys++, *data++);
		}
	}

		// Sorts the inserted nodes
	fooPtr<K,D> = keyDataCompair;
	mergeSortCallerBase(rbtNodes, size, size + arrayLength - 1);
		// Merges the nodes with the other nodes in the red-black tree
			// The merged result will be sorted in ascending order of keys and data (primarily sorted by keys)
	mergeSortBase(rbtNodes, 0, size - 1, size + arrayLength - 1);

		// Uses the nodes to reconstruct the tree
	size += arrayLength;
	treeifyShallow(0, size - 1, rbtNodes, &root);
	root->parent = NULL;

		// Deletes the remaining array
	delete [] rbtNodes;
}

/*
 * function_identifier: Inserts multiple nodes into the red-black tree, according to the provided array
 * parameters: 			The length of the array and an array of nodes
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_repInsert(size_t arrayLength, rbtNode<K,D> **inS) {
		// Ignores the trival case
	if (!arrayLength)
		return;

		// Checks for node overflow
	if (size > (size + arrayLength))
		throw std::overflow_error("overflow - red black tree(s) to large to perform opperation");

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxE;

		// Gets the needed nodes and indexes
	rbtNodes = new rbtNode<K,D> *[size + arrayLength];
	rbt_getAllNodes(rbtNodes);
	IdxS = rbtNodes + size;
	IdxE = IdxS + arrayLength;

		// Deep copies the given nodes
	while(IdxS != IdxE)
		*IdxS++ = new rbtNode<K,D>(*inS++);

		// Merges the nodes with the other nodes in the red-black tree
			// The merged result will be sorted in ascending order of keys and data (primarily sorted by keys)
	fooPtr<K,D> = keyDataCompair;
	mergeSortBase(rbtNodes, 0, size, size + arrayLength);

		// Uses the nodes to reconstruct the tree
	size += arrayLength;
	treeifyShallow(0, size - 1, rbtNodes, &root);
	root->parent = NULL;

		// Deletes the remaining array
	delete [] rbtNodes;
}

/*
 * function_identifier: Inserts multiple nodes into the red-black tree, with an increasing key amount
 * parameters: 			The length of the array, the starting key, the step key value, and an array of data
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_repInsertInc(size_t num, K st, K step, const D *data) {
		// Ignores the trival case
	if (!num)
		return;

		// Checks for node overflow
	if (size > (size + num))
		throw std::overflow_error("overflow - red black tree(s) to large to perform opperation");

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxE;

		// Gets the needed nodes and indexes
	rbtNodes = new rbtNode<K,D> *[size + num];
	rbt_getAllNodes(rbtNodes);
	IdxS = rbtNodes + size;
	IdxE = IdxS + num;

		// Checks if given a data array to work with
	if (data == NULL) {
			// If not, then just use the default constructor for the data type
		while(IdxS != IdxE) {
			*IdxS++ = new rbtNode<K,D>(st, D());
			st += step; // Increases the key step
		}
	} else {
			// If so, use the provided array data values
		while(IdxS != IdxE) {
			*IdxS++ = new rbtNode<K,D>(st, *data++);
			st += step; // Increases the key step
		}
	}

		// Merges the nodes with the other nodes in the red-black tree
			// The merged result will be sorted in ascending order of keys and data (primarily sorted by keys)
	fooPtr<K,D> = keyDataCompair;
	mergeSortBase(rbtNodes, 0, size - 1, size + num);

		// Uses the nodes to reconstruct the tree
	size += num;
	treeifyShallow(0, size - 1, rbtNodes, &root);
	root->parent = NULL;

		// Deletes the remaining array
	delete [] rbtNodes;
}

/*
 * function_identifier: Inserts multiple nodes into the red-black tree, with an decreasing key amount
 * parameters: 			The length of the array, the starting key, the step key value, and an array of data
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_repInsertDec(size_t num, K st, K step, const D *data) {
		// Ignores the trival case
	if (!num)
		return;

		// Checks for node overflow
	if (size > (size + num))
		throw std::overflow_error("overflow - red black tree(s) to large to perform opperation");

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxE;

		// Gets the needed nodes and indexes
	rbtNodes = new rbtNode<K,D> *[size + num];
	rbt_getAllNodes(rbtNodes);
	IdxS = rbtNodes + size;
	IdxE = IdxS + num;

		// Checks if given a data array to work with
	if (data == NULL) {
			// If not, then just use the default constructor for the data type
		while(IdxS < IdxE) {
			*--IdxE = new rbtNode<K,D>(st, D());
			st -= step; // Decreases the key step
		}
	} else {
			// If so, use the provided array data values
		while(IdxS < IdxE) {
			*--IdxE = new rbtNode<K,D>(st, *data++);
			st -= step; // Decreases the key step
		}
	}

		// Merges the nodes with the other nodes in the red-black tree
			// The merged result will be sorted in ascending order of keys and data (primarily sorted by keys)
	fooPtr<K,D> = keyDataCompair;
	mergeSortBase(rbtNodes, 0, size - 1, size + num);

		// Uses the nodes to reconstruct the tree
	size += num;
	treeifyShallow(0, size - 1, rbtNodes, &root);
	root->parent = NULL;

		// Deletes the remaining array
	delete [] rbtNodes;
}

/*	============================================================================  */
/* |                                                                            | */
/* |                                MULTI DELETE                                | */
/* |                                                                            | */
/*	============================================================================  */

/*		   KEY_DATA			*/

/*
 * function_identifier: Deletes multiple nodes from the red-black tree
 * parameters: 			The length of the array and an array of nodes
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_repDelete(size_t arrayLength, rbtNode<K,D> **delS) {
		// Ignores the trival case
	if (!arrayLength || !size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE, **delE;
	struct pair {K key; D data;};
	struct pair *infoS, *infoE;

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;
	delE = delS + arrayLength;

		// Allocates memory to store key and data pairs
	infoS = new pair[arrayLength];
	infoE = infoS + arrayLength;

		// Sorts accessed nodes
	fooPtr<K,D> = keyDataCompair;
	mergeSortCallerBase(delS, 0, arrayLength - 1);

		// Saves the key and data values of provided nodes
		// This ensures we do not access invaild data during deletion
	while(delS != delE) {
		(*infoS).key = (*delS)->key;
		(*infoS++).data = (*delS++)->data;
	}
		// Resets 'infoS' back to start of allocated array
	infoS -= arrayLength;

		// Cycles through the entire array of accessed nodes
	while(IdxS != IdxE && infoS != infoE)
			// Checks if both considered nodes have equal keys and data
			// If so, delete it and move forward
			// If not, skip/save it, then move forward in one of the arrays
		if ((*IdxS)->key == (*infoS).key) {
			if ((*IdxS)->data == (*infoS).data) {
				delete *IdxS++; infoS++;
			} else if ((*IdxS)->data < (*infoS).data)
				*IdxM++ = *IdxS++;
			else
				infoS++;
		} else if ((*IdxS)->key < (*infoS).key)
			*IdxM++ = *IdxS++;
		else
			infoS++;

		// Clears pair array
	delete [] (infoE - arrayLength);

		// Places remaining, non-deleted, nodes into the array
	if (IdxS != IdxM)
		while(IdxS != IdxE)
			*IdxM++ = *IdxS++;

		// Calculates the size of the array of non-deleted nodes
	size = IdxM - rbtNodes;

		// Checks if there is any nodes left to process
	if (size) {
			// If there are remaining nodes, create a tree from them
		treeifyShallow(0, size - 1, rbtNodes, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Clears accessed node array
	delete [] rbtNodes;
}

/*
 * function_identifier: Deletes multiple nodes, from the red-black tree, which have the same keys and data provided
 * parameters: 			The length of the arrays, an array of keys, and an array of data
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_repDelete(size_t arrayLength, K *keysS, D *dataS) {
		// Ignores the trival case
	if (!arrayLength || !size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE;
	K *keysE;
	struct pair {K key; D data; bool operator<=(const pair &info) {return (key < info.key || (key == info.key && data <= info.data));}};
	struct pair *infoS, *infoE;

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;
	keysE = keysS + arrayLength;

		// Allocates memory to hold key and data pairs
	infoS = new pair[arrayLength];
	infoE = infoS + arrayLength;

		// Saves the key and data values from the provided arrays
		// This ensures we are able to sort them while preserving their relative order
	while(keysS != keysE) {
		(*infoS).key = *keysS++;
		(*infoS++).data = *dataS++;
	}
		// Resets 'infoS' back to start of allocated array
	infoS -= arrayLength;

		// Sorts the pair array (primarily sorted by keys)
	quickSort(infoS, infoE - 1);

		// Cycles through the entire array of accessed nodes
	while(IdxS != IdxE && infoS != infoE)
			// Checks if the node considered has the requested keys and data pairs
			// If so, delete it and move forward
			// If not, skip/save it, then move forward in one of the arrays
		if ((*IdxS)->key == (*infoS).key) {
			if ((*IdxS)->data == (*infoS).data) {
				delete *IdxS++; infoS++;
			} else if ((*IdxS)->data < (*infoS).data)
				*IdxM++ = *IdxS++;
			else
				infoS++;
		} else if ((*IdxS)->key < (*infoS).key)
			*IdxM++ = *IdxS++;
		else
			infoS++;

		// Clears pair array
	delete [] (infoE - arrayLength);

		// Places remaining, non-deleted, nodes into the array
	if (IdxS != IdxM)
		while(IdxS != IdxE)
			*IdxM++ = *IdxS++;

		// Calculates the size of the array of non-deleted nodes
	size = IdxM - rbtNodes;

		// Checks if there is any nodes left to process
	if (size) {
			// If there are remaining nodes, create a tree from them
		treeifyShallow(0, size - 1, rbtNodes, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Clears accessed node array
	delete [] rbtNodes;
}

/*		 KEY 		*/

/*
 * function_identifier: Deletes multiple nodes, from the red-black tree, which have the same keys provided
 * parameters: 			The length of the arrays and an array of keys
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_repDeleteKey(size_t arrayLength, K *keysS) {
		// Ignores the trival case
	if (!arrayLength || !size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE;
	K *keysE;

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;
	keysE = keysS + arrayLength;

		// Sorts the keys
	quickSort(keysS, keysE - 1);

		// Cycles through the entire array of accessed nodes and keys
	while(IdxS != IdxE && keysS != keysE) {
			// Checks if the node considered has the requested keys
			// If so, delete it and move forward
			// If not, skip/save it, then move forward in one of the arrays
		if ((*IdxS)->key == *keysS) {
			delete *IdxS++; keysS++;
		} else if ((*IdxS)->key < *keysS)
			*IdxM++ = *IdxS++;
		else
			keysS++;
	}

		// Places remaining, non-deleted, nodes into the array
	if (IdxS != IdxM)
		while(IdxS != IdxE)
			*IdxM++ = *IdxS++;

		// Calculates the size of the array of non-deleted nodes
	size = IdxM - rbtNodes;

		// Checks if there is any nodes left to process
	if (size) {
			// If there are remaining nodes, create a tree from them
		treeifyShallow(0, size - 1, rbtNodes, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Clears accessed node array
	delete [] rbtNodes;
}

/*
 * function_identifier: Deletes multiple nodes, from the red-black tree, which have the same keys provided
 * parameters: 			The length of the arrays and an array of nodes with keys
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_repDeleteKey(size_t arrayLength, rbtNode<K,D> **delS) {
		// Ignores the trival case
	if (!arrayLength || !size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE, **delE;
	D *dataE;

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;
	delE = delS + arrayLength;

		// Sorts the given array
	fooPtr<K,D> = keyDataCompair;
	mergeSortCallerBase(delS, 0, arrayLength - 1);

		// Cycles through the entire array of accessed nodes and keys
	while(IdxS != IdxE && delS != delE) {
			// Checks if both considered nodes have equal keys
			// If so, delete it and move forward
			// If not, skip/save it, then move forward in one of the arrays
		if ((*IdxS)->key == (*delS)->key) {
			delete *IdxS++; delS++;
		} else if ((*IdxS)->key < (*delS)->key)
			*IdxM++ = *IdxS++;
		else
			delS++;
	}

		// Places remaining, non-deleted, nodes into the array
	if (IdxS != IdxM)
		while(IdxS != IdxE)
			*IdxM++ = *IdxS++;

		// Calculates the size of the array of non-deleted nodes
	size = IdxM - rbtNodes;

		// Checks if there is any nodes left to process
	if (size) {
			// If there are remaining nodes, create a tree from them
		treeifyShallow(0, size - 1, rbtNodes, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Clears accessed node array
	delete [] rbtNodes;
}

/*		   DATA			*/

/*
 * function_identifier: Deletes multiple nodes, from the red-black tree, which have the same data provided
 * parameters: 			The length of the arrays and an array of data
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_repDeleteData(size_t arrayLength, D *dataS) {
		// Ignores the trival case
	if (!arrayLength || !size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE;
	D *dataE;

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;
	dataE = dataS + arrayLength;

		// Sorts the data and nodes
	fooPtr<K,D> = dataCompair;
	mergeSortCallerBase(rbtNodes, 0, size - 1);
	quickSort(dataS, dataE - 1);

		// Cycles through the entire array of accessed nodes and data
	while(IdxS != IdxE && dataS != dataE) {
			// Checks if the node considered has the requested data
			// If so, delete it and move forward
			// If not, skip/save it, then move forward in one of the arrays
		if ((*IdxS)->data == *dataS) {
			delete *IdxS++; dataS++;
		} else if ((*IdxS)->data < *dataS)
			*IdxM++ = *IdxS++;
		else
			dataS++;
	}

		// Places remaining, non-deleted, nodes into the array
	if (IdxS != IdxM)
		while(IdxS != IdxE)
			*IdxM++ = *IdxS++;

		// Calculates the size of the array of non-deleted nodes
	size = IdxM - rbtNodes;

		// Checks if there is any nodes left to process
	if (size) {
			// If there are remaining nodes, sort them and create a tree from the result
		fooPtr<K,D> = keyDataCompair;
		mergeSortCallerBase(rbtNodes, 0, size - 1);
		treeifyShallow(0, size - 1, rbtNodes, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Clears accessed node array
	delete [] rbtNodes;
}

/*
 * function_identifier: Deletes multiple nodes, from the red-black tree, which have the same data provided
 * parameters: 			The length of the arrays and an array of nodes with data
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_repDeleteData(size_t arrayLength, rbtNode<K,D> **delS) {
		// Ignores the trival case
	if (!arrayLength || !size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE, **delE;

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;
	delE = delS + arrayLength;

		// Sorts the nodes
	fooPtr<K,D> = dataCompair;
	mergeSortCallerBase(rbtNodes, 0, size - 1);
	mergeSortCallerBase(delS, 0, arrayLength);

		// Cycles through the entire array of accessed nodes and data
	while(IdxS != IdxE && delS != delE) {
			// Checks if both considered nodes have equal data
			// If so, delete it and move forward
			// If not, skip/save it, then move forward in one of the arrays
		if ((*IdxS)->data == (*delS)->data) {
			delete *IdxS++; delS++;
		} else if ((*IdxS)->data < (*delS)->data)
			*IdxM++ = *IdxS++;
		else
			delS++;
	}

		// Places remaining, non-deleted, nodes into the array
	if (IdxS != IdxM)
		while(IdxS != IdxE)
			*IdxM++ = *IdxS++;

		// Calculates the size of the array of non-deleted nodes
	size = IdxM - rbtNodes;

		// Checks if there is any nodes left to process
	if (size) {
			// If there are remaining nodes, sort them and create a tree from the result
		fooPtr<K,D> = keyDataCompair;
		mergeSortCallerBase(rbtNodes, 0, size - 1);
		treeifyShallow(0, size - 1, rbtNodes, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Clears accessed node array
	delete [] rbtNodes;
}

/*	============================================================================  */
/* |                                                                            | */
/* |                                  REMOVE                                    | */
/* |                                                                            | */
/*	============================================================================  */

/*		   KEY_DATA			*/

/*
 * function_identifier: Deletes all nodes, in the red-black tree, which have the data and key as he node provided
 * parameters: 			A node
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_remove(rbtNode<K,D> *delNode) {
	rbt_remove(delNode->key, delNode->data);
}

/*
 * function_identifier: Deletes all nodes, in the red-black tree, which have the data and keys as the nodes provided
 * parameters: 			The array length and an array of nodes
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_remove(size_t arrayLength, rbtNode<K,D> **delS) {
		// Ignores the trival case
	if (!size)
		return;

		// Sets up dataType 'pair'. This is needed later.
	struct pair {K key; D data;};

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE, **delE;
	struct pair *infoS, *infoE;

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;
	delE = delS + arrayLength;
	infoS = new pair[arrayLength];
	infoE = infoS + arrayLength;

		// Sorts the provided nodes
	fooPtr<K,D> = keyDataCompair;
	mergeSortCallerBase(delS, 0, arrayLength - 1);

		// Saves the key and data values from the provided arrays
		// This ensures we are able to sort them while preserving their relative order
	while(delS != delE) {
		(*infoS).key = (*delS)->key;
		(*infoS++).data = (*delS++)->data;
	}

		// Resets 'infoS' back to start of allocated array
	infoS -= arrayLength;

		// Cycles through the entire array of accessed nodes and data
	while(IdxS != IdxE && infoS != infoE)
			// Checks if both considered nodes have equal keys and data
			// If so, delete it and all similar nodes
			// If not, skip/save it, then move forward in one of the arrays
		if ((*IdxS)->key == (*infoS).key) {
			if ((*IdxS)->data == (*infoS).data)
				delete *IdxS++;
			else if ((*IdxS)->data < (*infoS).data)
				*IdxM++ = *IdxS++;
			else
				infoS++;
		} else if ((*IdxS)->key < (*infoS).key)
			*IdxM++ = *IdxS++;
		else
			infoS++;

	delete [] (infoE - arrayLength);

		// Checks if any nodes were deleted
	if (IdxM != IdxS)
			// If some nodes were deleted, place the remaining nodes into the array
		while(IdxS != IdxE)
			*IdxM++ = *IdxS++;
	else
			// If no nodes deleted, just skip to the end
			// Everything is already fine how it is
		IdxM = IdxE;

		// Calculates the size of the array of non-deleted nodes
	size = IdxM - rbtNodes;

		// Checks if there is any nodes left to process
	if (size) {
			// If there are remaining nodes, create a tree from them
		treeifyShallow(0, size - 1, rbtNodes, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Clears accessed node array
	delete [] rbtNodes;
}

/*
 * function_identifier: Deletes all nodes, in the red-black tree, which have the data and key provided
 * parameters: 			A key and data value
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_remove(K key, D data) {
		// Ignores the trival case
	if (!size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE;

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;

		// Cycles through the entire array of accessed nodes and data
	while(IdxS != IdxE)
			// Checks if the considered node has the requested key and data pair
			// If so, delete it and all similar nodes
			// If not, skip/save it, then move forward in one of the arrays
		if (((*IdxS)->key == key) && ((*IdxS)->data == data))
			delete *IdxS++;
		else
			*IdxM++ = *IdxS++;

		// Calculates the size of the array of non-deleted nodes
	size = IdxM - rbtNodes;

		// Checks if there is any nodes left to process
	if (size) {
			// If there are remaining nodes, create a tree from them
		treeifyShallow(0, size - 1, rbtNodes, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Clears accessed node array
	delete [] rbtNodes;
}

/*
 * function_identifier: Deletes all nodes, in the red-black tree, which have the datas and keys provided
 * parameters: 			The array length, an array of keys, and an array of data
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_remove(size_t arrayLength, K *keysS, D *dataS) {
		// Ignores the trival case
	if (!size)
		return;

		// Sets up dataType 'pair'. This is needed later.
	struct pair {K key; D data; bool operator<=(const pair &info) {return (key < info.key || (key == info.key && data <= info.data));}};

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE;
	K *keysE;
	struct pair *infoS, *infoE;

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;
	keysE = keysS + arrayLength;
	infoS = new pair[arrayLength];
	infoE = infoS + arrayLength;

		// Saves the key and data values from the provided arrays
		// This ensures we don't access deleted memory by accident
	while(keysS != keysE) {
		(*infoS).key = *keysS++;
		(*infoS++).data = *dataS++;
	}

		// Resets 'infoS' back to start of allocated array
	infoS -= arrayLength;

		// Sorts the provided keys and data
	quickSort(infoS, infoE - 1);

		// Cycles through the entire array of accessed nodes and data
	while(IdxS != IdxE && infoS != infoE)
			// Checks if the node considered has a requested key and data pair
			// If so, delete it and all similar nodes
			// If not, skip/save it, then move forward in one of the arrays
		if ((*IdxS)->key == (*infoS).key) {
			if ((*IdxS)->data == (*infoS).data)
				delete *IdxS++;
			else if ((*IdxS)->data < (*infoS).data)
				*IdxM++ = *IdxS++;
			else if ((*IdxS)->data > (*infoS).data)
				infoS++;
		} else if ((*IdxS)->key < (*infoS).key)
			*IdxM++ = *IdxS++;
		else
			infoS++;

		// Clears pair array
	delete [] (infoE - arrayLength);

		// Checks if any nodes were deleted
	if (IdxM != IdxS)
			// If some nodes were deleted, place the remaining nodes into the array
		while(IdxS != IdxE)
			*IdxM++ = *IdxS++;
	else
			// If no nodes deleted, just skip to the end
			// Everything is already fine how it is
		IdxM = IdxE;

		// Calculates the size of the array of non-deleted nodes
	size = IdxM - rbtNodes;

		// Checks if there is any nodes left to process
	if (size) {
			// If there are remaining nodes, create a tree from them
		treeifyShallow(0, size - 1, rbtNodes, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Clears accessed node array
	delete [] rbtNodes;
}

/*		 KEY 		*/

/*
 * function_identifier: Deletes all nodes, in the red-black tree, which have the key provided
 * parameters: 			A key
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_removeKey(K key) {
		// Ignores the trival case
	if (!size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE;

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;

		// Cycles through the entire array of accessed nodes and data
	while(IdxS != IdxE)
			// Checks if the node considered had the requested key
			// If so, delete it and all similar nodes
			// If not, skip/save it, then move forward in one of the arrays
		if ((*IdxS)->key == key)
			delete *IdxS++;
		else
			*IdxM++ = *IdxS++;

		// Calculates the size of the array of non-deleted nodes
	size = IdxM - rbtNodes;

		// Checks if there is any nodes left to process
	if (size) {
			// If there are remaining nodes, create a tree from them
		treeifyShallow(0, size - 1, rbtNodes, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Clears accessed node array
	delete [] rbtNodes;
}

/*
 * function_identifier: Deletes all nodes, in the red-black tree, which have the key in the node provided
 * parameters: 			A node
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_removeKey(rbtNode<K,D> *remNode) {
	rbt_removeKey(remNode->key);
}


/*
 * function_identifier: Deletes all nodes, in the red-black tree, which have the keys provided
 * parameters: 			An array length and an array of keys
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_removeKey(size_t arrayLength, K *keysS) {
		// Ignores the trival case
	if (!size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE;
	K *keysE;

		// Gets the needed nodes and indexes
	IdxM = IdxS = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;
	keysE = keysS + arrayLength;

		// Sorts the provided keys
	quickSort(keysS, keysE - 1);

		// Cycles through the entire array of accessed nodes and data
	while(IdxS != IdxE && keysS != keysE)
			// Checks if the node considered had the requested keys
			// If so, delete it and all similar nodes
			// If not, skip/save it, then move forward in one of the arrays
		if ((*IdxS)->key == *keysS)
			delete *IdxS++;
		else if ((*IdxS)->key < *keysS)
			*IdxM++ = *IdxS++;
		else
			keysS++;

		// Checks if any nodes were deleted
	if (IdxM != IdxS)
			// If some nodes were deleted, place the remaining nodes into the array
		while(IdxS != IdxE)
			*IdxM++ = *IdxS++;
	else
			// If no nodes deleted, just skip to the end
			// Everything is already fine how it is
		IdxM = IdxE;

		// Calculates the size of the array of non-deleted nodes
	size = IdxM - rbtNodes;

		// Checks if there is any nodes left to process
	if (size) {
			// If there are remaining nodes, create a tree from them
		treeifyShallow(0, size - 1, rbtNodes, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Clears accessed node array
	delete [] rbtNodes;
}

/*
 * function_identifier: Deletes all nodes, in the red-black tree, which have the keys in the nodes provided
 * parameters: 			An array length and an array of nodes
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_removeKey(size_t arrayLength, rbtNode<K,D> **remNodeS) {
		// Ignores the trival case
	if (!size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE, **remNodeE;
	K *keyS, *keyE;

		// Gets the needed nodes and indexes
	IdxM = IdxS = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;
	remNodeE = remNodeS + arrayLength;
	keyS = new K[arrayLength];
	keyE = keyS + arrayLength;

		// Sorts the provided nodes
	fooPtr<K,D> = keyDataCompair;
	mergeSortCallerBase(remNodeS, 0, arrayLength - 1);

		// Saves the key values from the provided arrays
		// This ensures we don't access deleted memory by accident
	while(remNodeE != remNodeS)
		*keyS++ = (*remNodeS++)->key;

	keyS -= arrayLength;

		// Cycles through the entire array of accessed nodes and data
	while(IdxS != IdxE && keyS != keyE)
			// Checks if both considered nodes have equal keys
			// If so, delete it and all similar nodes
			// If not, skip/save it, then move forward in one of the arrays
		if ((*IdxS)->key == *keyS)
			delete *IdxS++;
		else if ((*IdxS)->key < *keyS)
			*IdxM++ = *IdxS++;
		else
			keyS++;

		// Clears pair array
	delete [] (keyE - arrayLength);

		// Checks if any nodes were deleted
	if (IdxM != IdxS)
			// If some nodes were deleted, place the remaining nodes into the array
		while(IdxS != IdxE)
			*IdxM++ = *IdxS++;
	else
			// If no nodes deleted, just skip to the end
			// Everything is already fine how it is
		IdxM = IdxE;

		// Calculates the size of the array of non-deleted nodes
	size = IdxM - rbtNodes;

		// Checks if there is any nodes left to process
	if (size) {
			// If there are remaining nodes, create a tree from them
		treeifyShallow(0, size - 1, rbtNodes, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Clears accessed node array
	delete [] rbtNodes;
}

/*		   DATA			*/

/*
 * function_identifier: Deletes all nodes, in the red-black tree, which have the data provided
 * parameters: 			A data value
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_removeData(D data) {
		// Ignores the trival case
	if (!size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE;

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;

		// Cycles through the entire array of accessed nodes and data
	while(IdxS != IdxE)
			// Checks if the node considered had the requested data
			// If so, delete it and all similar nodes
			// If not, skip/save it, then move forward in one of the arrays
		if ((*IdxS)->data == data)
			delete *IdxS++;
		else
			*IdxM++ = *IdxS++;

		// Calculates the size of the array of non-deleted nodes
	size = IdxM - rbtNodes;

		// Checks if there is any nodes left to process
	if (size) {
			// If there are remaining nodes, create a tree from them
		treeifyShallow(0, size - 1, rbtNodes, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Clears accessed node array
	delete [] rbtNodes;
}

/*
 * function_identifier: Deletes all nodes, in the red-black tree, which have the data in the node provided
 * parameters: 			A node
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_removeData(rbtNode<K,D> *remNode) {
	rbt_removeData(remNode->data);
}

/*
 * function_identifier: Deletes all nodes, in the red-black tree, which have the data values provided
 * parameters: 			The array length and an array of data values
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_removeData(size_t arrayLength, D *dataS) {
		// Ignores the trival case
	if (!size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE;
	D *dataE;

		// Gets the needed nodes and indexes
	IdxM = IdxS = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;
	dataE = dataS + arrayLength;

		// Sorts the accessed nodes and data values
	fooPtr<K,D> = dataCompair;
	mergeSortCallerBase(rbtNodes, 0, size - 1);
	quickSort(dataS, dataE - 1);

		// Cycles through the entire array of accessed nodes and data
	while(IdxS != IdxE && dataS != dataE)
			// Checks if the node considered had the requested data values
			// If so, delete it and all similar nodes
			// If not, skip/save it, then move forward in one of the arrays
		if ((*IdxS)->data == *dataS)
			delete *IdxS++;
		else if ((*IdxS)->data < *dataS)
			*IdxM++ = *IdxS++;
		else
			dataS++;

		// Checks if any nodes were deleted
	if (IdxM != IdxS)
			// If some nodes were deleted, place the remaining nodes into the array
		while(IdxS != IdxE)
			*IdxM++ = *IdxS++;
	else
			// If no nodes deleted, just skip to the end
			// Everything is already fine how it is
		IdxM = IdxE;

		// Calculates the size of the array of non-deleted nodes
	size = IdxM - rbtNodes;

		// Checks if there is any nodes left to process
	if (size) {
			// If there are remaining nodes, sort them and create a tree from the result
		fooPtr<K,D> = keyDataCompair;
		mergeSortCallerBase(rbtNodes, 0, size - 1);
		treeifyShallow(0, size - 1, rbtNodes, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Clears accessed node array
	delete [] rbtNodes;
}

/*
 * function_identifier: Deletes all nodes, in the red-black tree, which have the data in the nodes provided
 * parameters: 			The array length and an array of nodes
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_removeData(size_t arrayLength, rbtNode<K,D> **remNodeS) {
		// Ignores the trival case
	if (!size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE, **remNodeE;
	D *dataS, *dataE;

		// Gets the needed nodes and indexes
	IdxM = IdxS = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;
	remNodeE = remNodeS + arrayLength;
	dataS = new D[arrayLength];
	dataE = dataS + arrayLength;

		// Sorts the provided and accessed nodes
	fooPtr<K,D> = dataCompair;
	mergeSortCallerBase(rbtNodes, 0, size - 1);
	mergeSortCallerBase(remNodeS, 0, arrayLength - 1);

		// Saves the data values from the provided arrays
		// This ensures we don't access deleted memory by accident
	while(remNodeS != remNodeE)
		*dataS++ = (*remNodeS++)->data;

		// Resets 'infoS' back to start of allocated array
	dataS -= arrayLength;

		// Cycles through the entire array of accessed nodes and data
	while(IdxS != IdxE && dataS != dataE)
			// Checks if both considered nodes have equal data
			// If so, delete it and all similar nodes
			// If not, skip/save it, then move forward in one of the arrays
		if ((*IdxS)->data == *dataS)
			delete *IdxS++;
		else if ((*IdxS)->data < *dataS)
			*IdxM++ = *IdxS++;
		else
			dataS++;

		// Clears pair array
	delete [] (dataE - arrayLength);

		// Checks if any nodes were deleted
	if (IdxM != IdxS)
			// If some nodes were deleted, place the remaining nodes into the array
		while(IdxS != IdxE)
			*IdxM++ = *IdxS++;
	else
			// If no nodes deleted, just skip to the end
			// Everything is already fine how it is
		IdxM = IdxE;

		// Calculates the size of the array of non-deleted nodes
	size = IdxM - rbtNodes;

		// Checks if there is any nodes left to process
	if (size) {
			// If there are remaining nodes, sort them and create a tree from the result
		fooPtr<K,D> = keyDataCompair;
		mergeSortCallerBase(rbtNodes, 0, size - 1);
		treeifyShallow(0, size - 1, rbtNodes, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Clears accessed node array
	delete [] rbtNodes;
}

/*
 * function_identifier: 
 * parameters: 			
 * return value:		N/A
*/
template <typename K, typename D>
template <typename F>
void rbTree<K,D>::removeDuplicatesBase(F foo) {
		// Ignores the trival case
	if (size <= 1)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE;

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;

	foo(IdxS, IdxE, IdxM);

		// Calculates the size of the array of non-deleted nodes
	size = IdxM - rbtNodes + 1;

	if (size) {
			// Sort the nodes, if needed
		if (fooPtr<K,D> != keyDataCompair) {
			fooPtr<K,D> = keyDataCompair;
			mergeSortCallerBase(rbtNodes, 0, size - 1);
		}

			// If there are remaining nodes, create a tree from them
		treeifyShallow(0, size - 1, rbtNodes, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Clears accessed node array
	delete [] rbtNodes;
}

/*
 * function_identifier: Limits all nodes, in a red-black tree, to have a unique data and key pair
 *						Extras are deleted 
 * parameters: 			N/A
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_removeDuplicates() {
	removeDuplicatesBase([] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **&IdxE, rbtNode<K,D> **&IdxM) -> void {
			// Cycles through the entire array of accessed nodes
		while (++IdxS != IdxE) {
				// Checks if current node has equal data and key to the previously accepted node
				// If so, delete it and all similar nodes after
				// If not, accept it, then move forward in the array
			if ((*IdxS)->equal(*IdxM))
				delete *IdxS;
			else
				*++IdxM = *IdxS;
		}
		fooPtr<K,D> = keyDataCompair;
	});
}

/*
 * function_identifier: Limits all nodes, in a red-black tree, to have a unique keys
 *						Extras are deleted 
 * parameters: 			N/A
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_removeDuplicateKeys() {
	removeDuplicatesBase([] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **&IdxE, rbtNode<K,D> **&IdxM) -> void {
			// Cycles through the entire array of accessed nodes
		while (++IdxS != IdxE) {
				// Checks if current node has equal key to the previously accepted node
				// If so, delete it and all similar nodes after
				// If not, accept it, then move forward in the array
			if ((*IdxS)->key == (*IdxM)->key)
				delete *IdxS;
			else
				*++IdxM = *IdxS;
		}
		fooPtr<K,D> = keyDataCompair;
	});
}

/*
 * function_identifier: Limits all nodes, in a red-black tree, to have a unique data values
 *						Extras are deleted 
 * parameters: 			N/A
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_removeDuplicateData() {
	removeDuplicatesBase([this] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **&IdxE, rbtNode<K,D> **&IdxM) -> void {
			// Sorts the nodes by data
		fooPtr<K,D> = dataCompair;
		mergeSortCallerBase(IdxS, 0, size - 1);

			// Cycles through the entire array of accessed nodes
		while (++IdxS != IdxE) {
				// Checks if current node has equal data to the previously accepted node
				// If so, delete it and all similar nodes after
				// If not, accept it, then move forward in the array
			if ((*IdxS)->data == (*IdxM)->data)
				delete *IdxS;
			else
				*++IdxM = *IdxS;
		}
	});
}

#endif /* _rbt_INSERTDELETE */