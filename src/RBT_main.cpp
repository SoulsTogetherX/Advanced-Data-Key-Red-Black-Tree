#ifndef _rbt_MAIN
#define _rbt_MAIN
#include	"RBT_main.h"

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

/*	============================================================================  */
/* |                                                                            | */
/* |                              NODE FUNCTIONS                                | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: The constructor for nodes
 * parameters: 			A key and data values
 * return value:		N/A
*/
template <typename K, typename D>
rbtNode<K,D>::rbtNode(K Key, D Data) {
		// Initialize basic values for the node
	parent = left = right = NULL;
	descendants = 0;

		// Copy over basic values for the node
	key = Key;
	data = Data;
}

/*
 * function_identifier: The copy constructor for nodes
 * parameters: 			A node
 * return value:		N/A
*/
template <typename K, typename D>
rbtNode<K,D>::rbtNode(const rbtNode<K,D> *newNode) {
		// Initialize basic values for the node
	parent = left = right = NULL;
	descendants = 0;

		// Copy over basic values for the node
	data = newNode->data;
	key = newNode->key;
	color = newNode->color;
}

/*
 * function_identifier: Returns the data in a node
 * parameters: 			N/A
 * return value:		A data value
*/
template <typename K, typename D>
D rbtNode<K,D>::rbt_getData() const {
	return data;
}

/*
 * function_identifier: Returns the key in a node
 * parameters: 			N/A
 * return value:		A key
*/
template <typename K, typename D>
K rbtNode<K,D>::rbt_getKey() const {
	return key;
}

/*
 * function_identifier: Checks if two nodes have the same keys and data values
 * parameters: 			A node
 * return value:		A boolean value if the equality expression is true or not
*/
template <typename K, typename D>
bool rbtNode<K,D>::equal(const rbtNode<K,D> *node) {
	return ((key == node->key) && (data == node->data));
}

/*	============================================================================  */
/* |                                                                            | */
/* |                         RED-BLACK TREE FUNCTIONS                           | */
/* |                                                                            | */
/*	============================================================================  */

