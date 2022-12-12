#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>
#include <limits.h>
#include<stdbool.h>
#include <stdint.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define BILLION 1000000000L

FILE *in1;
FILE *in2;
int n; int krishn=0;
long long int row1,row2,col2,cur_row1,cur_row2;
char inpLine1[10000];char inpLine2[10000];
pthread_mutex_t lock;
pthread_mutex_t lock2;
long long int **arr1;
long long int **arr2;
long long int **arr3;
long long int **arr4;
long long int **arr5;
FILE *in1;
FILE *in2; int x=1;

typedef struct {
    int col;
} parameter;

long long int min(long long int a, long long int b){
    return a < b ? a : b;
}
long long int max(long long int a, long long int b){
    return a > b ? a : b;
}

void print(long long int **arr1,int rows,int cols){
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            printf("%lld ",arr1[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
void *solveOnly1(void *parameters){
    pthread_mutex_lock(&lock);
        int col1=0;
    
        fgets(inpLine1, sizeof(inpLine1),in1); 

        char * token1 = strtok(inpLine1, " ");
        while( token1 != NULL) {
            arr1[cur_row1][col1++]=atoi(token1);
            token1 = strtok(NULL, " ");
        }
        cur_row1++;   
    pthread_mutex_unlock(&lock);
}
void *solveOnly2(void *parameters){
    pthread_mutex_lock(&lock);
        int col2=0;
    
        fgets(inpLine2, sizeof(inpLine2),in2); 

        char * token2 = strtok(inpLine2, " ");
        while( token2 != NULL) {
            arr1[cur_row2][col2++]=atoi(token2);
            token2 = strtok(NULL, " ");
        }
        cur_row2++;   
    pthread_mutex_unlock(&lock);
}
void *solve(void *parameters){
    pthread_mutex_lock(&lock);
        int col1=0;int col2=0;
    
        fgets(inpLine1, sizeof(inpLine1),in1); 
        char * token1 = strtok(inpLine1, " ");
        while( token1 != NULL) {
            arr1[cur_row1][col1++]=atoi(token1);
            token1 = strtok(NULL, " ");
        }
        cur_row1++;
        fgets(inpLine2, sizeof(inpLine2),in2);
        char * token2 = strtok(inpLine2, " ");
        while(token2!=NULL){
            arr2[cur_row2][col2++]=atoi(token2);
            token2 = strtok(NULL, " ");
        }
        cur_row2++;
    pthread_mutex_unlock(&lock);
}
unsigned int sizeof_dm(int rows, int cols, size_t sizeElement){
    size_t size = rows * (sizeof(void *) + (cols * sizeElement));
    return size;
}
void create_index(void **m, int rows, int cols, size_t sizeElement){
    int i;  
    size_t sizeRow = cols * sizeElement;
    m[0] = m+rows;
    for(i=1; i<rows; i++){      
        m[i] = (m[i-1]+sizeRow);
    }
}
int main(int argc,char *argv[]){

    if (argc != 7) {
		printf("Usage: ./groupX_assignment2.out i j k in1.txt in2.txt out.txt");
		exit(-1);
	}
    uint64_t diff;
	struct timespec start, end;
    long long int *arrf1;
    long long int *arrf2;
    long long int *p; 
    p = malloc( 100000 * sizeof( long long int ) );

    // initializes elements in the array
    for (long long int i = 0; i < 100000; ++i ) {
        p[i] = i * 4;
    }
    // printf("%lld",p[100000]);
    arrf1 = malloc((row1*row2) * sizeof(long long int));
    arrf2 = malloc((row2*col2) * sizeof(long long int));

    row1=atoi(argv[1]); row2=atoi(argv[2]); col2=atoi(argv[3]);
    // Creating shared memory (Part 2 starting)
    
 
        //PROCESS 1 
    clock_gettime(CLOCK_MONOTONIC, &start);	/* mark start time */
    
    arr1=(long long int*)malloc(row1 * sizeof(long long int));
    for (int i = 0; i < row1; i++)
        arr1[i] = (long long int*)malloc(row2 * sizeof(long long int));
    arr2=(long long int*)malloc(row2 * sizeof(long long int));
    for (int i = 0; i < row2; i++)
        arr2[i] = (long long int*)malloc(col2 * sizeof(long long int));

    //open input file 1
    
    in1= fopen(argv[4], "r");
    in2= fopen(argv[5], "r");
    
    if(in1==NULL || in2==NULL){
        printf("Error! opening input file(s)\n");
        exit(0);
    }
    
    n=1;
    pthread_t tID[n];
    pthread_mutex_init(&lock, NULL);long long int RowsPerThread;
    int temp=row1; 
    row1=min(row1,row2);
    if(n>=row1) RowsPerThread=1;
    else RowsPerThread=row1%n==0? row1/n:(row1/n)+1;
    for(int iter=0;iter<RowsPerThread;iter++){          // n threads will do 1 line each till 'RowsPerThread' number of times
        int called=0;
        for(int i=0;i<n;i++){              // n threads
            if((iter*n+i)<row1){
                pthread_create(&tID[i], NULL, solve,NULL);
                called++;
            }
        }
        for(int i = 0; i < called; i++) pthread_join(tID[i], NULL);
    }
    row1=temp;
    if(row1>row2){
        row1=row1-row2;
        if(n>=row1) RowsPerThread=1;
        else RowsPerThread=row1%n==0? row1/n:(row1/n)+1;
        for(int iter=0;iter<RowsPerThread;iter++){          // n threads will do 1 line each till 'RowsPerThread' number of times
            int called=0;
            for(int i=0;i<n;i++){              // n threads
                if((iter*n+i)<row1){
                    pthread_create(&tID[i], NULL, solveOnly1,NULL);
                    called++;
                }
            }
            for(int i = 0; i < called; i++) pthread_join(tID[i], NULL);
        }
    }
    else if(row2>row1){
        row1=row2-row1;
        if(n>=row1) RowsPerThread=1;
        else RowsPerThread=row1%n==0? row1/n:(row1/n)+1;
        for(int iter=0;iter<RowsPerThread;iter++){          // n threads will do 1 line each till 'RowsPerThread' number of times
            int called=0;
            for(int i=0;i<n;i++){              // n threads
                if((iter*n+i)<row1){
                    pthread_create(&tID[i], NULL, solveOnly2,NULL);
                    called++;
                }
            }
            for(int i = 0; i < called; i++) pthread_join(tID[i], NULL);
        }
    }
    row1=temp;
    //printf("%lld",row1);
    fclose(in1);
    fclose(in2);
    clock_gettime(CLOCK_MONOTONIC, &end);	/* mark the end time */

    diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    // printf("elapsed time = %llu nanoseconds\n", (long long unsigned int) diff);



    FILE *csvfp;
    csvfp = fopen("values.csv", "a+");
    uint64_t runtime = diff;
    int threads = n;    // max threads
    int inputSize = row1*col2;

    if (csvfp == NULL)
    {
        printf("CSV File could not be opened\n");
        getchar(); // use getchar() to pause for input
        return 1;
    }
    fprintf(csvfp, "%llu,%d,%d\n", (long long unsigned int)runtime, threads, inputSize); // Single column print sample

    fclose(csvfp);
    csvfp = 0;
    // system("clear"); // clearing the terminal
    key_t key11 = ftok("a11.txt",66);
    int shmid1 = shmget(key11,1024,0666|IPC_CREAT);
    char *str1 = (char*) shmat(shmid1,(void*)0,0);
    
    // printf("Performance metrics of the run written to values.csv successfully!.\n");


    key_t key1 = ftok("a.txt",34);
    key_t key2 = ftok("b.txt",35);
    int shmId1,shmId2;
    shmId1 = shmget(key1, row1*row2*sizeof(long long int), IPC_CREAT|0666);    
    shmId2 = shmget(key2, row2*col2*sizeof(long long int), IPC_CREAT|0666); 
    //long long int **arr11;
    long long int (*arr11)[row2];
    long long int (*arr21)[col2];
   // long long int **arr21;
    arr11=shmat(shmId1,NULL,0);
    for(long long int i=0;i<row1;i++){
        for(long long int j=0;j<row2;j++){
            arr11[i][j]=arr1[i][j];
        }
    }
    arr21=shmat(shmId2,NULL,0);
    // for(long long int i=0;i<row2;i++){
    //     arr21[i] = (long long int*)malloc(col2 * sizeof(long long int));
    // }
    for(long long int i=0;i<row2;i++){
        for(long long int j=0;j<col2;j++){
            arr21[i][j]=arr2[i][j];
        }
    }

    // printf("%lld--%lld\n",arr11[1][1],arr11[0][0]);
 
    // for(int i=0; i<row1;i++){
    //     for(int j=0; j<row2;j++){
    //         arrf1[i*row2+j]=arr1[i][j];
    //     }
    // }
    // for(int i=0; i<row2;i++){
    //     for(int j=0; j<col2;j++){
    //         arrf2[i*col2+j]=arr2[i][j];
    //     }
    // }
    // printf("%lld",row1);
    // while(1){
    //     print(arr1,row1,row2);
    //     usleep(5000);
    //     }
    // print(arr1,row1,row2);
    //sleep(10);
    shmdt(arr11);
    shmdt(arr21);
    str1='d';
    shmdt(str1);
    // shmctl(shmId1,IPC_RMID,NULL);
    // shmctl(shmId2,IPC_RMID,NULL);
    
    return 0;
}