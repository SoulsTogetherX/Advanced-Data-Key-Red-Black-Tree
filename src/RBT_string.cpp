#ifndef _rbt_STRING
#define _rbt_STRING
#include	"RBT_main.h"
#include	"RBT_string.h"

	/*	 GLOBAL VARIABLES	*/
inline std::ostringstream traverseStr;

/*	============================================================================  */
/* |                                                                            | */
/* |                     	       TRAVERSALS                                   | */
/* |                                                                            | */
/*	============================================================================  */

	/* TEST ORDER */

/*
 * function_identifier: Prints out the structure of the given red-black tree (called by rbt_string())
 * parameters: 			A node pointer to process through a preorder traversal fashion
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::testOrder(rbtNode<K,D> *curr) const {
		// Prints curr's color
	if(curr->color) traverseStr << "R ";
	else traverseStr << "B ";

		// Check if current node has a parent
			// Convert what is found
	traverseStr << "key: " << curr->key;
	if(curr->parent)
		traverseStr << ",\twith parent " << curr->parent->key;
	else
		traverseStr << ",\twith no parent";

		// Check if current node has children
			// Convert what is found
	traverseStr << ",  \twith children";
	if(curr->left) {
		traverseStr << ' ' << curr->left->key;
	} else traverseStr << " nil";
	if(curr->right) {
		traverseStr << ' ' << curr->right->key;
	} else traverseStr << " nil";

		// Transcribe number of descendants
	traverseStr << ", \twith " << curr->descendants << " descendants";

		// Prints data and key values
	traverseStr << ", \tand data " << curr->data << std::endl;


		// Performs preorder traversal
	if(curr->left) testOrder(curr->left);
	if(curr->right) testOrder(curr->right);
}

	/* PRE ORDER */

/*
 * function_identifier: Prints the red-black-tree in an preorder traversal fashion (called by rbt_string())
 * parameters: 			A node pointer to process through a preorder traversal fashion
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::preOrder(rbtNode<K,D> *curr) const {
		// Performs preorder traversal
	traverseStr << curr->data << ' ';
	if(curr->left) preOrder(curr->left);
	if(curr->right) preOrder(curr->right);
}

	/* IN ORDER */

/*
 * function_identifier: Prints the red-black-tree in an inorder traversal fashion (called by rbt_string())
 * parameters: 			A node pointer to process through a inorder traversal fashion
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::inOrder(rbtNode<K,D> *curr) const {
		// Performs inorder traversal
	if(curr->left) inOrder(curr->left);
	traverseStr << curr->data << ' ';
	if(curr->right) inOrder(curr->right);
}

	/* POST ORDER */

/*
 * function_identifier: Prints the red-black-tree in an postorder traversal fashion (called by rbt_string())
 * parameters: 			A node pointer to process through a postorder traversal fashion
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::postOrder(rbtNode<K,D> *curr) const {
		// Performs postorder traversal
	if(curr->left) postOrder(curr->left);
	if(curr->right) postOrder(curr->right);
	traverseStr << curr->data << ' ';
}

	/* KEY AND DATA */

/*
 * function_identifier: Prints the keys and data values of the red-black-tree in an inorder traversal fashion (called by rbt_string())
 * parameters: 			A node pointer to process through a inorder traversal fashion
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::keyAndData(rbtNode<K,D> *curr) const {
		// Performs inorder traversal
	if(curr->left) keyAndData(curr->left);
	traverseStr << curr->key << " : " << curr->data << '\n';
	if(curr->right) keyAndData(curr->right);
}

	/* LEVEL ORDER */

/*
 * function_identifier: Prints the red-black-tree in an levelorder traversal fashion (called by rbt_string())
 * parameters: 			N/A
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::levelOrder() const {
		// Initializes starting values
	rbtNode<K,D> *nodes[size], **startPtr = nodes, **endPtr = nodes + 1;

		// Places the node as first value in queue
	*startPtr = root;

		// Loops through all nodes in queue
	while(startPtr != endPtr) {
			// Prints the current node out
		traverseStr << (*startPtr)->data << ' ';

			// If left child of current node exists, place it on queue
		if((*startPtr)->left)
			*endPtr++ = (*startPtr)->left;

			// If right child of current node exists, place it on queue
		if((*startPtr)->right)
			*endPtr++ = (*startPtr)->right;

			// Move forward in queue
		startPtr++;
	}
}

	/* PRINT LEVEL */

/*
 * function_identifier: Prints a single level of node values in the red-black tree (called by rbt_string())
 * parameters: 			A root node pointer and the level to print
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::printLevel(rbtNode<K,D> *curr, size_t level) const {
		// Returns if no more levels to explore
	if(curr == NULL) return;

	if(level == 0)
			// Prints out node if we are at the requested level
		traverseStr << curr->data << ' ';
	else {
			// Performs perorder traversal (node that left is first visted branch)
		printLevel(curr->left, level-1);
		printLevel(curr->right, level-1);
	}
}

	/* PRINT CONTROLER */

/*
 * function_identifier: User's interface to the traversal functions
 * parameters: 			The traversal type and an optional level operator (used only for PRINT_LEVEL)
 * return value:		N/A
*/
template <typename K, typename D>
std::string rbTree<K,D>::rbt_string(enum rbtraversal traversal, size_t level) const {
		// Returns empty string if red-black tree is empty
	if (!root)
		return "";

		// Clears string buffer
	traverseStr.str(std::string());
		// Calls the appropriate function depending on the given traversal argument
	switch(traversal) {
		case TEST_ORDER:		testOrder(root); break;
		case PRE_ORDER:			preOrder(root); break;
		case IN_ORDER:			inOrder(root); break;
		case POST_ORDER:		postOrder(root); break;
		case KEY_AND_DATA:		keyAndData(root); break;
		case LEVEL_ORDER:		levelOrder(); break;
		case PRINT_LEVEL:		printLevel(root, level); break;
	}
		// Returns output of string buffer
	return traverseStr.str();
}

#endif /* _rbt_STRING */