/*
 * function_identifier: Performs a left rotation in the red-black tree
 * parameters: 			The pivot node
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::leftRotate(rbtNode<K,D> *pivot) {
		// Return if invaild pivot given
	if(!pivot || !pivot->right) return;

		// Initializes the needed values
	rbtNode<K,D> *detach;

		// Detaches right child of pivot and attaches the right-left grandchild of pivot in its place
		// The detached right child is stored in node detach for later use
	detach = pivot->right;
	pivot->right = detach->left;
	if(detach->left) detach->left->parent = pivot; // Used to change the parent of pivot's right-left grandchild to be pivot

		// Assignes the detach child's parent to be the same as pivot
	detach->parent = pivot->parent;

		// If pivot was the root node, then make detach the new root node
	if(!detach->parent) root = detach;
		// Else, put detach in pivot's position
	else if(pivot == pivot->parent->right) pivot->parent->right = detach;
	else pivot->parent->left = detach;

		// Assigns pivot as a child of detach
	detach->left = pivot;
	pivot->parent = detach;

		// Corrects the descendants of each node
	pivot->descendants -= detach->descendants+1;
	if(pivot->right)
		pivot->descendants += pivot->right->descendants + 1;

	detach->descendants = pivot->descendants + 1;
	if(detach->right)
		detach->descendants += detach->right->descendants + 1;
}

/*
 * function_identifier: Performs a right rotation in the red-black tree
 * parameters: 			The pivot node
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rightRotate(rbtNode<K,D> *pivot) {
		// Return if invaild pivot given
	if(!pivot || !pivot->left) return;

		// Initializes the needed values
	rbtNode<K,D> *detach;

		// Detaches left child of pivot and attaches the left-right grandchild of pivot in its place
		// The detached left child is stored in node detach for later use
	detach = pivot->left;
	pivot->left = detach->right;
	if(detach->right) detach->right->parent = pivot; // Used to change the parent of pivot's left-right grandchild to be pivot

		// Assignes the detach child's parent to be the same as pivot
	detach->parent = pivot->parent;

		// If pivot was the root node, then make detach the new root node
	if(!detach->parent) root = detach;
		// Else, put detach in pivot's position
	else if(pivot == pivot->parent->left) pivot->parent->left = detach;
	else pivot->parent->right = detach;

		// Assigns pivot as a child of detach
	detach->right = pivot;
	pivot->parent = detach;

		// Corrects the descendants of each node
	detach->descendants++;
	pivot->descendants -= detach->descendants;
	if(pivot->left)
		pivot->descendants += pivot->left->descendants + 1;

	detach->descendants = pivot->descendants + 1;
	if(detach->left)
		detach->descendants += detach->left->descendants + 1;
}

/*
 * function_identifier: Deletes a node, and all its descendants, without caring about red-black tree rules (used in deleteTree function)
 * parameters: 			The root node of the branch to delete
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::deleteBranch(rbtNode<K,D> *curr) {
		// Recursively delete descendants
	if(curr->left) deleteBranch(curr->left);
	if(curr->right) deleteBranch(curr->right);

		// Frees the current root node
	delete curr;
}

/*
 * function_identifier: A recursive function used to copy a red-black tree
 * parameters: 			A node in the original red-black tree, and a position in the new red-black tree
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::copyTree(rbtNode<K,D> *curr, rbtNode<K,D> **currNew){
		// Ignores the trival case
	if(!curr) {
		*currNew = NULL;
		return;
	}

		// Allocates a new node for the copy tree
	*currNew = new rbtNode<K,D>(curr);
	(*currNew)->color = curr->color;
	(*currNew)->descendants = curr->descendants;

	// Sets the children node's parents appropriately
		// Sets the left child
	copyTree(curr->left, &(*currNew)->left);
	if((*currNew)->left)
			// If there is a left child, set its parent
		(*currNew)->left->parent = *currNew;

		// Sets the right child
	copyTree(curr->right, &(*currNew)->right);
	if((*currNew)->right)
			// If there is a right child, set its parent
		(*currNew)->right->parent = *currNew;
}

/*
 * function_identifier: Frees all memory and pointers used in the tree
 * parameters: 			N/A
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_free() {
		// Ignores the trival case
	if(!root) return;

		// Deletes all of the root's descendants
	if(root->right) deleteBranch(root->right);
	if(root->left) deleteBranch(root->left);

	delete root;
}

/*
 * function_identifier: Returns the root node of the red-black tree
 * parameters: 			N/A
 * return value:		A node
*/
template <typename K, typename D>
rbtNode<K,D> *rbTree<K,D>::rbt_getRoot() const {
	return root;
}

/*
 * function_identifier: Checks if the red-black tree is empty
 * parameters: 			N/A
 * return value:		Boolean with value depending if the tree if empty or not
*/
template <typename K, typename D>
bool rbTree<K,D>::rbt_isEmpty() const {
	return size == 0;
}

/*
 * function_identifier: Returns red-black tree's size
 * parameters: 			N/A
 * return value:		the red-black tree's size
*/
template <typename K, typename D>
size_t rbTree<K,D>::rbt_getSize() const {
	return size;
}

