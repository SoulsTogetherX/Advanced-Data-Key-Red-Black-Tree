# Advanced-Data-Key-Red-Black-Tree\
A red-black semi-balanced binary search tree. Provides a multitude of useful functions, including O(log(N)) lookup, insert and delete operations.\
\
This is Version 1.02. Any bugs will be patched the moment found. Still working on more additions.\
\
# Modules Available\
RBT_main		- Includes all the basic-most functions for any red-black tree to run\
RBT_string		- Provides multiple ways to convert an tree into a printable string\
RBT_traversal	- Provides multiple ways to access all the raw nodes in a red-black tree\
RBT_access.cpp	- Aids in the accessing of desired nodes in the tree\
RBT_indel		- Includes functions to more effectively insert or delete values from the tree\
RBT_limit		- Limits a tree's values in a certain desire way. More advanced deletion operations\
RBT_ops			- Includes basic operations such as addition, subtraction, multiplication, and divisition\
RBT_setops		- Includes more set-like operations. Uninon, intersection, xor, etc.\
RBT_sort		- User friendly module used to sort a list of nodes\
RBT_value		- Allows the manipulation of key and data values while they are still inside a given tree\
\
# Lastest Bug Fixes or Additions\
Added Functions:\
	rbt_repInsertFunc()\
	rbt_repInsertFor()\
	rbt_clamp() for single and multiple data and key ranges\
	rbt_exclude() for single and multiple data and key ranges\
\
Changed:
	Condensed code for all rbt_repDelete()\
	Condensed code for all rbt_remove()\
	Fixed a bug in rbt_repInsert (the last node was not being included in the tree properly)\
	Split rbt_distribute() into rbt_distribute() and rbt_distribute_shallow() functions\
	Fixed all the 'rbt_funcAll' functions, in 'rbt_value' work properly\
	Reworked all function in 'rbt_value'\
	Function 'rbt_clampData' is now atcually clamping by data instead of key\
	Added newlines to the 'README.md' file\
