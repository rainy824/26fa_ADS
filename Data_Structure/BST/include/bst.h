#ifndef BST_H
#define BST_H

typedef struct BSTNode* BSTree;

BSTree bst_create(void);
BSTree bst_insert(BSTree root, int key);
BSTree bst_delete(BSTree root, int key);
void bst_destroy(BSTree root);
void bst_inorder(BSTree root);

#endif