/*
 * function_identifier: Allows a user to insert new data, with given key, into a red-black tree
 * parameters: 			The key and data to insert
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_insert(K key, D data) {
		// Checks for node overflow while increasing size
	if (++size == 0)
		throw std::overflow_error ("overflow - too many inserted into the red black tree"); 

		// Allocates a new node
	rbtNode<K,D> *opNode = new rbtNode<K,D>(key, data);
	opNode->color = true;

		// If there is no root node, use the node we just created and finish
	if(!root) {
		root = opNode;
		opNode->parent = 0;
	}
	else {
			// Initializes the needed values
		rbtNode<K,D> *temp = 0;
		rbtNode<K,D> *curr = root;

			// Finds an empty position that opNode can be put in
		while(1) {
				// Adding a child to this node
			curr->descendants++;
				// Sets temp to curr, before storing a child in curr
			temp = curr;

			if((opNode->key < curr->key) || ((opNode->key == curr->key) && (opNode->data < curr->data))){
					// If opNode < curr, opNode should be somewhere at the left branch of curr
					// temp is now curr's parent
				curr = curr->left;
					// If curr position is empty, fill it with opNode and finish
				if(!curr) {
					temp->left = opNode;
					break;
				}
			}
			else {
					// If opNode > curr, opNode should be somewhere at the right branch of curr
					// temp is now curr's parent
				curr = curr->right;
					// If curr position is empty, fill it with opNode and finish
				if(!curr) {
					temp->right = opNode;
					break;
				}
			}
		}

			// Sets opNode's parent appropriately
		opNode->parent = temp;

		// Insertion finished
		// Now begins to rebalance the tree in case the insertion messed something up

			// Loops until opNode's parent isn't red
		while(opNode != root && opNode->parent != root && opNode->parent->color == true) {
				// Sets temp to be opNode's uncle
				// If left grandparent's child is opNode's parent, right grandparent's child is its uncle
			if(opNode->parent->parent->left == opNode->parent) temp = opNode->parent->parent->right;
				// If right grandparent's child is opNode's parent, left grandparent's child is its uncle
			else temp = opNode->parent->parent->left;

				// If uncle is black, check if the opNode node fits in one of 4 cases (LL, RR, LR, RL)
			if(!temp || temp->color == false) {
				if(opNode == opNode->parent->left) {
					// opNode is left child

						// LL case
						// Swap the colors of opNode's parent and grandparent, then right rotate on opNode's grandparent
					if(opNode->parent == opNode->parent->parent->left) {
							// Swap Colors
						bool color = opNode->parent->color;
						opNode->parent->color = opNode->parent->parent->color;
						opNode->parent->parent->color = color;

							// Right Rotate grandparent
						rightRotate(opNode->parent->parent);
					}

						// RL case
						// Swaps the color of opNode and its grandparent, right rotates opNode's parent, and finally left rotate on opNode's grandparent
					else {
							// Swap Colors
						bool color = opNode->color;
						opNode->color = opNode->parent->parent->color;
						opNode->parent->parent->color = color;

							// Right Rotate Parent
						rightRotate(opNode->parent);

							// Left Rotate grandparent
						leftRotate(opNode->parent->parent);
					}
					
				} else {
					// opNode is right child

						// RR case
						// Swap the colors of opNode's parent and grandparent, then left rotate on opNode's grandparent
					if(opNode->parent == opNode->parent->parent->right) {
							// Swap Colors
						bool color = opNode->parent->color;
						opNode->parent->color = opNode->parent->parent->color;
						opNode->parent->parent->color = color;

							// Left Rotate grandparent
						leftRotate(opNode->parent->parent);
					}

						// LR case
						// Swaps the color of opNode and its grandparent, left rotates opNode's parent, and finally right rotate on opNode's grandparent
					else {
							// Swap Colors
						bool color = opNode->color;
						opNode->color = opNode->parent->parent->color;
						opNode->parent->parent->color = color;

							// Left Rotate Parent
						leftRotate(opNode->parent);

							// Right Rotate grandparent
						rightRotate(opNode->parent->parent);
					}
				}
			}
				// If uncle is red, color it and its sibling black, but it's parent red
				// Afterwards, begin rebalancing the tree at opNode's grandparent's positon now
			else {
				opNode = opNode->parent->parent;
				opNode->color = true;
				opNode->left->color = false;
				opNode->right->color = false;
			}
		}
	}

		// Ensures that the root node is black
	root->color = false;
}

/*
 * function_identifier: Allows a user to delete a node from a red-black tree
 * parameters: 			The node, in the tree, to delete
 * return value:		The old data in the deleted node
*/

