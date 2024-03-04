#ifndef _rbt_LIMIT
#define _rbt_LIMIT
#include	"RBT_main.h"
#include	"RBT_limit.h"
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
/* |                               TREE LIMITS                                  | */
/* |                                                                            | */
/*	============================================================================  */

	/*		KEYS DATA	  */

/*
 * function_identifier: 
 * parameters: 			
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_clamp(K stK, K edK, D stD, D edD) {
		// Ignores the trival case
	if (stK > edK || stD > edD) {
		rbt_clear();
		return;
	}

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxE;

		// Gets the needed nodes and indexes
	IdxS = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size - 1;

		// Deletes all nodes less than start key
	while(IdxE >= IdxS && (*IdxS)->key < stK)
		delete *IdxS++;
		// Deletes all nodes greater than end key
	while(IdxE > IdxS && (*IdxE)->key > edK)
		delete *IdxE--;

		// Initializes extra variables
	rbtNode<K,D> **IdxM = IdxS, **markS = IdxS;
		// Deletes all nodes with data not in range
	while(IdxE >= IdxS) {
		if ((*IdxS)->data < stD || (*IdxS)->data > edD)
			delete *IdxS++;
		else
			*IdxM++ = *IdxS++;
	}
		// Repositions the start and end indexes
	IdxE = IdxM; IdxS = markS;

		// Checks if there is any nodes left to process
	if (IdxE != IdxS) {
			// If there are remaining nodes, create a tree from them
		treeifyShallow(IdxS - rbtNodes, IdxE - rbtNodes - 1, rbtNodes, &root);
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
 * function_identifier: 
 * parameters: 			
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_clamp(size_t arrayLengthK, K *stK, K *edK, size_t arrayLengthD, D *stD, D *edD) {
		// Ignores the trival case
	if (!size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE;
	K tempKey, *endPtrK; D *endPtrD; D *stDBase = stD, *edDBase = edD;

		// Sorts the given ranges based on their start value
	quickSortPair(stK, edK, 0, arrayLengthK - 1);
	quickSortPair(stD, edD, 0, arrayLengthD - 1);

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;
	endPtrK = stK + arrayLengthK; endPtrD = stD + arrayLengthD;

		// Cycles through the entire array of accessed nodes
	while(1) {
start_exclude:
			// Deletes all nodes before the current start key and after the previous end key
		while((*IdxS)->key < *stK) {
			delete *IdxS++;
				// Stops when we run out of nodes
			if (IdxS == IdxE)
				goto finish_clamp;
		}
			// Saves all nodes after the current start key and before the current end key 
		while((*IdxS)->key <= *edK) {
				// Moves up in the data array if current node is outside of it
			while(stD < endPtrD && (*IdxS)->data > *edD) {
				stD++; edD++;
					// If we reached the end of the data array, then all future nodes (with
					// the current key) are outside of the clamp. Delete them
				if (stD == endPtrD) {
						// Saves the current node's key for comparison
					tempKey = (*IdxS)->key;
					while(true) {
							// Deletes the current node and moves forward in the array
						delete *IdxS++;
							// Stops when we run out of nodes
						if (IdxS == IdxE)
							goto finish_clamp;
							// Escapes when we find a node with a different key
						if (tempKey != (*IdxS)->key)
							break;
					}

						// Reset base data range indexes and resume normal pratice
					stD = stDBase; edD = edDBase;
					goto start_exclude;
				}
			}
				// Saves the current node's key for comparison
			tempKey = (*IdxS)->key;
				// Checks if to save or delete the current node
			if ((*IdxS)->data < *stD)
				delete *IdxS++;
			else
				*IdxM++ = *IdxS++;
				// Stops when we run out of nodes
			if (IdxS == IdxE)
				goto finish_clamp;
				// Reset base data range indexes when we find a node with a different key
			if (tempKey != (*IdxS)->key) {
				stD = stDBase; edD = edDBase;
			}
		}
			// Moves forward in the start and end key arrays
		stK++; edK++;
			// Stops when we run out of ranges to process
		if (endPtrK == stK)
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
 * function_identifier: 
 * parameters: 			
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_exclude(K stK, K edK, D stD, D edD) {
		// Ignores the trival case
	if (stK > edK)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE;

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;

	while(IdxS < IdxE && (*IdxS)->key < stK) {
		if ((*IdxS)->data < stD || (*IdxS)->data > edD)
			*IdxM++ = *IdxS++;
		else
			delete *IdxS++;
	}
	while(IdxS < IdxE && (*IdxS)->key <= edK)
		delete *IdxS++;
	while(IdxS < IdxE) {
		if ((*IdxS)->data < stD || (*IdxS)->data > edD)
			*IdxM++ = *IdxS++;
		else
			delete *IdxS++;
	}

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
 * function_identifier: 
 * parameters: 			
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_exclude(size_t arrayLengthK, K *stK, K *edK, size_t arrayLengthD, D *stD, D *edD) {
		// Ignores the trival case
	if (!size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE;
	K tempKey, *endPtrK; D *endPtrD; D *stDBase = stD, *edDBase = edD;

		// Sorts the given ranges based on their start value
	quickSortPair(stK, edK, 0, arrayLengthK - 1);
	quickSortPair(stD, edD, 0, arrayLengthD - 1);

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;
	endPtrK = stK + arrayLengthK; endPtrD = stD + arrayLengthD;

		// Cycles through the entire array of accessed nodes
	while(1) {
start_exclude:
			// Saves all nodes before the current start key and after the previous end key
		while(endPtrK == stK || (*IdxS)->key < *stK) {
				// Moves up in the data array if current node is outside of it
			while(stD < endPtrD && (*IdxS)->data > *edD) {
				stD++; edD++;
					// If we reached the end of the data array, then all future nodes (with
					// the current key) are outside of the clamp. Delete them
				if (stD == endPtrD) {
					while(true) {
						*IdxM++ = *IdxS++;
							// Stops when we run out of nodes
						if (IdxS == IdxE)
							goto finish_exclude;
							// Escapes when we find a node with a different key
						if ((*(IdxS - 1))->key != (*IdxS)->key)
							break;
					}

						// Reset base data range indexes and resume normal pratice
					stD = stDBase; edD = edDBase;
					goto start_exclude;
				}
			}
				// Saves the current node's key for comparison
			tempKey = (*IdxS)->key;
				// Checks if to save or delete the current node
			if ((*IdxS)->data >= *stD)
				delete *IdxS++;
			else
				*IdxM++ = *IdxS++;
				// Stops when we run out of nodes
			if (IdxS == IdxE)
				goto finish_exclude;
				// Reset base data range indexes when we find a node with a different key
			if (tempKey != (*IdxS)->key) {
				stD = stDBase; edD = edDBase;
			}
		}
			// Deletes all nodes after the current start key and before the current end key
		while((*IdxS)->key <= *edK) {
			delete *IdxS++;
				// Stops when we run out of nodes
			if (IdxS == IdxE)
				goto finish_exclude;
		}
			// Moves forward in the start and end key arrays
		stK++; edK++;
	}
finish_exclude:
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

	/*		KEYS	  */

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
		// Deletes all nodes greater than end key
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
				goto finish_clamp_key;
		}
			// Saves all nodes after the current start key and before the current end key 
		while((*IdxS)->key <= *ed) {
			*IdxM++ = *IdxS++;
				// Stops when we run out of nodes
			if (IdxS == IdxE)
				goto finish_clamp_key;
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

finish_clamp_key:
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
				goto finish_exclude_key;
		}
			// Deletes all nodes after the current start key and before the current end key
		while((*IdxS)->key <= *ed) {
			delete *IdxS++;
				// Stops when we run out of nodes
			if (IdxS == IdxE)
				goto finish_exclude_key;
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

finish_exclude_key:
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

	/*		DATA	  */

/*
 * function_identifier: Limits all nodes, in the red-black tree, to have a data between the two data provided
 *						Deletes extras
 * parameters: 			The start and end data of a range
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_clampData(D st, D ed) {
		// Ignores the trival case
	if (st > ed) {
		rbt_clear();
		return;
	}

		// Initializes the needed values
	rbtNode<K,D> **IdxS, **IdxM, **IdxE;

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbt_getAllNodes();
	IdxE = IdxS + size - 1;

		// Cycles through all nodes
			// Saves nodes within range, deletes all nodes that are outside
	while(IdxE >= IdxS)
			// Deletes nodes outside range
		if ((*IdxS)->data < st || (*IdxS)->data > ed)
			delete *IdxS++;
		else
			// Saves node at next vaild spot
			*IdxM++ = *IdxS++;

		// Resets IdxS to start of node array and calculates the size
	IdxS -= size;
	size = IdxM - IdxS;

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
 * function_identifier: Limits all nodes, in the red-black tree, to have a data between the data ranges provided
 *						Deletes extras
 * parameters: 			The length of the arrays, an array of start data, and an array of end data
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_clampData(size_t arrayLength, D *st, D *ed) {
		// Ignores the trival case
	if (!size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE;
	D *endPtr;

		// Sorts the given data ranges based on their start data
	quickSortPair(st, ed, 0, arrayLength - 1);

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;
	endPtr = st + arrayLength;

		// Sorts the nodes
	fooPtr<K,D> = dataCompair;
	mergeSortCallerBase(rbtNodes, 0, size - 1);

		// Cycles through the entire array of accessed nodes
	while(1) {
			// Deletes all nodes before the current start data and after the previous end data
		while((*IdxS)->data < *st) {
			delete *IdxS++;
				// Stops when we run out of nodes
			if (IdxS == IdxE)
				goto finish_clamp_data;
		}
			// Saves all nodes after the current start data and before the current end data 
		while((*IdxS)->data <= *ed) {
			*IdxM++ = *IdxS++;
				// Stops when we run out of nodes
			if (IdxS == IdxE)
				goto finish_clamp_data;
		}
			// Moves forward in the start and end data arrays
		st++; ed++;
			// Stops when we run out of ranges to process
		if (endPtr == st)
			break;
	}
		// Deletes any extra nodes
	while(IdxS != IdxE)
		delete *IdxS++;

finish_clamp_data:
		// Calculates the size of the array of non-deleted nodes
	size = IdxM - rbtNodes;

		// Checks if there is any nodes left to process
	if (size) {
			// Sorts the nodes
		fooPtr<K,D> = keyDataCompair;
		mergeSortCallerBase(rbtNodes, 0, size - 1);
		
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
 * function_identifier: Limits all nodes, in the red-black tree, to have a key outside the range of the two data provided
 *						Deletes extras
 * parameters: 			The start and end data of a range
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_excludeData(D st, D ed) {
		// Ignores the trival case
	if (st > ed)
		return;

		// Initializes the needed values
	rbtNode<K,D> **IdxS, **IdxM, **IdxE;

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbt_getAllNodes();
	IdxE = IdxS + size - 1;

		// Cycles through all nodes
			// Saves nodes outside range, deletes all nodes that are inside
	while(IdxE >= IdxS)
			// Saves node at next vaild spot if outside range
		if ((*IdxS)->key < st || (*IdxS)->key > ed)
			*IdxM++ = *IdxS++;
		else
			// Deletes nodes inside range
			delete *IdxS++;

		// Resets IdxS to start of node array and calculates the size
	IdxS -= size;
	size = IdxM - IdxS;

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
 * function_identifier: Limits all nodes, in the red-black tree, to have a data outside the ranges provided
 *						Deletes extras
 * parameters: 			The length of the arrays, an array of start data, and an array of end data
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_excludeData(size_t arrayLength, D *st, D *ed) {
		// Ignores the trival case
	if (!size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE;
	D *endPtr;

		// Sorts the given data ranges based on their start data
	quickSortPair(st, ed, 0, arrayLength - 1);

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;
	endPtr = st + arrayLength;

		// Sorts the nodes
	fooPtr<K,D> = dataCompair;
	mergeSortCallerBase(rbtNodes, 0, size - 1);

		// Cycles through the entire array of accessed nodes
	while(1) {
			// Saves all nodes before the current start data and after the previous end data
		while((*IdxS)->data < *st) {
			*IdxM++ = *IdxS++;
				// Stops when we run out of nodes
			if (IdxS == IdxE)
				goto finish_exclude_data;
		}
			// Deletes all nodes after the current start data and before the current end data
		while((*IdxS)->data <= *ed) {
			delete *IdxS++;
				// Stops when we run out of nodes
			if (IdxS == IdxE)
				goto finish_exclude_data;
		}
			// Moves forward in the start and end data arrays
		st++; ed++;
			// Stops when we run out of ranges to process
		if (endPtr == st)
			break;
	}
		// Deletes any extra nodes
	while(IdxS != IdxE)
		*IdxM++ = *IdxS++;

finish_exclude_data:
		// Calculates the size of the array of non-deleted nodes
	size = IdxM - rbtNodes;

		// Checks if there is any nodes left to process
	if (size) {
			// Sorts the nodes
		fooPtr<K,D> = keyDataCompair;
		mergeSortCallerBase(rbtNodes, 0, size - 1);

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
 * function_identifier: Limits all nodes, in the red-black tree, to have a data less than or equal to the data provided
 *						Deletes extras
 * parameters: 			A data limit
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_upperLimitData(D lim) {
		// Ignores the trival case
	if (!size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE;

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;

		// Deletes all nodes with a greater data
	do {
		if ((*IdxS)->data <= lim)
			*IdxM++ = *IdxS;
		else
			delete *IdxS;
	} while(++IdxS != IdxE);

		// Calculates the size of the new tree
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
 * function_identifier: Limits all nodes, in the red-black tree, to have a data greater than or equal to the data provided
 *						Deletes extras
 * parameters: 			A data limit
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_lowerLimitData(D lim) {
		// Ignores the trival case
	if (!size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE;

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;

		// Deletes all nodes with a smaller data
	do {
		if ((*IdxS)->data >= lim)
			*IdxM++ = *IdxS;
		else
			delete *IdxS;
	} while(++IdxS != IdxE);

		// Calculates the size of the new tree
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