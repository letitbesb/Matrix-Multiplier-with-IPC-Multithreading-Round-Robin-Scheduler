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
char inpLine1[100];char inpLine2[100];
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

typedef struct {
    int alpha;
    // ...
} mystruct_t;


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
void *multiplier(void *parameters){
    parameter *param = (parameter *) parameters;
    int r1=param->col;
    pthread_mutex_lock(&lock2);
    if(arr5[r1][0]==0){
        krishn++;
        for (int j = 0; j < col2; ++j) {
            for (int k = 0; k < row2; ++k) {
                arr5[r1][j] += (arr3[r1][k] * arr4[k][j]);
                
            }
        }
    }
    pthread_mutex_unlock(&lock2);
}
int main(int argc,char *argv[]){

    if (argc != 7) {
		printf("Usage: ./groupX_assignment2.out i j k in1.txt in2.txt out.txt");
		exit(-1);
	}
    uint64_t diff;
	struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);	/* mark start time */
    long long int **arrf1;
    long long int **arrf2;
    row1=atoi(argv[1]); row2=atoi(argv[2]); col2=atoi(argv[3]);

    // Creating shared memory (Part 2 starting)

    // printf("he he bhaiya");

    key_t key1 = ftok("a.txt",34);
    key_t key2 = ftok("b.txt",35);
    int shmId1,shmId2;
    shmId1 = shmget(key1, row1*row2*sizeof(long long int), 0);    
    if(shmId1 == -1) {
        perror("error shmget 1");
        exit(1);
    }
    shmId2 = shmget(key2, row2*col2*sizeof(long long int), 0); 
    if(shmId2 == -1) {
        perror("error shmget 1");
        exit(1);
    }

    long long int (*arr11)[row2];
    long long int (*arr21)[col2];

    arr11=shmat(shmId1,NULL,0);
    arr21=shmat(shmId2,NULL,0);


    arr3=(long long int**)malloc(row1 * sizeof(long long int*));
    for (int i = 0; i < row1; i++)
        arr3[i] = (long long int*)malloc(row2 * sizeof(long long int));
    arr4=(long long int**)malloc(row2 * sizeof(long long int*));
    for (int i = 0; i < row2; i++)
        arr4[i] = (long long int*)malloc(col2 * sizeof(long long int));
    arr5=(long long int**)malloc(row1 * sizeof(long long int*));
    for (int i = 0; i < row1; i++)
        arr5[i] = (long long int*)malloc(col2 * sizeof(long long int));
    for(int i=0; i<row1;i++){
        for(int j=0; j<col2;j++){
            arr5[i][j]=0;
        }
    }
    // printf("%d\n",arr11[0][0]);
    for(int i=0; i<row1;i++){
        for(int j=0; j<row2;j++){
            arr3[i][j]=arr11[i][j];
            // if(i==23 && j==12) arr3[23][12]=99;
        }
    }
    for(int i=0; i<row2;i++){
        for(int j=0; j<col2;j++){
            arr4[i][j]=arr21[i][j];
        }
    }
    //print(arr3,row1,row2);
    // for(int i=0; i<row2*row1; i++){
    //     arr3[i/row2][i%row2]=arrf1[i];
    // }
    // for(int i=0; i<row2*col2; i++){
    //     arr4[i/col2][i%col2]=arrf2[i];
    // }

    int N=10;
    pthread_mutex_init(&lock2, NULL);
    pthread_t tID1[N];long long int RowsPerThread;
    if(N>=row1) RowsPerThread=1;
    else RowsPerThread=row1%N==0? row1/N:(row1/N)+1;
    for(int iter=0;iter<RowsPerThread;iter++){          // n threads will do 1 line each till 'RowsPerThread' number of times
        int called=0;
        for(int i=0;i<N;i++){              // n threads
            if((iter*N+i)<row1){
                parameter arg; arg.col=iter*N+i;
                pthread_create(&tID1[i], NULL,multiplier,(void *)&arg);
                pthread_join(tID1[i], NULL);
                called++;
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);	/* mark the end time */

    diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    // printf("elapsed time = %llu nanoseconds\n", (long long unsigned int) diff);

    // print(arr3,row1,row2);
    // printf("\n");
    // key_t key11 = ftok("a11.txt",65);
    // int shmid1 = shmget(key11,sizeof(mystruct_t),IPC_CREAT | S_IRUSR | S_IWUSR);;
    // char *s2;
    // s2=shmat(shmid1,NULL,0);

    // print(arr5,row1,col2);
    key_t key12 = ftok("b11.txt",66);
    int shmid2 = shmget(key12,1024,0666|IPC_CREAT);
    char *str2 = (char*) shmat(shmid2,(void*)0,0);

    FILE *csvfp;
    csvfp = fopen("valuesP2.csv", "a+");
    uint64_t runtime = diff;
    int threads = N;    // max threads
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
    // while(1){
    //     print(arr5,row1,col2);
    //     usleep(5000);
    // }
    // FILE *fpout=NULL;             //to plot the graph of time vs threads
    // fpout=fopen("out.txt","w");
    // for(int i=0; i<row1;i++){
    //     for(int j=0; j<col2;j++){
    //         fprintf(fpout,"%d ",arr5[i][j]);
    //     }
    //     fprintf(fpout,"\n");
    // }
    // fclose(fpout);

    shmdt(arr11);
    shmdt(arr21);
    shmctl(shmId1,IPC_RMID,NULL);
    shmctl(shmId2,IPC_RMID,NULL);
    str2='d';
    shmdt(str2);

    // shmdt(s2);
    return 0;
}