template <typename K, typename D>
D rbTree<K,D>::rbt_delete(rbtNode<K,D> *opNode) {
		// Returns default if given invaild node
	if(!opNode)
		return D();

		// Initializes the needed values
	rbtNode<K,D> *curr;

	if(opNode->right && opNode->left) {
			// This node has two children

			// Find the node, with the next smallest key in the red-black tree
		curr = opNode->right;
		while(curr->left) curr = curr->left;

			// Exchange data and key with the found node
		opNode->key = curr->key;
		D tempData = opNode->data;
		opNode->data = curr->data;
		curr->data = tempData;

			// Now, act to delete this found node instead
		opNode = curr;
	}

		// Copies the opNode's data to be later returned
	D ret = opNode->data;
	
		// Decreases the rbt's node count
	size--;

		// Corrects the descendants of the parent nodes
	curr = opNode;
	while(curr) {
		curr->descendants--;
		curr = curr->parent;
	}
	
		// Finds the child node, of the node to be deleted
		// Save this child for later
	if(opNode->left) curr = opNode->left;
	else curr = opNode->right;

		// Checks if the node to be deleted is the root node
	if(root == opNode) {
			// If so, replace the root node with the child found earlier
		root = curr;
			// Color the new root node black and assign a NULL parent
		if(curr) {
			curr->parent = NULL;
			curr->color = false;
		}
			// Frees the node scheduled to be deleted
		delete opNode;
		return ret; // Finish fixup
	}
		// Else, the node to be deleted is not the root
	else {
			// Place the child found earlier in the position the deleted node was in before
		if(curr) curr->parent = opNode->parent;
		if(opNode == opNode->parent->left) opNode->parent->left = curr;
		else opNode->parent->right = curr;
	}

	// Node deleted
	// Now begins to rebalance the tree in case the deletion messed something up

		// Checks simple cases
			// Checks if the opNode node was red and if there is no child
			// If so, no fixup required
	if(opNode->color == true && !curr) {delete opNode; return ret;}

		// Initializes needed variable
	rbtNode<K,D> *parent = opNode->parent;

	delete opNode; // Frees the deleted node

		// Checks if the replacement is red
		// If so, set it to black and finish
	if(curr && curr->color == true) {curr->color = false; return ret;}

	// At this point, the replacement node is now double black
	// We will now promote it

		// Fixup loop
	while(curr != root) {
			// Checks if promoted node is left child
		if(curr == parent->right) {
				// If so, then sibling is left child
			opNode = parent->left;
				// Check if sibling is not NULL
			if(opNode) {
					// Checks if sibling is red and left child
				if(opNode->color == true) {
						// Colors the appropriate nodes
					parent->color = true;
					opNode->color = false;

						// Right Rotation of parent
					rightRotate(parent);
					continue; // Go back to start of loop
				}
					// Checks if sibling's left child is red
					// If so, LL case: sibling is black and left child and sibling's left child is red
				else if(opNode->left && opNode->left->color == true) {
						// Right Rotation of parent
					rightRotate(parent);
				}
					// Checks if sibling's right child is red
					// If so, LR case: sibling is black and left child and sibling's right child is red
				else if(opNode->right && opNode->right->color == true) {
						// Left Rotation of sibling
					leftRotate(opNode);
						// Right Rotation of parent
					rightRotate(parent);
				}
					// Else, sibling is black and has two black children
				else goto blackChildren;

					// Else, sibling is black and has at least one red child
					// Moves down the blackness
				parent->parent->color = parent->color;
				parent = parent->parent;
				parent->left->color = false;
				if(parent->right) parent->right->color = false;
				if(curr) curr->color = false;

				break; // Finish fixup
			}
		}
			// Else promoted node is right child
		else {
				// Thus, sibling is right child
			opNode = parent->right;
				// Check if sibling is not NULL
			if(opNode) {
					// Checks if sibling is red and right child
				if(opNode->color == true) {
						// Colors the appropriate nodes
					parent->color = true;
					opNode->color = false;

						// Left Rotation of parent
					leftRotate(parent);
					continue; // Go back to start of loop
				}
					// Checks if sibling's right child is red
					// If so, RR case: sibling is black and right child and sibling's right child is red
				else if(opNode->right && opNode->right->color == true) {
						// Left Rotation of parent
					leftRotate(parent);
				}
					// Checks if sibling's left child is red
					// If so, RL case: sibling is black and right child and sibling's left child is red
				else if(opNode->left && opNode->left->color == true) {
						// Right Rotation of sibling
					rightRotate(opNode);
						// Left Rotation of parent
					leftRotate(parent);
				}
					// Else, sibling is black and has two black children
				else goto blackChildren;

					// Else, sibling is black and has at least one red child
					// Moves down the blackness
				parent->parent->color = parent->color;
				parent = parent->parent; // Moves up the parent
				if(parent->left) parent->left->color = false;
				parent->right->color = false;
				if(curr) curr->color = false; // Sets the promoted node to be black
				break; // Finish fixup
			}
		}
blackChildren:

			// Else, sibling is either NULL or black and has two black children
			// Moves up the blackness
		if(opNode) opNode->color = true;
		if(curr) curr->color = false;
			// If parent was already black, it is now the promoted node
		if(parent != root && parent->color == false) {
			curr = parent;
			parent = curr->parent;
			continue; // Go back to start of loop with new promoted node
		}
		parent->color = false;
		break; // Finish fixup
	}

		// Ensures that the root node is black
	root->color = false;
	return ret;
}

