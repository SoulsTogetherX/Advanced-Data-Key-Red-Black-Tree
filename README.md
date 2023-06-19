# Advanced-Data-Key-Red-Black-Tree
A red-black semi-balanced binary search tree. Provides a multitude of useful functions, including O(log(N)) lookup, insert and delete operations.

This is Version 1.01. Any bugs will be patched the moment found. Still working on more additions.

# Modules Available
RBT_main		- Includes all the basic-most functions for any red-black tree to run
RBT_string		- Provides multiple ways to convert an tree into a printable string
RBT_traversal	- Provides multiple ways to access all the raw nodes in a red-black tree
RBT_access.cpp	- Aids in the accessing of desired nodes in the tree
RBT_indel		- Includes functions to more effectively insert or delete values from the tree
RBT_limit		- Limits a tree's values in a certain desire way. More advanced deletion operations
RBT_ops			- Includes basic operations such as addition, subtraction, multiplication, and divisition
RBT_setops		- Includes more set-like operations. Uninon, intersection, xor, etc.
RBT_sort		- User friendly module used to sort a list of nodes
RBT_value		- Allows the manipulation of key and data values while they are still inside a given tree

# Lastest Bug Fixes or Additions
Added Functions:
	rbt_invertI()
	rbt_clampData
	rbt_excludeData
	rbt_upperLimitData
	rbt_lowerLimitData
	rbt_deleteKey
	rbt_deleteData
	nodesSpecifiedLevelWithKey
	nodesSpecifiedLevelWithData
	nodesSpecifiedLevelWithDataKey
	rbt_orLeast
	rbt_orLeastI
	rbt_orMost
	rbt_orMostI
	rbt_xorMost
	rbt_xorMostI
	rbt_xorKeyMost
	rbt_xorKeyMostI
	rbt_xorDataMost
	rbt_xorDataMostI
	ostream operator (<<)

Changed:
	rbt_invert()				- Has no affect the tree operated on anymore
	rbt_delete					- Can now accept direct Key/Data pairs
	rbt_getAllNodesWithKey		- Can now work with PRINT_LEVEL
	rbt_getAllNodesWithData		- Can now work with PRINT_LEVEL
	rbt_getAllNodesWithDataKey	- Can now work with PRINT_LEVEL
	rbt_or						- Bug fixed. No longer overwrites values in some edge cases
								- Now merges tree duplicates (when multiple duplicates are found, the resulting tree will have the min number of duplicates). 
	rbt_orI						- Bug fixed. No longer overwrites values in some edge cases
								- Now merges tree duplicates (when multiple duplicates are found, the resulting tree will have the min number of duplicates). 
	rbt_xor						- Reworked
	rbt_xorI					- Reworked
	rbt_xorKey					- Reworked
	rbt_xorKeyI					- Reworked
	rbt_xorData					- Reworked
	rbt_xorDataI				- Reworked
	rbt_string					- Transversal time KEYDATA now creates strings in the format: (K, D) (K, D) (K, D) ... (K, D)


