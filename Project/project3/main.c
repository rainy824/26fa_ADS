#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<limits.h>
#define MAXM 101


int N, M;
int totalCost;
int HazarTable[MAXM][MAXM];
int PriceTable[MAXM];
int ansSize;
int ansList[MAXM];

int FeasibleansList[MAXM];
int FeasibleansSize;

void __init__(void){
    scanf("%d %d", &N, &M);

    //initialize Hazard Table
    for(int i = 1; i <= N; i++){
        int u, v;
        scanf("%d %d", &u, &v);
        HazarTable[u][v] = HazarTable[v][u] = 1;
    }

    //initialize Price Table
    for(int i = 1; i <= M; i++){
        int price, fruit;
        scanf("%d %d", &fruit, &price);
        PriceTable[fruit] = price;
    }

}

bool check(int newFruit){
    for(int i = 1; i <= ansSize; i++){
        if(HazarTable[ansList[i]][newFruit]){
            return false;
        }
    }
    return true;
}

void clean(){
    for(int i = 1; i <= ansSize; i++){
        ansList[i] = 0;
    }
    ansSize = 0;
}

void list(){
    for(int i = 1; i <= ansSize; i++){
        FeasibleansList[i] = ansList[i];
    }
    FeasibleansSize = ansSize;
}

bool dfs(int newFruit, int targetSize){
    bool Found = false;
    if(ansSize == targetSize){ //找到一个解
        return true;
    }

    if(newFruit > M){
        return false;
    }

    if(ansSize + (M - newFruit + 1) < targetSize){
        return false;
    }

    if(check(newFruit)){
        ansSize += 1;
        ansList[ansSize] = newFruit;
        Found = dfs(newFruit+1, targetSize);
        if(Found){
            return true;
        }
        else{
            ansList[ansSize] = 0; 
            ansSize -= 1;
        }
    }
    return dfs(newFruit+1, targetSize);
}

void dfsMinCost(int newFruit, int targetSize, int currentCost){
    if(ansSize == targetSize){
        if(currentCost < totalCost){
            totalCost = currentCost;
            list();
        }
        return ;
    }

    if(newFruit > M){
        return ;
    }
    if(ansSize + (M - newFruit + 1) < targetSize){
        return;
    }
    if(currentCost >= totalCost){
        return ;
    }

    //option1 : add
    if(check(newFruit)){
        ansSize += 1;
        ansList[ansSize] = newFruit;
        currentCost += PriceTable[newFruit];
        dfsMinCost(newFruit+1, targetSize, currentCost);

        //option2: skip
        ansList[ansSize] = 0;
        ansSize -= 1;
        currentCost -= PriceTable[newFruit];
    }
    dfsMinCost(newFruit+1, targetSize, currentCost);
}

void print(){
    int first = 1;
    for(int i = 1; i <= FeasibleansSize; i++){
        if(first){
            printf("%03d", FeasibleansList[i]);
            first = 0;
        }else{
            printf(" ");
            printf("%03d", FeasibleansList[i]);
        }
    }
    printf("\n");
}

int main(){

    __init__();
    int left = 1, right = M;
    while(left <= right){
        int mid = left + (right - left) / 2;
        bool found = dfs(1, mid);
        if(found){
            list(); //记录解
            clean(); //清除，继续搜索
            left = mid + 1;
        }else{
            right = mid - 1;
        }
    }

    for(int i = 1; i <= FeasibleansSize; i++){
        totalCost += PriceTable[FeasibleansList[i]];
    }

    clean();
    dfsMinCost(1, FeasibleansSize, 0);
    printf("%d\n", FeasibleansSize);
    print();
    printf("%d", totalCost);

    return 0;
}