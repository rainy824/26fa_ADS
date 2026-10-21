#include "BinomialQueue.h"

bool isEmpty(BinQueue H){
    return H->Size == 0;
}

BinTree newTree(ElementType x){
    BinTree T = (BinTree)malloc(sizeof(BinTreeNode));
    if(!T){
        fprintf(stderr, "Out of Space\n");
        exit(EXIT_FAILURE);
    }
    T->degree = 0;
    T->key = x;
    T->LeftChild = NULL;
    T->NextSibling = NULL;

    return T;
}

BinTree MergeTree(BinTree T1, BinTree T2){
    switch(!!T1*2 + !!T2){
        case 0: return NULL; break;
        case 1: return T2; break;
        case 2: return T1; break;
        case 3: break;
    }

    if(T1->degree != T2->degree){
        printf("Inequal Degree Merge!");
        exit(1);
    }

    if( T1->key > T2->key){
        return MergeTree(T2, T1);
    }

    T2->NextSibling = T1->LeftChild;
    T1->LeftChild = T2;
    T1->degree ++;

    return T1;
}

BinQueue BuildQueue(void){
    BinQueue newQueue = (BinQueue)calloc(1, sizeof(BinQueueNode));
    if(!newQueue){
        fprintf(stderr, "Out of Space\n");
        exit(EXIT_FAILURE);
    }

    newQueue->min_index = -1;
    newQueue->Size = 0;
    
    return newQueue;
}


BinQueue Merge(BinQueue H1, BinQueue H2){
    if(H1 == H2)return H1;


    BinTree T1, T2, Carry = NULL;
    if(H2->Size > LLONG_MAX - H1->Size){
        fprintf(stderr, "Queue size overflow\n");
        exit(EXIT_FAILURE);
    }
    H1->Size += H2->Size;
    H1->min_index = -1;
    long long i, remaining = H1->Size;
    
    for(i = 0; i < TreeMAX && remaining > 0; i++, remaining /= 2){
        T1 = H1->TheTrees[i], T2 = H2->TheTrees[i];

        switch(!!Carry*4 + !!T2*2 + !!T1){
            case 0: 
            case 1: break;
            case 2: H1->TheTrees[i] = T2; H2->TheTrees[i] = NULL; break;
            case 3: Carry = MergeTree(T1, T2); 
                    H1->TheTrees[i] = H2->TheTrees[i] = NULL;
                    break;
            case 4: H1->TheTrees[i] = Carry; Carry = NULL; break;
            case 5: Carry = MergeTree(Carry, T1);
                    H1->TheTrees[i] = NULL;
                    break;
            case 6: Carry = MergeTree(Carry, T2);
                    H2->TheTrees[i] = NULL;
                    break;
            case 7: Carry = MergeTree(Carry, T2);
                    H2->TheTrees[i] = NULL;
                    break;
        }

        if(H1->TheTrees[i] != NULL && (H1->min_index == -1 || H1->TheTrees[i]->key < H1->TheTrees[H1->min_index]->key)){
            H1->min_index = i;
        }
    }
    
    H2->Size = 0;
    H2->min_index = -1;

    return H1;
}

BinQueue Insert(BinQueue H, ElementType x){
    BinQueue newQueue = BuildQueue();
    newQueue->min_index = 0;
    newQueue->Size = 1;
    newQueue->TheTrees[0] = newTree(x);

    H = Merge(H, newQueue);
    free(newQueue);
    return H;
}


ElementType DeletMin(BinQueue H){
    if( isEmpty(H) ){
        printf("the Queue is Empty");
        exit(1);
    }

    int minItem;
    BinQueue tempQueue = BuildQueue();

    minItem = H->TheTrees[H->min_index]->key;
    BinTree DeletedTree = H->TheTrees[H->min_index];
    H->TheTrees[H->min_index] = NULL;
    BinTree OldRoot = DeletedTree;
    DeletedTree = DeletedTree->LeftChild;
    free(OldRoot);


    tempQueue->Size = (1LL <<H->min_index) - 1;
    for(int i = H->min_index - 1; i >= 0; i--){
        tempQueue->TheTrees[i] = DeletedTree;
        DeletedTree = DeletedTree->NextSibling;
        tempQueue->TheTrees[i]->NextSibling = NULL;
    }
    H->Size -= tempQueue->Size + 1;
    H = Merge(H, tempQueue);
    free(tempQueue);

    return minItem;
}