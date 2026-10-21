#include"bst.h"
#include<stdio.h>
#include<stdlib.h>

// Binary Search Tree node structure
struct BSTNode{
    int key;              // key value stored in this node
    BSTree left;          // pointer to left subtree
    BSTree right;         // pointer to right subtree
};

// Create a new node with given key
static BSTree create_node(int key){
    BSTree node = (BSTree)malloc(sizeof(struct BSTNode));
    if( !node ){
        printf("Out of Space");
        exit(1);
    }
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}



// Create an empty BST (returns NULL)
BSTree bst_create(){
    return NULL;
}

// Insert a key into BST, return the root
BSTree bst_insert(BSTree T, int key){
    //T == NULL,  add a root
    if( !T ){
        return create_node(key);
    }

    struct BSTNode* parent = NULL;
    struct BSTNode* current = T;
    // Traverse to find the insertion position
    while(current != NULL){
        if(current->key > key){
            parent = current;
            current = current->left;
        }else if(current->key < key){
            parent = current;
            current = current->right;
        }else{
            // Duplicate key, do nothing
            return T;
        }
    }
    // Attach new node to the correct side of parent
    if(key < parent->key){
        parent->left = create_node(key);
    }else{
    parent->right = create_node(key);
    }

    return T;
}

// Delete a node with given key from BST
BSTree bst_delete(BSTree T, int key){
    if( !T ){
        return T;
    }

    struct BSTNode* parent = NULL;
    struct BSTNode* current = T;
    // Search for the node to delete
    while( current != NULL && current->key != key ){
        if(key < current->key){
            parent = current;
            current = current->left;
        }else{
            parent = current;
            current = current->right;
        }
    }
    //Not Found
    if( current == NULL ){
        return T;
    }
    //case1: leave
    if(!current->left && !current->right){
        //if delete root
        if(parent == NULL){
            free(current);
            return NULL;
        }
        else{
            if(parent->left == current){
                parent->left = NULL;
                free(current);
            }else{
                parent->right = NULL;
                free(current);
            }
        }
    }
    //case2: only 1 child
    else if((current->left != NULL) ^ (current->right != NULL)){
        BSTree OnlyChild = !current->left ? current->right : current->left;
        //if delete root
        if(parent == NULL){
            free(current);
            return OnlyChild;
        }
        else{
            if(parent->left == current){
                parent->left = OnlyChild;
                free(current);
            }else{
                parent->right = OnlyChild;
                free(current);
            }
        }
    }
    //case3: 2 children
    else{
        //Find MIN in Right
        struct BSTNode* p = current->right;
        struct BSTNode* q = NULL;
        while( p->left != NULL ){
            q = p;
            p = p->left;
        }
        int RightMin = p->key;

        // Replace current key with successor's key
        current->key = RightMin;
        if(q == NULL){
            current->right = p->right;
        }else{
            q->left = p->right;
        }
        free(p);
    }

    return T;
}

// In-order traversal: print keys in ascending order
void bst_inorder(BSTree T){
    if( !T ){
        return;
    }
    bst_inorder(T->left);
    printf("%d ",T->key);
    bst_inorder(T->right);
}

// Recursively free all nodes in the tree
void bst_destroy(BSTree T){
    if( !T ){
        return;
    }

    BSTree stack[100001];
    int top = -1;

    stack[++top] = T;

    while(top >= 0){
        BSTree current = stack[top--];

        if(current->left != NULL){
            stack[++top] = current->left;
        }
        if(current->right != NULL){
            stack[++top] = current->right;
        }

        free(current);
    }
}