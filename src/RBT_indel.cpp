#ifndef _rbt_INSERTDELETE
#define _rbt_INSERTDELETE
#include	"RBT_main.h"
#include	"RBT_indel.h"
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

		// External From 'def.h'
	extern inline void *rbt__otherData__;

		// Base type for zipping
	template <typename K, typename D>
	struct rbt__pair__ {K key; D data; bool operator<=(const rbt__pair__ &info) {return (key < info.key || (key == info.key && data <= info.data));};};

	/*
	 * function_identifier: Zips two arrays (a key array and a data array) into a single usable array
	 * parameters: 			The length of the arrays, an array of keys, and an array of data values
	 * return value:		The zip array of the provided arrays
	*/
	template <typename K, typename D>
	inline struct rbt__pair__<K,D> *rbt__zip__(size_t arrayLength, K *keysS, D *dataS) {
			// Initializes needed variables and indexes
		struct rbt__pair__<K,D> *infoS = new struct rbt__pair__<K,D>[arrayLength];
		K *keysE = keysS + arrayLength;

			// Copies over values
		while(keysS != keysE) {
			(*infoS).key = *keysS++;
			(*infoS++).data = *dataS++;
		}

			// Returns new array
		return infoS - arrayLength;
	}

	/*
	 * function_identifier: Expands an given array to a given length
	 * parameters: 			An array of elements, the current length of the array, and the array's new desired length
	 * return value:		The expanded array's address
	*/
	template <typename T>
	inline T *rbt__expand__(T *array, size_t currLen, size_t newLen) {
			// Initializes needed variables and indexes
		T *temp = array, *st = new T[newLen];

			// Copies over values
		for(size_t idx = 0; idx < currLen; idx++)
			*st++ = *temp++;

			// Clears old array
		delete [] array;

			// Returns new array
		return st;
	}
}

/*	============================================================================  */
/* |                                                                            | */
/* |                                MULTI INSERT                                | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: 
 * parameters: 			
 * return value:		N/A
*/
template <typename K, typename D>
template <typename F>
void rbTree<K,D>::rbt_repInsertBase(size_t arrayLength, F foo, bool sortHow) {
		// Ignores the trival case
	if (!arrayLength)
		return;

		// Checks for node overflow
	if (size > (size + arrayLength))
		throw std::overflow_error("overflow - red black tree(s) to large to perform opperation");

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS;

		// Gets the needed nodes and indexes
	rbtNodes = new rbtNode<K,D> *[size + arrayLength];
	rbt_getAllNodes(rbtNodes);
	IdxS = rbtNodes + size;

	foo(IdxS, IdxS + arrayLength);

	fooPtr<K,D> = keyDataCompair;
	if (sortHow) {
				// Sorts the inserted nodes
		mergeSortCallerBase(rbtNodes, size, size + arrayLength - 1);
			// Merges the nodes with the other nodes in the red-black tree
				// The merged result will be sorted in ascending order of keys and data (primarily sorted by keys)
		mergeSortBase(rbtNodes, 0, size - 1, size + arrayLength);
	}
	else
		mergeSortBase(rbtNodes, 0, size, size + arrayLength);

		// Uses the nodes to reconstruct the tree
	size += arrayLength;
	treeifyShallow(0, size - 1, rbtNodes, &root);
	root->parent = NULL;

		// Deletes the remaining array
	delete [] rbtNodes;
}

