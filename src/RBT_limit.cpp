#ifndef _rbt_LIMIT
#define _rbt_LIMIT
#include	"RBT_main.h"
#include	"RBT_limit.h"
#include	"RBT_traversal.h"
#include	"RBT_sort.h"

	/*	 GLOBAL VARIABLES	*/
		// External From 'RBT_sort.cpp'
template <typename K, typename D>
extern inline bool(*fooPtr)(rbtNode<K,D> *, rbtNode<K,D> *);

/*	============================================================================  */
/* |                                                                            | */
/* |                               TREE LIMITS                                  | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: Limits all nodes, in the red-black tree, to have a key between the two keys provided
 *						Deletes extras
 * parameters: 			The start and end keys of a range
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_clampKey(K st, K ed) {
		// Ignores the trival case
	if (st > ed) {
		rbt_clear();
		return;
	}

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxE;

		// Gets the needed nodes and indexes
	IdxS = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size - 1;

		// Deletes all nodes less than start key
	while(IdxE >= IdxS && (*IdxS)->key < st)
		delete *IdxS++;
		// Deletes all nodes greater than start end
	while(IdxE > IdxS && (*IdxE)->key > ed)
		delete *IdxE--;

		// Checks if there is any nodes left to process
	if (IdxE != IdxS) {
			// If there are remaining nodes, create a tree from them
		treeifyShallow(IdxS - rbtNodes, IdxE - rbtNodes, rbtNodes, &root);
		size = IdxE - IdxS + 1;
		root->parent = NULL;
	} else {
			// If no remaining nodes, set root to NULL and size to 0
		root = NULL; size = 0;
	}

		// Clears accessed node array
	delete [] rbtNodes;
}

/*
 * function_identifier: Limits all nodes, in the red-black tree, to have a key between the key ranges provided
 *						Deletes extras
 * parameters: 			The length of the arrays, an array of start keys, and an array of end keys
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_clampKey(size_t arrayLength, K *st, K *ed) {
		// Ignores the trival case
	if (!size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE;
	K *endPtr;

		// Sorts the given key ranges based on their start key
	quickSortPair(st, ed, 0, arrayLength - 1);

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;
	endPtr = st + arrayLength;

		// Cycles through the entire array of accessed nodes
	while(1) {
			// Deletes all nodes before the current start key and after the previous end key
		while((*IdxS)->key < *st) {
			delete *IdxS++;
				// Stops when we run out of nodes
			if (IdxS == IdxE)
				goto finish_clamp;
		}
			// Saves all nodes after the current start key and before the current end key 
		while((*IdxS)->key <= *ed) {
			*IdxM++ = *IdxS++;
				// Stops when we run out of nodes
			if (IdxS == IdxE)
				goto finish_clamp;
		}
			// Moves forward in the start and end key arrays
		st++; ed++;
			// Stops when we run out of ranges to process
		if (endPtr == st)
			break;
	}
		// Deletes any extra nodes
	while(IdxS != IdxE)
		delete *IdxS++;

finish_clamp:
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
 * function_identifier: Limits all nodes, in the red-black tree, to have a key outside the range of the two keys provided
 *						Deletes extras
 * parameters: 			The start and end keys of a range
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_excludeKey(K st, K ed) {
		// Ignores the trival case
	if (st > ed)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE;

		// Gets the needed nodes and indexes
	IdxS = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;

		// Skips all nodes before the start key
	while(IdxS < IdxE && (*IdxS)->key < st)
		IdxS++;

		// Sets this index as the begining of our deletion
	IdxM = IdxS;

		// Deletes all nodes until passing the end key
	while(IdxS < IdxE && (*IdxS)->key <= ed)
		delete *IdxS++;

		// Checks if any nodes were deleted
	if (IdxM != IdxS)
			// If some nodes were deleted, place the remaining nodes into the array
		while(IdxS < IdxE)
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
 * function_identifier: Limits all nodes, in the red-black tree, to have a key outside the ranges provided
 *						Deletes extras
 * parameters: 			The length of the arrays, an array of start keys, and an array of end keys
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_excludeKey(size_t arrayLength, K *st, K *ed) {
		// Ignores the trival case
	if (!size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE;
	K *endPtr;

		// Sorts the given key ranges based on their start key
	quickSortPair(st, ed, 0, arrayLength - 1);

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;
	endPtr = st + arrayLength;

		// Cycles through the entire array of accessed nodes
	while(1) {
			// Saves all nodes before the current start key and after the previous end key
		while((*IdxS)->key < *st) {
			*IdxM++ = *IdxS++;
				// Stops when we run out of nodes
			if (IdxS == IdxE)
				goto finish_clamp;
		}
			// Deletes all nodes after the current start key and before the current end key
		while((*IdxS)->key <= *ed) {
			delete *IdxS++;
				// Stops when we run out of nodes
			if (IdxS == IdxE)
				goto finish_clamp;
		}
			// Moves forward in the start and end key arrays
		st++; ed++;
			// Stops when we run out of ranges to process
		if (endPtr == st)
			break;
	}
		// Deletes any extra nodes
	while(IdxS != IdxE)
		*IdxM++ = *IdxS++;

finish_clamp:
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
 * function_identifier: Limits all nodes, in the red-black tree, to have a key less than or equal to the key provided
 *						Deletes extras
 * parameters: 			A key limit
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_upperLimitKey(K lim) {
		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxE;

		// Gets the needed nodes and indexes
	IdxS = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;

		// Skips all nodes until one with a greater key than the limit is found
	while(IdxS != IdxE && (*IdxS)->key <= lim)
		IdxS++;

		// Calculates the size of the new tree
	size = IdxS - rbtNodes;

		// Checks if there is any nodes left to process
	if (size) {
			// If there are remaining nodes, create a tree from them
		treeifyShallow(0, size - 1, rbtNodes, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Deletes all extra nodes
	while(IdxS != IdxE)
		delete *IdxS++;

		// Clears accessed node array
	delete [] rbtNodes;
}

/*
 * function_identifier: Limits all nodes, in the red-black tree, to have a key greater than or equal to the key provided
 *						Deletes extras
 * parameters: 			A key limit
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_lowerLimitKey(K lim) {
		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxE;

		// Gets the needed nodes and indexes
	IdxS = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;

		// Deletes all nodes until one with a greater key than the limit is found
	while(IdxS != IdxE && (*IdxS)->key < lim)
		delete *IdxS++;

		// Checks if there is any nodes left to process
	if (IdxE != IdxS) {
			// If there are remaining nodes, create a tree from them
		treeifyShallow(IdxS - rbtNodes, size - 1, rbtNodes, &root);
		size = IdxE - IdxS;
		root->parent = NULL;
	} else {
			// If no remaining nodes, set root to NULL and size to 0
		root = NULL; size = 0;
	}

		// Clears accessed node array
	delete [] rbtNodes;
}

/*
 * function_identifier: Deletes a certain number of the nodes with the highest keys
 * parameters: 			Number of nodes to delete
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_deleteHighestKey(size_t val) {
		// Ignores the trival cases
	if (!val)
		return;
	if (val >= size) {
		rbt_clear();
		return;
	}
		// Initializes the needed values
	rbtNode<K,D> **IdxS, **IdxE;

		// Gets the needed nodes and indexes
	IdxS = rbt_getAllNodes(); IdxE = IdxS + size;

		// Shortens the size
	size -= val;

		// Create a tree from vaild nodes
	treeifyShallow(0, size - 1, IdxS, &root);
	root->parent = NULL;

		// Deletes extra nodes
	IdxS += size;
	while(IdxS != IdxE)
		delete *IdxS++;

		// Clears accessed node array
	delete [] (IdxE - size - val);
}

/*
 * function_identifier: Removes a certain number of the nodes with the highest keys
 * parameters: 			Number of keys to remove
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_removeHighestKey(size_t val) {
		// Ignores the trival cases
	if (!val)
		return;
	if (val >= size) {
		rbt_clear();
		return;
	}
		// Initializes the needed values
	rbtNode<K,D> **IdxS, **IdxE;
	K keyCheck;

		// Gets the needed nodes and indexes
	IdxS = rbt_getAllNodes(); IdxE = IdxS + size - 1;

		// Deletes the last node
	keyCheck = (*IdxE)->key;
	delete *IdxE--;

		// Cycles through the array of accessed nodes until all nodes were considered or a break is reached
	while(IdxS <= IdxE) {
			// Checks if a node with a key is found
		if ((*IdxE)->key != keyCheck) {
				// If so, check if there are more nodes to remove
			if (--val > 0) {
					// If so, set a new key temp and start removing from here
				keyCheck = (*IdxE)->key;
			} else
					// Else, we are finished here
				break;
		}
			// Deletes the current node and decreases the index
		delete *IdxE--;
	}

		// Calculates the size of the array of non-deleted nodes
	size = IdxE - IdxS + 1;

		// Checks if there is any nodes left to process
	if (size) {
			// If there are remaining nodes, create a tree from them
		treeifyShallow(0, size - 1, IdxS, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Clears accessed node array
	delete [] IdxS;
}

/*
 * function_identifier: Deletes a certain number of the nodes with the lowest keys
 * parameters: 			Number of nodes to delete
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_deleteLowestKey(size_t val) {
		// Ignores the trival cases
	if (!val)
		return;
	if (val >= size) {
		rbt_clear();
		return;
	}
		// Initializes the needed values
	rbtNode<K,D> **IdxS, **IdxE;

		// Gets the needed nodes and indexes
	IdxS = rbt_getAllNodes(); IdxE = IdxS + val;

		// Create a tree from vaild nodes
	treeifyShallow(val, size - 1, IdxS, &root);
	root->parent = NULL;

		// Deletes extra nodes
	while(IdxS != IdxE)
		delete *IdxS++;

		// Clears accessed node array
	delete [] (IdxE - val);

		// Shortens the size
	size -= val;
}

/*
 * function_identifier: Removes a certain number of the nodes with the lowest keys
 * parameters: 			Number of keys to remove
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_removeLowestKey(size_t val) {
		// Ignores the trival cases
	if (!val)
		return;
	if (val >= size) {
		rbt_clear();
		return;
	}
		// Initializes the needed values
	rbtNode<K,D> **IdxS, **IdxE;
	K keyCheck;

		// Gets the needed nodes and indexes
	IdxS = rbt_getAllNodes(); IdxE = IdxS + size;

		// Deletes the first node
	keyCheck = (*IdxS)->key;
	delete *IdxS++;

		// Cycles through the array of accessed nodes until all nodes were considered or a break is reached
	while(IdxS != IdxE) {
			// Checks if a node with a key is found
		if ((*IdxS)->key != keyCheck) {
				// If so, check if there are more nodes to remove
			if (--val > 0) {
					// If so, set a new key temp and start removing from here
				keyCheck = (*IdxS)->key;
			} else
					// Else, we are finished here
				break;
		}
			// Deletes the current node and increases the index
		delete *IdxS++;
	}

		// Calculates the size of the array of non-deleted nodes
	IdxE -= size;
	size += IdxE - IdxS;

		// Checks if there is any nodes left to process
	if (size) {
			// If there are remaining nodes, create a tree from them
		treeifyShallow(0, size - 1, IdxS, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Clears accessed node array
	delete [] IdxE;
}

/*
 * function_identifier: Deletes a certain number of the nodes with the highest data values
 * parameters: 			Number of nodes to delete
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_deleteHighestData(size_t val) {
		// Ignores the trival cases
	if (!val)
		return;
	if (val >= size) {
		rbt_clear();
		return;
	}
		// Initializes the needed values
	rbtNode<K,D> **IdxS, **IdxE;

		// Gets the needed nodes and indexes
	IdxS = rbt_getAllNodes(); IdxE = IdxS + size;

		// Gets a sorts a subset of the nodes, divided via their data
	fooPtr<K,D> = dataCompair;
	mergeSortCallerBase(IdxS, 0, size - 1);
	size -= val; // Shortens the size
	fooPtr<K,D> = keyDataCompair;
	mergeSortCallerBase(IdxS, 0, size - 1);

		// Create a tree from vaild nodes
	treeifyShallow(0, size - 1, IdxS, &root);
	root->parent = NULL;

		// Deletes extra nodes
	IdxS += size;
	while(IdxS != IdxE)
		delete *IdxS++;

		// Clears accessed node array
	delete [] (IdxE - size - val);
}

/*
 * function_identifier: Removes a certain number of the nodes with the highest data values
 * parameters: 			Number of data values to remove
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_removeHighestData(size_t val) {
		// Ignores the trival cases
	if (!val)
		return;
	if (val >= size) {
		rbt_clear();
		return;
	}
		// Initializes the needed values
	rbtNode<K,D> **IdxS, **IdxE;
	D dataCheck;

		// Gets the needed nodes and indexes
	IdxS = rbt_getAllNodes(); IdxE = IdxS + size - 1;

		// Sorts the nodes
	fooPtr<K,D> = dataCompair;
	mergeSortCallerBase(IdxS, 0, size - 1);

		// Deletes the last node
	dataCheck = (*IdxE)->data;
	delete *IdxE--;

		// Cycles through the array of accessed nodes until all nodes were considered or a break is reached
	while(IdxS <= IdxE) {
			// Checks if a node with a data is found
		if ((*IdxE)->data != dataCheck) {
				// If so, check if there are more nodes to remove
			if (--val > 0)
					// If so, set a new data temp and start removing from here
				dataCheck = (*IdxE)->data;
			else
					// Else, we are finished here
				break;
		}
			// Deletes the current node and decreases the index
		delete *IdxE--;
	}

		// Calculates the size of the array of non-deleted nodes
	size = IdxE - IdxS + 1;

		// Checks if there is any nodes left to process
	if (size) {
			// If there are remaining nodes, sort them and create a tree from the result
		fooPtr<K,D> = keyDataCompair;
		mergeSortCallerBase(IdxS, 0, size - 1);
		treeifyShallow(0, size - 1, IdxS, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Clears accessed node array
	delete [] IdxS;
}

/*
 * function_identifier: Deletes a certain number of the nodes with the lowest data values
 * parameters: 			Number of nodes to delete
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_deleteLowestData(size_t val) {
		// Ignores the trival cases
	if (!val)
		return;
	if (val >= size) {
		rbt_clear();
		return;
	}
		// Initializes the needed values
	rbtNode<K,D> **IdxS, **IdxE;

		// Gets the needed nodes and indexes
	IdxS = rbt_getAllNodes(); IdxE = IdxS + val;

		// Gets a sorts a subset of the nodes, divided via their data
	fooPtr<K,D> = dataCompair;
	mergeSortCallerBase(IdxS, 0, size - 1);
	size -= val; // Shortens the size
	fooPtr<K,D> = keyDataCompair;
	mergeSortCallerBase(IdxE, 0, size - 1);

		// Create a tree from vaild nodes
	treeifyShallow(0, size - 1, IdxE, &root);
	root->parent = NULL;

		// Deletes extra nodes
	while(IdxS != IdxE)
		delete *IdxS++;

		// Clears accessed node array
	delete [] (IdxE - val);
}

/*
 * function_identifier: Removes a certain number of the nodes with the lowest data values
 * parameters: 			Number of data values to remove
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_removeLowestData(size_t val) {
		// Ignores the trival cases
	if (!val)
		return;
	if (val >= size) {
		rbt_clear();
		return;
	}
		// Initializes the needed values
	rbtNode<K,D> **IdxS, **IdxE, **rbtNodes;
	D dataCheck;

		// Gets the needed nodes and indexes
	IdxS = rbtNodes = rbt_getAllNodes(); IdxE = IdxS + size;
	fooPtr<K,D> = dataCompair;
	mergeSortCallerBase(IdxS, 0, size - 1);

		// Deletes the first node
	dataCheck = (*IdxS)->data;
	delete *IdxS++;

		// Cycles through the array of accessed nodes until all nodes were considered or a break is reached
	while(IdxS != IdxE) {
			// Checks if a node with a data is found
		if ((*IdxS)->data != dataCheck) {
				// If so, check if there are more nodes to remove
			if (--val > 0)
					// If so, set a new key temp and start removing from here
				dataCheck = (*IdxS)->data;
			else
					// Else, we are finished here
				break;
		}
			// Deletes the current node and increases the index
		delete *IdxS++;
	}

		// Calculates the size of the array of non-deleted nodes
	size = IdxE - IdxS;

		// Checks if there is any nodes left to process
	if (size) {
			// If there are remaining nodes, sort them and create a tree from the result
		fooPtr<K,D> = keyDataCompair;
		mergeSortCallerBase(IdxS, 0, size - 1);
		treeifyShallow(0, size - 1, IdxS, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Clears accessed node array
	delete [] rbtNodes;
}

#endif /* _rbt_LIMIT */