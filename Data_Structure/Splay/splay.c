#include "splay.h"
#include <stdio.h>
#include <stdlib.h>

struct SplayNode{
    int key;
    SplayTree parent;
    SplayTree left;
    SplayTree right;
};


static SplayTree create_node(int key){
    SplayTree node = (SplayTree)malloc(sizeof(struct SplayNode));
    if( !node ){
        printf("Out of Space");
        exit(1);
    }
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;

    return node;
}

static SplayTree FindMax(SplayTree T){
    if( !T )return NULL;
    SplayTree p = T;
    while(p->right != NULL){
        p = p->right;
    }
    return p;
}

static SplayTree leftRotate(SplayTree T){
    SplayTree newRoot = T->right;
    SplayTree beta = newRoot->left;

    newRoot->parent = T->parent;
    if(T->parent != NULL){
        if(T == T->parent->left){
            T->parent->left = newRoot;
        }else{
            T->parent->right = newRoot;
        }
    }

    T->parent = newRoot;
    T->right = beta;
    newRoot->left = T;
    if(beta != NULL){
        beta->parent = T;
    }

    return newRoot;
}

static SplayTree rightRotate(SplayTree T){
    SplayTree newRoot = T->left;
    SplayTree beta = newRoot->right;


    newRoot->parent = T->parent;
    if(T->parent != NULL){
        if(T == T->parent->left){
            T->parent->left = newRoot;
        }else{
            T->parent->right = newRoot;
        }
    }

    T->parent = newRoot;
    T->left = beta;
    newRoot->right = T;
    if(beta != NULL){
        beta->parent = T;
    }

    return newRoot;
}

static SplayTree splay(SplayTree node){
    if( !node )return NULL;

    while(node->parent != NULL){
        SplayTree p = node->parent;
        SplayTree g = p->parent;

        //Zig
        if(g == NULL){
            if(p->left == node){
                node = rightRotate(p); 
            }else{
                node = leftRotate(p);
            }
        }else{
            //Zig-zig
            if((node == p->left)&&(p == g->left)){
                p = rightRotate(g);
                node = rightRotate(p);
            }else if((node == p->right)&&(p == g->right)){
                p = leftRotate(g);
                node = leftRotate(p);
            }
            //Zig-zag
            else if((node == p->left)&&(p == g->right)){
                node = rightRotate(p);
                node = leftRotate(g);
            }else{
                node = leftRotate(p);
                node = rightRotate(g);
            }
        }
    }
    return node;
}

SplayTree splay_create(void){
    return NULL;
}

SplayTree splay_insert(SplayTree T, int key){
    if( !T )return create_node(key);

    SplayTree current = T;
    SplayTree parent = NULL;

    while(current != NULL){
        if(key < current->key){
            parent = current;
            current = current->left;
        }else if(key > current->key){
            parent = current;
            current = current->right;
        }else{
            return splay(current);
        }
    }

    SplayTree Newnode = create_node(key);
    if(key > parent->key){
        parent->right = Newnode;
        Newnode->parent = parent;
    }else{
        parent->left = Newnode;
        Newnode->parent = parent;
    }
    

    return splay(Newnode);
}

SplayTree splay_delete(SplayTree T, int key){
    if( !T )return NULL;

    SplayTree p = T;
    SplayTree q = NULL;
    while(p != NULL){
        if(key < p->key){
            q = p;
            p = p->left;
        }else if(key > p->key){
            q = p;
            p = p->right;
        }else{
            break;
        }
    }
    //Not Found
    if(p == NULL){
        return splay(q);
    }

    //Found
    SplayTree OldRoot = splay(p);
    SplayTree NewRoot;

    if(!OldRoot->left && !OldRoot->right){
        free(OldRoot);
        return NULL;
    }else if(!OldRoot->left){
        NewRoot = OldRoot->right;
        NewRoot->parent = NULL;
            free(OldRoot);
    }else if(!OldRoot->right){
        NewRoot = OldRoot->left;
        NewRoot->parent = NULL;
        free(OldRoot);
    }else{
        OldRoot->left->parent = NULL;
        NewRoot = splay(FindMax(OldRoot->left));
        NewRoot->right = OldRoot->right;
        OldRoot->right->parent = NewRoot;
        free(OldRoot);
    }

    return NewRoot;
}

void splay_inorder(SplayTree T){
    if( !T ){
        return ;
    }
    splay_inorder(T->left);
    printf("%d ", T->key);
    splay_inorder(T->right);
    return ;
}

void splay_destroy(SplayTree T){
    if( !T )return ;
    splay_destroy(T->left);
    splay_destroy(T->right);
    free(T);
    return ;
}