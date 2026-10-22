#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAXN 101

int N, M, L;
int statement[MAXN];
int isWolf[MAXN];
int found = 0;

bool check(void){
    int liarCount = 0;
    int wolfLiarCount = 0;

    for(int i = 1; i <= N; i++){
        int target = abs(statement[i]);
        bool lie;

        if(statement[i] > 0){
            lie = isWolf[target];
        }else{
            lie = !isWolf[target];
        }

        if(lie){
            liarCount += 1;
            if(isWolf[i]){
                wolfLiarCount += 1;
            }
        }
    }
    
    return liarCount == L && wolfLiarCount >= 1 && wolfLiarCount < M;
}

bool possible(){
    int liarCount = 0;
    int unknownCount = 0;

    for(int i = 1; i <= N; i++){
        int target = abs(statement[i]);
        if(isWolf[target] == -1){
            unknownCount += 1;
            continue;
        }

        int lie;
        if(statement[i] > 0){
            lie = isWolf[target];
        }else{
            lie = !isWolf[target];
        }
        liarCount += lie;
    }
    
    if(liarCount > L){
        return false;
    }
    if(unknownCount + liarCount < L){
        return false;
    }
    return true;
}


void dfs(int player, int wolfCount){
    if(found){
        return ;
    }
    if(wolfCount > M){
        return ;
    }
    if(wolfCount + player < M){
        return ;
    }
    if(!possible()){
        return ;
    }
    
    if(player == 0){
        if(check()){
            found = 1;
        }
        return ;
    }
    //先假设第player号是狼
    isWolf[player] = 1;
    dfs(player-1, wolfCount+1);
    if(found){
        return ;
    }
    //找不到解再假设他是人
    isWolf[player] = 0;
    dfs(player-1, wolfCount);
    if(found){
        return ;
    }

    isWolf[player] = -1;
}


void Read(){
    for(int i = 1; i <= N; i++){
        scanf("%d", &statement[i]);
    }
    return ;
}
void Print(){
    if(!found){
        printf("No Solution\n");
    }
    int first = 1;
    for(int i = N; i >= 1; i--){
        if(isWolf[i] == 1){
            if(first){
                printf("%d", i);
                first = 0;
            }else{
                printf(" %d", i);
            }
        }
    }
    return ;
}

int main(){
    scanf("%d%d%d",&N, &M, &L);
    for(int i = 1; i <= N; i++){
        isWolf[i] = -1;
    }
    Read();
    dfs(N, 0);
    Print();
}