/*
 * function_identifier: Inserts multiple nodes into the red-black tree, according to the provided arrays
 * parameters: 			The length of the arrays, an array of keys, and an array of data values
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_repInsert(size_t arrayLength, K *keys, D *data) {
	rbt__otherData__ = new std::byte *[2];
	((K **)rbt__otherData__)[0] = keys;
	((D **)rbt__otherData__)[1] = data;

	rbt_repInsertBase(arrayLength, [] (rbtNode<K,D> **IdxS, rbtNode<K,D> **IdxE) -> void {
		K *keys = ((K **)rbt__otherData__)[0];
		D *data = ((D **)rbt__otherData__)[1];

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
	}, true);

	delete [] (std::byte **)rbt__otherData__;
}

/*
 * function_identifier: Inserts multiple nodes into the red-black tree, according to the provided array
 * parameters: 			The length of the array and an array of nodes
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_repInsert(size_t arrayLength, rbtNode<K,D> **inS) {
	rbt__otherData__ = (std::byte**)inS;

	rbt_repInsertBase(arrayLength, [] (rbtNode<K,D> **IdxS, rbtNode<K,D> **IdxE) -> void {
		rbtNode<K,D> **rbtNodes = (rbtNode<K,D> **)rbt__otherData__;

			// Deep copies the given nodes
		while(IdxS != IdxE)
			*IdxS++ = new rbtNode<K,D>(*rbtNodes++);
	}, true);
}

/*
 * function_identifier: Inserts multiple nodes into the red-black tree, with a key value that changes via a
 *						provided function
 * parameters: 			The length of the array, the starting key, the step function for the key, and an array of data
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_repInsertFunc(size_t arrayLength, K st, K (*step)(K, D), D *data) {
	typedef K (*stepType)(K, D);

	rbt__otherData__ = new std::byte *[3];
	((K **)rbt__otherData__)[0] = &st;
	((D **)rbt__otherData__)[1] = data;
	((stepType **)rbt__otherData__)[2] = &step;

	rbt_repInsertBase(arrayLength, [] (rbtNode<K,D> **IdxS, rbtNode<K,D> **IdxE) -> void {
		K st = *((K **)rbt__otherData__)[0];
		D *data = ((D **)rbt__otherData__)[1];
		stepType step = *((stepType **)rbt__otherData__)[2];

			// Checks if given a data array to work with
		if (data == NULL) {
				// If not, then just use the default constructor for the data type
			while(IdxS != IdxE) {
				*IdxS++ = new rbtNode<K,D>(st, D());
				st = step(st, D()); // Increases the key step
			}
		} else {
				// If so, use the provided array data values
			while(IdxS != IdxE) {
				*IdxS++ = new rbtNode<K,D>(st, *data);
				st = step(st, *data++); // Increases the key step
			}
		}
	}, false);

	delete [] (std::byte **)rbt__otherData__;
}

/*
 * function_identifier: Inserts multiple nodes into the red-black tree, with an increasing key amount
 * parameters: 			The length of the array, the starting key, the step key value, and an array of data
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_repInsertInc(size_t arrayLength, K st, K step, D *data) {
	rbt__otherData__ = new std::byte *[3];
	((K **)rbt__otherData__)[0] = &st;
	((D **)rbt__otherData__)[1] = data;
	((K **)rbt__otherData__)[2] = &step;

	rbt_repInsertBase(arrayLength, [] (rbtNode<K,D> **IdxS, rbtNode<K,D> **IdxE) -> void {
		K st = *((K **)rbt__otherData__)[0];
		D *data = ((D **)rbt__otherData__)[1];
		K step = *((K **)rbt__otherData__)[2];

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
	}, false);

	delete [] (std::byte *)rbt__otherData__;
}

/*
 * function_identifier: Inserts multiple nodes into the red-black tree, with an decreasing key amount
 * parameters: 			The length of the array, the starting key, the step key value, and an array of data
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_repInsertDec(size_t arrayLength, K st, K step, D *data) {
	rbt__otherData__ = new std::byte *[3];
	((K **)rbt__otherData__)[0] = &st;
	((D **)rbt__otherData__)[1] = data;
	((K **)rbt__otherData__)[2] = &step;

	rbt_repInsertBase(arrayLength, [] (rbtNode<K,D> **IdxS, rbtNode<K,D> **IdxE) -> void {
		K st = *((K **)rbt__otherData__)[0];
		D *data = ((D **)rbt__otherData__)[1];
		K step = *((K **)rbt__otherData__)[2];

			// Checks if given a data array to work with
		if (data == NULL) {
				// If not, then just use the default constructor for the data type
			while(IdxS != IdxE) {
				*IdxS++ = new rbtNode<K,D>(st, D());
				st -= step; // Increases the key step
			}
		} else {
				// If so, use the provided array data values
			while(IdxS != IdxE) {
				*IdxS++ = new rbtNode<K,D>(st, *data++);
				st -= step; // Increases the key step
			}
		}
	}, false);

	delete [] (std::byte *)rbt__otherData__;
}

/*
 * function_identifier: Inserts multiple nodes into the red-black tree, via a 'for-loop' method
 * parameters: 			A one-time initialize function that returns a key and data by reference and return void,
						a conditional function that requires one key and one data parameters and return a boolean,
						a step function that returns a key and data by reference and return void
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_repInsertFor(void (*initialize)(K &, D &), bool (*condition)(K &, D &), void (*step)(K &, D &)) {
		// Initializes base key and data by calling the given 'initialize' function
	K key; D data;
	initialize(key, data);

		// Initializes node arrays and grabs all nodes currently in the function
	size_t idx = size, nodeBufferLen = (size << 1) ? (size << 1) : 8;
	rbtNode<K,D> **rbtNodes = new rbtNode<K,D> *[nodeBufferLen];
	rbt_getAllNodes(rbtNodes);
	rbtNodes += idx;

		// Loops if the 'condition' function returns true
	while(condition(key, data)) {
			// Expands the node array if we ran out of space
		if (idx == nodeBufferLen) {
			rbtNodes = rbt__expand__(rbtNodes - nodeBufferLen, nodeBufferLen, nodeBufferLen << 1);
			nodeBufferLen <<= 1;
		}

			// Then creates a new node in the node array with the given values
		*rbtNodes++ = new rbtNode<K,D>(key, data); idx++;

			// Checks for node overflow
		if (idx == (size_t)-1)
			throw std::overflow_error("overflow - red black tree(s) to large to perform opperation");

			// Uses the 'step' function to gets the next key and data values
		step(key, data);
	}
		// Moves rbtNodes back to its start
	rbtNodes -= idx;

		// Merges and sorts all the nodes in the node array
	fooPtr<K,D> = keyDataCompair;
	mergeSortCallerBase(rbtNodes, size, idx - 1);
	mergeSortBase(rbtNodes, 0, size - 1, idx);

		// Uses the sorted node array to reconstruct the tree
	size = idx;
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

/*		   BASE			*/

