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
#include <signal.h>
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

long long int min(long long int a, long long int b){
    return a < b ? a : b;
}
long long int max(long long int a, long long int b){
    return a > b ? a : b;
}
bool checker1(){
    
}
bool checker2(){

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

int main(int argc,char *argv[]){

    if (argc != 7) {
		printf("Usage: ./groupX_assignment2.out i j k in1.txt in2.txt out.txt");
		exit(-1);
	}
    key_t key11 = ftok("a11.txt",66);
    key_t key12 = ftok("b11.txt",66);
    int shmid1 = shmget(key11,1024,0666|IPC_CREAT);
    int shmid2 = shmget(key12,1024,0666|IPC_CREAT);
    char *str1 = (char*) shmat(shmid1,(void*)0,0);
    char *str2 = (char*) shmat(shmid2,(void*)0,0);
    int pid1 = fork();
    if(pid1==-1){
        return 1;
    }
    if(pid1){  //og parent 
        int pid2=fork();
        if(pid2){   //og parent
            kill(pid1,SIGSTOP);
            kill(pid2,SIGSTOP);
            int t; int k=1;
            // printf("Enter time quantum in micro seconds for execeution: ");
            // scanf("%d",&t);
            t=2;
            // time_t secs =5; // 5 minutes (can be retrieved from user's input)
            // time_t startTime = time(NULL);
            // printf("haww");
            do{
                if(t>0&&k==1){
                    kill(pid1,SIGCONT);       //starts P1
                    usleep(t);   // goes till t seconds
                    kill(pid1,SIGSTOP);   //now stops P1
                    k=2; //now 2 has more priority
                }
                if(t>0&&k==2){
                    kill(pid2,SIGCONT);       //starts P2
                    usleep(t);   // goes till t seconds
                    kill(pid2,SIGSTOP);   //now stops P2
                    k=1; //now 1 has more priority
                }
            }while (str1!='d'&&str2!='d');
            wait(NULL);
        }
        else{      //child 2
            execv("./2.out", argv);
        }
    }
    else{       //child 1
        execv("./1.out", argv);
    }

    shmdt(str1);
    shmdt(str2);
    shmctl(shmid1,IPC_RMID,NULL);
    shmctl(shmid2,IPC_RMID,NULL);
    
    return 0;
}