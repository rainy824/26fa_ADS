#ifndef AVL_H
#define AVL_H

typedef struct AVLNode* AVLTree;

AVLTree avl_create(void);
AVLTree avl_insert(AVLTree T, int key);
AVLTree avl_delete(AVLTree T, int key);
void avl_destroy(AVLTree T);
void avl_inorder(AVLTree T);


#endif