#ifndef BINOMIALQUEUE_H
#define BINOMIALQUEUE_H
#include <stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

#define TreeMAX 100

typedef int ElementType;

typedef struct BinTreeNode{
    ElementType key;
    int degree;
    struct BinTreeNode* LeftChild;
    struct BinTreeNode* NextSibling;
} BinTreeNode, *BinTree;

typedef struct BinQueueNode{
    long long Size;      //节点总数
    int min_index;
    BinTree TheTrees[TreeMAX];
} BinQueueNode;
typedef BinQueueNode* BinQueue;

bool isEmpty(BinQueue H);
BinTree newTree(ElementType x);
BinTree MergeTree(BinTree T1, BinTree T2);
BinQueue BuildQueue(void);
BinQueue Merge(BinQueue H1, BinQueue H2);
BinQueue Insert(BinQueue H, ElementType x);
ElementType DeletMin(BinQueue H);

#endif