/*
 * function_identifier: The base template for repDelete functions (node array)
 * parameters: 			An the array length, an array of nodes, a function to select which nodes to delete,
 *						and a boolean to check if this call is in data context
 * return value:		N/A
*/
template <typename K, typename D>
template <typename F>
inline void rbTree<K,D>::repDeleteBase(size_t arrayLength, rbtNode<K,D> **delS, F foo, bool data) {
		// Ignores the trival case
	if (!arrayLength || !size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **delNodes, **IdxS, **IdxM, **IdxE, **delE, **remS;

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;
	delE = delS + arrayLength;
	remS = delNodes = new rbtNode<K,D> *[arrayLength]();

		// Sorts the nodes
	if (data) {
			// If data, sort by data
		fooPtr<K,D> = dataCompair;
		mergeSortCallerBase(rbtNodes, 0, size - 1);
	} else
		fooPtr<K,D> = keyDataCompair;
	mergeSortCallerBase(delS, 0, arrayLength - 1);

		// Cycles through the entire node array
	foo(IdxS, IdxM, IdxE, delS, delE, remS);

		// Frees nodes issued to be deleted
	delE = delNodes;
	if (delNodes != remS) do {
		delete *delNodes++;
	} while(delNodes != remS);

		// Clears delete node array
	delete [] delE;

		// Places remaining, non-deleted, nodes into the array
	if (IdxS != IdxM) do {
		*IdxM++ = *IdxS++;
	} while(IdxS != IdxE);

		// Calculates the size of the array of non-deleted nodes
	size = IdxM - rbtNodes;

		// Checks if there is any nodes left to process
	if (size) {
			// If there are remaining nodes, sort them (if needed) and create a tree from the result
		if (data) {
				// If data, sort them back in order
			fooPtr<K,D> = keyDataCompair;
			mergeSortCallerBase(rbtNodes, 0, size - 1);
		}
		treeifyShallow(0, size - 1, rbtNodes, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Clears accessed node array
	delete [] rbtNodes;
}

/*
 * function_identifier: The base template for repDelete functions (raw data array(s))
 * parameters: 			An the array length, an array of valuess, a function to select which nodes to
 *						delete, and a boolean to check if this call is in data context
 * return value:		N/A
*/
template <typename K, typename D>
template <typename F, typename T>
inline void rbTree<K,D>::repDeleteBase(size_t arrayLength, T *valueS, F foo, bool data) {
		// Ignores the trival case
	if (!arrayLength || !size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE;
	T *valueE;

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;
	valueE = valueS + arrayLength;

		// Sorts the values and nodes
	if (data) {
		fooPtr<K,D> = dataCompair;
		mergeSortCallerBase(rbtNodes, 0, size - 1);
	} else
		fooPtr<K,D> = keyDataCompair;
	quickSort(valueS, valueE - 1);

	foo(IdxS, IdxM, IdxE, valueS, valueE);

		// Places remaining, non-deleted, nodes into the array
	if (IdxS != IdxM) do {
		*IdxM++ = *IdxS++;
	} while(IdxS != IdxE);

		// Calculates the size of the array of non-deleted nodes
	size = IdxM - rbtNodes;

		// Checks if there is any nodes left to process
	if (size) {
			// If there are remaining nodes, sort them (if needed) and create a tree from the result
		if (data) {
			fooPtr<K,D> = keyDataCompair;
			mergeSortCallerBase(rbtNodes, 0, size - 1);
		}
		treeifyShallow(0, size - 1, rbtNodes, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Clears accessed node array
	delete [] rbtNodes;
}

/*		   KEY_DATA			*/

/*
 * function_identifier: Deletes multiple nodes from the red-black tree
 * parameters: 			The length of the array and an array of nodes
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_repDelete(size_t arrayLength, rbtNode<K,D> **delS) {
	repDeleteBase(arrayLength, delS, [] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **&IdxM, rbtNode<K,D> **IdxE, rbtNode<K,D> **delS, rbtNode<K,D> **delE, rbtNode<K,D> **&remS) -> void {
			// Cycles through the entire array of accessed nodes
		while(IdxS != IdxE && delS != delE)
				// Checks if both considered nodes have equal keys and data
				// If so, delete it and move forward
				// If not, skip/save it, then move forward in one of the arrays
			if ((*IdxS)->key == (*delS)->key) {
				if ((*IdxS)->data == (*delS)->data) {
					*remS++ = *IdxS++; delS++;
				} else if ((*IdxS)->data < (*delS)->data)
					*IdxM++ = *IdxS++;
				else
					delS++;
			} else if ((*IdxS)->key < (*delS)->key)
				*IdxM++ = *IdxS++;
			else
				delS++;
    }, false);
}

/*
 * function_identifier: Deletes multiple nodes, from the red-black tree, which have the same keys and data provided
 * parameters: 			The length of the arrays, an array of keys, and an array of data
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_repDelete(size_t arrayLength, K *keysS, D *dataS) {
		// Ignores the trival case
	if (!size)
		return;

		// Zip the given value array
	rbt__pair__<K,D> *infoS = rbt__zip__(arrayLength, keysS, dataS);

		// Calls the main base function
	repDeleteBase(arrayLength, infoS, [] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **&IdxM, rbtNode<K,D> **IdxE, rbt__pair__<K,D> *infoS, rbt__pair__<K,D> *infoE) -> void {
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
    }, false);

		// Clears pair array
	delete [] infoS;
}

/*		 KEY 		*/

/*
 * function_identifier: Deletes multiple nodes, from the red-black tree, which have the same keys provided
 * parameters: 			The length of the arrays and an array of nodes with keys
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_repDeleteKey(size_t arrayLength, rbtNode<K,D> **delS) {
	repDeleteBase(arrayLength, delS, [] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **&IdxM, rbtNode<K,D> **IdxE, rbtNode<K,D> **delS, rbtNode<K,D> **delE, rbtNode<K,D> **&remS) -> void {
			// Cycles through the entire array of accessed nodes
		while(IdxS != IdxE && delS != delE)
				// Checks if both considered nodes have equal keys and data
				// If so, delete it and move forward
				// If not, skip/save it, then move forward in one of the arrays
			if ((*IdxS)->key == (*delS)->key) {
				*remS++ = *IdxS++; delS++;
			} else if ((*IdxS)->key < (*delS)->key)
				*IdxM++ = *IdxS++;
			else
				delS++;
    }, false);
}

/*
 * function_identifier: Deletes multiple nodes, from the red-black tree, which have the same keys provided
 * parameters: 			The length of the arrays and an array of keys
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_repDeleteKey(size_t arrayLength, K *keysS) {
	repDeleteBase(arrayLength, keysS, [] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **&IdxM, rbtNode<K,D> **IdxE, K *keysS, K *keysE) -> void {
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
    }, false);
}

/*		   DATA			*/

/*
 * function_identifier: Deletes multiple nodes, from the red-black tree, which have the same data provided
 * parameters: 			The length of the arrays and an array of nodes with data
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_repDeleteData(size_t arrayLength, rbtNode<K,D> **delS) {
	repDeleteBase(arrayLength, delS, [] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **&IdxM, rbtNode<K,D> **IdxE, rbtNode<K,D> **delS, rbtNode<K,D> **delE, rbtNode<K,D> **&remS) -> void {
			// Cycles through the entire array of accessed nodes
		while(IdxS != IdxE && delS != delE)
				// Checks if both considered nodes have equal keys and data
				// If so, delete it and move forward
				// If not, skip/save it, then move forward in one of the arrays
			if ((*IdxS)->data == (*delS)->data) {
				*remS++ = *IdxS++; delS++;
			} else if ((*IdxS)->data < (*delS)->data)
				*IdxM++ = *IdxS++;
			else
				delS++;
    }, true);
}

/*
 * function_identifier: Deletes multiple nodes, from the red-black tree, which have the same data provided
 * parameters: 			The length of the arrays and an array of data
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_repDeleteData(size_t arrayLength, D *dataS) {
	repDeleteBase(arrayLength, dataS, [] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **&IdxM, rbtNode<K,D> **IdxE, D *dataS, D *dataE) -> void {
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
    }, false);
}

/*	============================================================================  */
/* |                                                                            | */
/* |                                  REMOVE                                    | */
/* |                                                                            | */
/*	============================================================================  */

/*		   BASE			*/

/*
 * function_identifier: The base template for remove functions (node array)
 * parameters: 			An the array length, an array of nodes, a function to extract all key and data values
 *						from given node values into an array, a function to select which nodes to delete, and
 *						a boolean to check if this call is in data context
 * return value:		N/A
*/
template <typename K, typename D>
template <typename F1, typename F2, typename T>
inline void rbTree<K,D>::removeBase(size_t arrayLength, rbtNode<K,D> **delS, T *valueS, F1 foo, F2 bar, bool data) {
		// Ignores the trival case
	if (!size) {
		delete [] valueS;
		return;
	}

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE, **delE;
	T *valueE;

		// Gets the needed nodes and indexes
	IdxM = IdxS = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;
	delE = delS + arrayLength;

		// Sorts the provided and accessed nodes
	if (data) {
		fooPtr<K,D> = dataCompair;
		mergeSortCallerBase(rbtNodes, 0, size - 1);
	} else
		fooPtr<K,D> = keyDataCompair;
	mergeSortCallerBase(delS, 0, arrayLength - 1);

		// Extracts all key and data values into a new array
	valueE = valueS + arrayLength;
	foo(delS, delE, valueS);

		// Selects and removes which nodes to later delete
	bar(IdxS, IdxM, IdxE, valueS, valueE);

		// Clears pair array
	delete [] valueS;

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
			// If there are remaining nodes, sort them (if needed) and create a tree from the result
		if (data) {
			fooPtr<K,D> = keyDataCompair;
			mergeSortCallerBase(rbtNodes, 0, size - 1);
		}
		treeifyShallow(0, size - 1, rbtNodes, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Clears accessed node array
	delete [] rbtNodes;
}

/*
 * function_identifier: The base template for remove functions (raw value array(s))
 * parameters: 			An the array length, an array of values, a function to select which nodes
 *						to delete, and a boolean to check if this call is in data context
 * return value:		N/A
*/
template <typename K, typename D>
template <typename F, typename T>
inline void rbTree<K,D>::removeRawBase(size_t arrayLength, T *valuesS, F foo, bool data) {
		// Ignores the trival case
	if (!size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE;
	T *valuesE;

		// Gets the needed nodes and indexes
	IdxM = IdxS = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;
	valuesE = valuesS + arrayLength;

		// Sorts the accessed nodes and values
	if (data) {
		fooPtr<K,D> = dataCompair;
		mergeSortCallerBase(rbtNodes, 0, size - 1);
	} else
		fooPtr<K,D> = keyDataCompair;
	quickSort(valuesS, valuesE - 1);

	foo(IdxS, IdxM, IdxE, valuesS, valuesE);

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
			// If there are remaining nodes, sort them (if needed) and create a tree from the result
		if (data) {
			fooPtr<K,D> = keyDataCompair;
			mergeSortCallerBase(rbtNodes, 0, size - 1);
		}
		treeifyShallow(0, size - 1, rbtNodes, &root);
		root->parent = NULL;
	} else
			// If no remaining nodes, set root to NULL
		root = NULL;

		// Clears accessed node array
	delete [] rbtNodes;
}

/*
 * function_identifier: The base template for remove functions (single raw value)
 * parameters: 			A value, a function to delete and reorder the nodes
 * return value:		N/A
*/
template <typename K, typename D>
template <typename F, typename T>
inline void rbTree<K,D>::removeBase(T value, F foo) {
		// Ignores the trival case
	if (!size)
		return;

		// Initializes the needed values
	rbtNode<K,D> **rbtNodes, **IdxS, **IdxM, **IdxE;

		// Gets the needed nodes and indexes
	IdxS = IdxM = rbtNodes = rbt_getAllNodes();
	IdxE = rbtNodes + size;

	foo(IdxS, IdxM, IdxE, value);

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
	removeBase(arrayLength, delS, new rbt__pair__<K,D>[arrayLength],
	[] (rbtNode<K,D> **delS, rbtNode<K,D> **delE, rbt__pair__<K,D> *infoS) -> void {
			// Saves the key and data values from the provided arrays
			// This ensures we are able to sort them while preserving their relative order
		while(delS != delE) {
			(*infoS).key = (*delS)->key;
			(*infoS++).data = (*delS++)->data;
		}
	},
	[] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **&IdxM, rbtNode<K,D> **IdxE, rbt__pair__<K,D> *infoS, rbt__pair__<K,D> *infoE) -> void {
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
    }, false);
}

/*
 * function_identifier: Deletes all nodes, in the red-black tree, which have the data and key provided
 * parameters: 			A key and data value
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_remove(K key, D data) {
	rbt__pair__<K,D> info;
	info.key = key; info.data = data;

	removeBase(info, [] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **&IdxM, rbtNode<K,D> **IdxE, rbt__pair__<K,D> info) -> void {
			// Cycles through the entire array of accessed nodes and data
		while(IdxS != IdxE)
				// Checks if the considered node has the requested key and data pair
				// If so, delete it and all similar nodes
				// If not, skip/save it, then move forward in one of the arrays
			if (((*IdxS)->key == info.key) && ((*IdxS)->data == info.data))
				delete *IdxS++;
			else
				*IdxM++ = *IdxS++;
	});
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

		// Zip the given value array
	rbt__pair__<K,D> *infoS = rbt__zip__(arrayLength, keysS, dataS);

		// Calls the main base function
	removeRawBase(arrayLength, infoS,
	[] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **&IdxM, rbtNode<K,D> **IdxE, rbt__pair__<K,D> *infoS, rbt__pair__<K,D> *infoE) -> void {
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
	}, false);

		// Clears pair array
	delete [] infoS;
}

/*		 KEY 		*/

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
 * function_identifier: Deletes all nodes, in the red-black tree, which have the keys in the nodes provided
 * parameters: 			An array length and an array of nodes
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_removeKey(size_t arrayLength, rbtNode<K,D> **delS) {
	removeBase(arrayLength, delS, new K[arrayLength],
	[] (rbtNode<K,D> **delS, rbtNode<K,D> **delE, K *keyS) -> void {
			// Saves the key values from the provided arrays
			// This ensures we don't access deleted memory by accident
		while(delE != delS)
			*keyS++ = (*delS++)->key;
	},
	[] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **&IdxM, rbtNode<K,D> **IdxE, K *keyS, K *keyE) -> void {
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
    }, false);
}

/*
 * function_identifier: Deletes all nodes, in the red-black tree, which have the key provided
 * parameters: 			A key
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_removeKey(K key) {
	removeBase(key, [] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **&IdxM, rbtNode<K,D> **IdxE, K key) -> void {
			// Cycles through the entire array of accessed nodes and data
		while(IdxS != IdxE)
				// Checks if the node considered had the requested key
				// If so, delete it and all similar nodes
				// If not, skip/save it, then move forward in one of the arrays
			if ((*IdxS)->key == key)
				delete *IdxS++;
			else
				*IdxM++ = *IdxS++;
	});
}

/*
 * function_identifier: Deletes all nodes, in the red-black tree, which have the keys provided
 * parameters: 			An array length and an array of keys
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_removeKey(size_t arrayLength, K *keysS) {
	removeRawBase(arrayLength, keysS,
	[] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **&IdxM, rbtNode<K,D> **IdxE, K *keysS, K *keysE) -> void {
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
	}, false);
}

/*		   DATA			*/

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
 * function_identifier: Deletes all nodes, in the red-black tree, which have the data in the nodes provided
 * parameters: 			The array length and an array of nodes
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_removeData(size_t arrayLength, rbtNode<K,D> **delS) {
	removeBase(arrayLength, delS, new D[arrayLength],
	[] (rbtNode<K,D> **delS, rbtNode<K,D> **delE, D *dataS) -> void {
			// Saves the data values from the provided arrays
			// This ensures we don't access deleted memory by accident
		while(delE != delS)
			*dataS++ = (*delS++)->data;
	}, [] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **&IdxM, rbtNode<K,D> **IdxE, D *dataS, D *dataE) -> void {
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
    }, true);
}

/*
 * function_identifier: Deletes all nodes, in the red-black tree, which have the data provided
 * parameters: 			A data value
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_removeData(D data) {
	removeBase(data, [] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **&IdxM, rbtNode<K,D> **IdxE, D data) -> void {
			// Cycles through the entire array of accessed nodes and data
		while(IdxS != IdxE)
				// Checks if the node considered had the requested data
				// If so, delete it and all similar nodes
				// If not, skip/save it, then move forward in one of the arrays
			if ((*IdxS)->data == data)
				delete *IdxS++;
			else
				*IdxM++ = *IdxS++;
	});
}

/*
 * function_identifier: Deletes all nodes, in the red-black tree, which have the data values provided
 * parameters: 			The array length and an array of data values
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_removeData(size_t arrayLength, D *dataS) {
	removeRawBase(arrayLength, dataS,
	[] (rbtNode<K,D> **&IdxS, rbtNode<K,D> **&IdxM, rbtNode<K,D> **IdxE, D *dataS, D *dataE) -> void {
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
	}, true);
}

/*
 * function_identifier: The base template for removeDuplicates functions 
 * parameters: 			A function to delete and reorder the nodes
 * return value:		N/A
*/
template <typename K, typename D>
template <typename F>
inline void rbTree<K,D>::removeDuplicatesBase(F foo) {
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

/*	============================================================================  */
/* |                                                                            | */
/* |                                  OTHER                                     | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: Deletes a single nodes, from the red-black tree, which have the same key and data provided
 * parameters: 			A key and data
 * return value:		N/A
*/
template <typename K, typename D>
D rbTree<K,D>::rbt_delete(K key, D data) {
	return rbt_delete(rbt_search(key, data));
}

/*
 * function_identifier: Deletes a single nodes, from the red-black tree, which have the same key provided
 * parameters: 			A key and data
 * return value:		N/A
*/
template <typename K, typename D>
D rbTree<K,D>::rbt_deleteKey(K key) {
	return rbt_delete(rbt_searchKey(key));
}

/*
 * function_identifier: Deletes a single nodes, from the red-black tree, which have the same data provided
 * parameters: 			A key and data
 * return value:		N/A
*/
template <typename K, typename D>
D rbTree<K,D>::rbt_deleteData(D data) {
	return rbt_delete(rbt_searchData(data));
}

#endif /* _rbt_INSERTDELETE */