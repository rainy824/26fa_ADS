#include "AVL.h"
#include <stdio.h>
#include <stdlib.h>

struct AVLNode{
    int key;
    int height;
    AVLTree left;
    AVLTree right;
};

static AVLTree create_node(int key){
    AVLTree node = (AVLTree)malloc(sizeof(struct AVLNode));
    if( !node ){
        printf("Out of Space");
        exit(1);
    }
    node->key = key;
    node->height = 0;
    node->left = NULL;
    node->right = NULL;

    return node;
}

static int max(int a, int b){
    return a > b ? a : b;
}

static int getHeight(AVLTree T){
    if( !T ){
        return -1;
    }
    return T->height;
}

static AVLTree leftRotate(AVLTree T){
    AVLTree newRoot = T->right;
    T->right = T->right->left;
    newRoot->left = T;

    T->height = 1 + max(getHeight(T->left), getHeight(T->right));
    newRoot->height = 1 + max(getHeight(newRoot->left), getHeight(newRoot->right));
    
    return newRoot;
} 

static AVLTree rightRotate(AVLTree T){
    AVLTree newRoot = T->left;
    T->left = T->left->right;
    newRoot->right = T;

    T->height = 1 + max(getHeight(T->left), getHeight(T->right));
    newRoot->height = 1 + max(getHeight(newRoot->left), getHeight(newRoot->right));

    return newRoot;
}

static int bf(AVLTree T){
    if( !T ){
        return 0;
    }
    return getHeight(T->left) - getHeight(T->right);
}

static AVLTree FindMin(AVLTree T){
    if( !T ){
        return NULL;
    }
    AVLTree p = T;
    while(p->left != NULL){
        p = p->left;
    }
    return p;
}

AVLTree avl_create(void){
    return NULL;
}

AVLTree avl_insert(AVLTree T, int key){
    if( !T ){
        return create_node(key);
    }
    if(T->key == key){
        printf("repeated!\n");
        exit(1);
    }else if(key < T->key){
        T->left = avl_insert(T->left, key);
    }else{
        T->right = avl_insert(T->right, key);
    }

    T->height = 1 + max(getHeight(T->left), getHeight(T->right));

    if(bf(T) > 1){
        if(bf(T->left) >= 0){
            T = rightRotate(T);
        }else{
            T->left = leftRotate(T->left);
            T = rightRotate(T);
        }
    }else if(bf(T) < -1){
        if(bf(T->right) <= 0){
            T = leftRotate(T);
        }else{
            T->right = rightRotate(T->right);
            T = leftRotate(T);
        }
    }

    return T;
}

AVLTree avl_delete(AVLTree T, int key){
    if( !T ){
        return NULL;
    }

    if(key < T->key){
        T->left = avl_delete(T->left, key);
    }else if(key > T->key){
        T->right = avl_delete(T->right, key);
    }else{
        //delete leave node
        if( !T->left && !T->right){
            free(T);
            return NULL;
        }
        //Only 1 child
        else if( (!T->left) ^ (!T->right) ){
            AVLTree SoleChild = !T->left ? T->right : T->left;
            free(T);
            return SoleChild;
        }
        //Double children
        else{
            AVLTree RightMin = FindMin(T->right);
            T->key = RightMin->key;
            T->right = avl_delete(T->right, RightMin->key);
        }
    }

    T->height = 1 + max(getHeight(T->left), getHeight(T->right));

    if(bf(T) > 1){
        if(bf(T->left) >= 0){
            T = rightRotate(T);
        }else{
            T->left = leftRotate(T->left);
            T = rightRotate(T);
        }
    }else if(bf(T) < -1){
        if(bf(T->right) <= 0){
            T = leftRotate(T);
        }else{
            T->right = rightRotate(T->right);
            T = leftRotate(T);
        }
    }

    return T;
}

void avl_destroy(AVLTree T){
    if( !T ){
        return;
    }
    avl_destroy(T->left);
    avl_destroy(T->right);
    free(T);
    return ;
}
void avl_inorder(AVLTree T){
    if( !T ){
        return ;
    }
    avl_inorder(T->left);
    printf("%d ",T->key);
    avl_inorder(T->right);
    return ;
}