#ifndef SPLAY_H
#define SPLAY_H

typedef struct SplayNode* SplayTree;

SplayTree splay_create(void);
SplayTree splay_insert(SplayTree T, int key);
SplayTree splay_delete(SplayTree T, int key);
void splay_inorder(SplayTree T);
void splay_destroy(SplayTree T);


#endif