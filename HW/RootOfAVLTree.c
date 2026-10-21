#include<stdio.h>
#include<stdlib.h>

typedef struct Node{
    int key;
    int height;
    struct Node* left;
    struct Node* right;
} Node;

typedef Node* Tree;

static int max(int a, int b){
    return a > b ? a : b;
}

Node* createNode(int key){
    Node* node = (Node*)calloc(1, sizeof(Node));
    node->height = 0;
    node->key = key;
    node->left = NULL;
    node->right = NULL;

    return node;
}

int getHeight(Tree T){
    if( !T ){
        return -1;
    }
    return T->height;
}

Tree rightRotate(Tree T){
    Node* newRoot = T->left;
    T->left = T->left->right;
    newRoot->right = T;

    T->height = 1 + max(getHeight(T->left), getHeight(T->right));
    newRoot->height = 1 + max(getHeight(newRoot->left), getHeight(newRoot->right));

    return newRoot;
}

Tree leftRotate(Tree T){
    Node* newRoot = T->right;
    T->right = T->right->left;
    newRoot->left = T;

    T->height = 1 + max(getHeight(T->left), getHeight(T->right));
    newRoot->height = 1 + max(getHeight(newRoot->left), getHeight(newRoot->right));

    return newRoot;
}

int bf(Node* node){
    if( !node )return 0;
    return getHeight(node->left) - getHeight(node->right);
}

Tree Insert(Tree T, int key){
    if( !T ){
        return T = createNode(key);
    }

    if(T->key == key){
        exit(1);
    }
    else if(T->key > key){
        T->left = Insert(T->left, key);
    }
    else{
        T->right = Insert(T->right, key);
    }

    T->height = 1 + max(getHeight(T->left), getHeight(T->right));

    //int bf = getHeight(T->left) - getHeight(T->right);
    if(bf(T) > 1 ){
        if(bf(T->left) >= 0){
            T = rightRotate(T);
        }else{
            T->left = leftRotate(T->left);
            T = rightRotate(T);
        }
    }
    else if(bf(T) < -1){
        if(bf(T->right) <= 0){
            T = leftRotate(T);
        }else{
            T->right = rightRotate(T->right);
            T = leftRotate(T);
        }
    }
    return T;
}

int main(){
    int N;
    scanf("%d", &N);

    Tree T = NULL;
    int key;
    for(int i = 0; i < N; i++){
        scanf("%d", &key);
        T = Insert(T, key);
    }

    if(T)
        printf("%d", T->key);

    return 0;
}


