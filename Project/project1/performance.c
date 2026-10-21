#include <stdio.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "bst.h"
#include "AVL.h"
#include "splay.h"

#define RUN_TIME 5
#define RAND_SEED 42
#define MAX_N 10000

static LARGE_INTEGER freq;

static void init_timer(void){
    QueryPerformanceFrequency(&freq);
}

static double get_time_diff_ms(LARGE_INTEGER* start, LARGE_INTEGER* end){
    return (double)(end->QuadPart - start->QuadPart) / (double)freq.QuadPart * 1000.0;
}

static void generate_increase(int* arr, int n){
    for(int i = 0 ; i < n ; i++){
        arr[i] = i+1;
    }
}

static void shuffle_array(int* arr, int n){
    for(int i = n-1 ; i > 0 ; i--){
        int j = rand() % (i+1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}



static double test_bst_insert(int* data, int n){
    double total =0.0;
    for(int run = 0; run < RUN_TIME; run++){
        //1.create NULL Tree
        BSTree tree = bst_create();

        //2.start
        LARGE_INTEGER start, end;
        QueryPerformanceCounter(&start);

        for(int i = 0; i < n; i++){
            tree = bst_insert(tree, data[i]);
        }

        //3.end
        QueryPerformanceCounter(&end);

        total += get_time_diff_ms(&start, &end);
        bst_destroy(tree);
    }
    return total / RUN_TIME;
}

static double test_avl_insert(int* data, int n){
    double total =0.0;
    for(int run = 0; run < RUN_TIME; run++){
        //1.create NULL Tree
        AVLTree tree = avl_create();

        //2.start
        LARGE_INTEGER start, end;
        QueryPerformanceCounter(&start);

        for(int i = 0; i < n; i++){
            tree = avl_insert(tree, data[i]);
        }

        //3.end
        QueryPerformanceCounter(&end);

        total += get_time_diff_ms(&start, &end);
        avl_destroy(tree);
    }
    return total / RUN_TIME;
}

static double test_splay_insert(int* data, int n){
    double total =0.0;
    for(int run = 0; run < RUN_TIME; run++){
        //1.create NULL Tree
        SplayTree tree = splay_create();

        //2.start
        LARGE_INTEGER start, end;
        QueryPerformanceCounter(&start);

        for(int i = 0; i < n; i++){
            tree = splay_insert(tree, data[i]);
        }

        //3.end
        QueryPerformanceCounter(&end);

        total += get_time_diff_ms(&start, &end);
        splay_destroy(tree);
    }
    return total / RUN_TIME;
}

//====================================================================

static double test_bst_delete(int* insert_data, int* delete_data, int n){
    double total = 0.0;

    for(int run = 0 ; run < RUN_TIME ; run++){
        BSTree tree = bst_create();

        for(int i = 0 ; i < n ; i++){
            tree = bst_insert(tree, insert_data[i]);
        }

        LARGE_INTEGER start, end;
        QueryPerformanceCounter(&start);

        for(int i = 0 ; i < n ; i++){
            tree = bst_delete(tree, delete_data[i]);
        }

        QueryPerformanceCounter(&end);

        total += get_time_diff_ms(&start, &end);

        bst_destroy(tree);
    }
    return total / RUN_TIME;
}

static double test_avl_delete(int* insert_data, int* delete_data, int n){
    double total = 0.0;

    for(int run = 0 ; run < RUN_TIME ; run++){
        AVLTree tree = avl_create();

        for(int i = 0 ; i < n ; i++){
            tree = avl_insert(tree, insert_data[i]);
        }

        LARGE_INTEGER start, end;
        QueryPerformanceCounter(&start);

        for(int i = 0 ; i < n ; i++){
            tree = avl_delete(tree, delete_data[i]);
        }

        QueryPerformanceCounter(&end);

        total += get_time_diff_ms(&start, &end);

        avl_destroy(tree);
    }
    return total / RUN_TIME;
}

static double test_splay_delete(int* insert_data, int* delete_data, int n){
    double total = 0.0;

    for(int run = 0 ; run < RUN_TIME ; run++){
        SplayTree tree = splay_create();

        for(int i = 0 ; i < n ; i++){
            tree = splay_insert(tree, insert_data[i]);
        }

        LARGE_INTEGER start, end;
        QueryPerformanceCounter(&start);

        for(int i = 0 ; i < n ; i++){
            tree = splay_delete(tree, delete_data[i]);
        }

        QueryPerformanceCounter(&end);

        total += get_time_diff_ms(&start, &end);

        splay_destroy(tree);
    }
    return total / RUN_TIME;
}



int main(void){
    //1初始化随机种子
    srand(RAND_SEED);
    init_timer();

    //2打开CSV文件
    FILE* fp = fopen("result.csv", "w");
    if( !fp ){
        printf("unable to open\n");
        return 1;
    }


    //3CSV表头
    fprintf(fp, "N,场景,BST_insert_ms,BST_delete_ms,AVL_insert_ms,AVL_delete_ms,Splay_insert_ms,Splay_delete_ms\n");

    //4测试规模列表
    int sizes[] = {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    //5遍历每个测试的规模
    for(int s = 0 ; s < num_sizes ; s++){
        int n = sizes[s];
        printf("正在测试 N = %d ...\n", n);

        //准备数据
        int* arr_inc = (int*)malloc(n * sizeof(int));//升序数据
        int* arr_dec = (int*)malloc(n * sizeof(int));//降序数据
        int* arr_rand_ins = (int*)malloc(n * sizeof(int));//随机插入
        int* arr_rand_del = (int*)malloc(n * sizeof(int));//随机删除

        generate_increase(arr_inc, n);

        for(int i = 0 ; i < n ; i++){
            arr_dec[i] = n - i;
        }

        generate_increase(arr_rand_ins, n);
        shuffle_array(arr_rand_ins, n);

        generate_increase(arr_rand_del, n);
        shuffle_array(arr_rand_del, n);

        {
            //升序插入 + 升序删除
            double bst_ins = test_bst_insert(arr_inc, n);
            double bst_del = test_bst_delete(arr_inc, arr_inc, n);
            double avl_ins = test_avl_insert(arr_inc, n);
            double avl_del = test_avl_delete(arr_inc, arr_inc, n);
            double splay_ins = test_splay_insert(arr_inc, n);
            double splay_del = test_splay_delete(arr_inc, arr_inc, n);
            
            fprintf(fp, "%d,升序插入升序删除,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f\n", 
                        n, bst_ins, bst_del, avl_ins, avl_del, splay_ins, splay_del);
        }
        {
            //升序插入，降序删除
            double bst_ins = test_bst_insert(arr_inc, n);
            double bst_del = test_bst_delete(arr_inc, arr_dec, n);
            double avl_ins = test_avl_insert(arr_inc, n);
            double avl_del = test_avl_delete(arr_inc, arr_dec, n);
            double splay_ins = test_splay_insert(arr_inc, n);
            double splay_del = test_splay_delete(arr_inc, arr_dec, n);

            fprintf(fp, "%d,升序插入降序删除,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f\n", 
                        n, bst_ins, bst_del, avl_ins, avl_del, splay_ins, splay_del);
        }
        {
            //随机插入随机删除
            double bst_ins = test_bst_insert(arr_rand_ins, n);
            double bst_del = test_bst_delete(arr_rand_ins, arr_rand_del, n);
            double avl_ins = test_avl_insert(arr_rand_ins, n);
            double avl_del = test_avl_delete(arr_rand_ins, arr_rand_del, n);
            double splay_ins = test_splay_insert(arr_rand_ins, n);
            double splay_del = test_splay_delete(arr_rand_ins, arr_rand_del, n);

            fprintf(fp, "%d,随机插入随机删除,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f\n", 
                        n, bst_ins, bst_del, avl_ins, avl_del, splay_ins, splay_del);
        }

        free(arr_inc);
        free(arr_dec);
        free(arr_rand_ins);
        free(arr_rand_del);
    }

    fclose(fp);
    printf("测试完成，结果已写入 result.csv");
    return 0;
}