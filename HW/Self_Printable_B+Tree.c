#include<stdio.h>
#include<stdlib.h>
#define MAX_CAPACITY 4
#define MAX_SIZE 3

struct BTNode{
    int leaf;
    int key[MAX_CAPACITY];
    int cnt;
    struct BTNode* children[MAX_CAPACITY];
};

struct BTNode* FindLeaf(struct BTNode* T, int x){
    if(!T){
        return NULL;
    }

    struct BTNode* cur = T;
    while(!cur->leaf){
        int i = 0;
        while(i < cur->cnt && cur->key[i] <= x){
            i++;
        }
        cur = cur->children[i];
    }

    return cur;
}

int InsertInto(struct BTNode* node, int x, int* upKey, struct BTNode** upRight){
    //node is a leaf
    if(node->leaf){
        int i;
        for(i = 0; i < node->cnt; i++){
            if(node->key[i] == x){
                printf("Key %d is duplicated\n", x);
                return 0;
            }
            if(node->key[i] > x){
                break;
            }
        }
        for(int j = node->cnt; j > i; j--){
            node->key[j] = node->key[j-1];
        }
        node->key[i] = x;
        node->cnt++;
        //if leaf is full
        if(node->cnt == MAX_CAPACITY){
            struct BTNode* right = (struct BTNode*)calloc(1, sizeof(struct BTNode));
            right->leaf = 1;
            right->cnt = 2;
            right->key[0] = node->key[2];
            right->key[1] = node->key[3];
            node->cnt = 2;//left node keeps key[0:1]
            *upKey = right->key[0]; //lift the least key in right
            *upRight = right;
            return 1;
        }
        //else leaf is not full
        else{
            return 0;
        }
    }
    //else node is interior
    else{
        int ci = 0;
        while(ci < node->cnt && node->key[ci] <= x){
            ci++;
        }
        int upK;
        struct BTNode* upR = NULL;
        if(InsertInto(node->children[ci], x, &upK, &upR) == 0){
            return 0;
        }
        //else child splitted
        for(int j = node->cnt; j > ci; j--){
            node->key[j] = node->key[j-1];
        }
        node->key[ci] = upK;
        for(int j = node->cnt + 1; j > ci + 1; j--){
            node->children[j] = node->children[j-1];
        }
        node->children[ci+1] = upR;
        node->cnt++;
        if(node->cnt == MAX_SIZE){
            struct BTNode* right = (struct BTNode*)calloc(1, sizeof(struct BTNode));
            right->leaf = 0;
            right->cnt = 1;
            right->key[0] = node->key[2];
            right->children[0] = node->children[2];
            right->children[1] = node->children[3];
            *upKey = node->key[1];
            node->cnt = 1;
            *upRight = right;
            return 1;
        }
        //else node not split
        return 0;
    }
}


struct BTNode* Insert(struct BTNode* T, int x){
    if(!T){
        struct BTNode* root = (struct BTNode*)calloc(1, sizeof(struct BTNode));
        root->leaf = 1;
        root->key[0] = x;
        root->cnt = 1;
        return root;
    }
    int upKey;
    struct BTNode* upRight = NULL;
    if(InsertInto(T, x, &upKey, &upRight)){
        struct BTNode* newRoot = (struct BTNode*)calloc(1, sizeof(struct BTNode));
        newRoot->leaf = 0;
        newRoot->cnt = 1;
        newRoot->key[0] = upKey;
        newRoot->children[0] = T;
        newRoot->children[1] = upRight;
        return newRoot;
    }
    return T;
}

int main(){
    int N;
    scanf("%d", &N);

    struct BTNode* T = NULL;

    int key;
    for(int i = 0 ; i < N; i++){
        scanf("%d", &key);
        T = Insert(T, key);
    }

    struct BTNode* queue[2*N];
    int front = 0, tail = 0;
    queue[tail++] = T;
    while(tail != front){
        int levelSize = tail-front;
        for(int i = 0; i < levelSize; i++){
            struct BTNode* cur = queue[front++];
            if(!cur->leaf){
                for(int j = 0; j < cur->cnt+1; j++){
                    queue[tail++] = cur->children[j];
                }
            }
            int first=1;
            printf("[");
            for(int j = 0 ; j < cur->cnt ; j++){
                if(first){
                    printf("%d", cur->key[j]);
                    first = 0;
                }else{
                    printf(",%d", cur->key[j]);
                }
            }
            printf("]");
        }
        printf("\n");
    }

    return 0;

}