/*
 * function_identifier: Deletes the root node from a red-black tree
 * parameters: 			N/A
 * return value:		The data from the old root
*/

template <typename K, typename D>
D rbTree<K,D>::rbt_pop() {
	return rbt_delete(root);
}

/*
 * function_identifier: The basic assignment operation for red-black trees
 * parameters: 			A red-black tree
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::operator=(const rbTree<K,D> &rbt) {
		// Ignores the trival case
	if (rbt.root == root)
		return;

		// Deletes all nodes
	rbt_free();

		// Deep copies over the tree
	size = rbt.size;
	copyTree(rbt.root, &(this->root));
}

/*
 * function_identifier: A function used to deep copy a red-black tree. An alternative to the basic assignment operation
 * parameters: 			N/A
 * return value:		A red-black tree deep copy
*/
template <typename K, typename D>
rbTree<K,D> rbTree<K,D>::rbt_copy() const {
	return rbTree(*this);
}

/*
 * function_identifier: Reinitializes the entire tree (deletes all nodes and sets size to 0)
 * parameters: 			N/A
 * return value:		N/A
*/
template <typename K, typename D>
void rbTree<K,D>::rbt_clear() {
	rbt_free(); // Deletes all nodes
	root = NULL; size = 0;
}

/*
 * function_identifier: The constructor for the red-black tree
 * parameters: 			N/A
 * return value:		N/A
*/
template <typename K, typename D>
rbTree<K,D>::rbTree() {
	root = NULL; size = 0;
}

/*
 * function_identifier: The copy constructor for the red-black tree
 * parameters: 			A red-black tree
 * return value:		N/A
*/
template <typename K, typename D>
rbTree<K,D>::rbTree(const rbTree<K,D> &rbt) {
		// Deep copies over the tree
	size = rbt.size;
	copyTree(rbt.root, &(this->root));
}

/*
 * function_identifier: Frees the entire tree and before this object is deleted
 * parameters: 			N/A
 * return value:		N/A
*/
template <typename K, typename D>
rbTree<K,D>::~rbTree() {
		// Deletes the root and all of its descendants
	rbt_free();
}

#endif /* _rbt_MAIN */