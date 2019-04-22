#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define QSIZE 20
char cirQ[QSIZE];
int cursor=0,finished=0;
void *producer(void *param){
        FILE *fp=fopen("string.txt", "r");
        if(fp==NULL){
                printf("\nCan't open file");
                pthread_exit(NULL);
                exit(1);
        }
        char ch;
        int i=0;
//      struct timespec ts;
//      ts.tv_sec=1;  // seconds
//      ts.tv_nsec=0; // nanoseconds=10ms
//      while(i<10){
        while((ch = fgetc(fp)) != EOF){  // EOF=-1
                cirQ[i%QSIZE] = ch;
//              nanosleep(&ts, NULL);
                sleep(1);  // 1 sec sleep
                ++i;
                ++cursor;
        }
        finished=1;
        pthread_exit(NULL);
}
int semaphore=1;
void *consumer1(void *param){
        int i=0,writer=0;
        char A[10];
        while(1){
                while(semaphore<=0) // wait
                        ;
//              printf("\nIn sem 1 i(%d) cur(%d)",i,cursor);
                while(i<cursor){
                        if((i+1)%3){
//                              printf("%c",cirQ[i]);
                                A[writer++]=cirQ[i];
                        }
                        ++i;
                }
                semaphore--;
                if(finished) break;
        }
        printf("\n/-/-/-/-/- Consumer 1(%d chars) -\\-\\-\\-\\-\\-\\-\\ \n",writer);
        for(i=0; i<writer; ++i){
                printf("%c", A[i]);
        }
        printf("\n");
        pthread_exit(NULL);
}
void *consumer2(void *param){
        int i=0, writer=0;
        char B[10];
        while(1){
                while(semaphore>0) // wait
                        ;
//              printf("\nIn sem 2 i(%d) cur(%d)",i,cursor);
                while(i<cursor){
//                      printf("^^^^^^^^^^^^^^^^^^^Iamin^^^^^^^^^^^^^^");
//                      if(0 == (i+1)%3){
                        if(!((i+1)%3)){  // ! has more precedence than %(had a hard time finding this bug)
//                              printf("\n########$$$$$$$%c$$$$#####\n",cirQ[i]);
                                B[writer++]=cirQ[i];
                        }
                        ++i;
                }
                semaphore++;
                if(finished) break;
        }
        printf("\n/-/-/-/-/- Consumer 2(%d chars) -\\-\\-\\-\\-\\-\\-\\ \n",writer);
        for(i=0; i<writer; ++i){
                printf("%c", B[i]);
        }
        printf("\n");
        pthread_exit(NULL);
}
//FILE *fp;
int main(int argc, char *argv[]){
//      i am here to rule
        pthread_t prod_tid, cons1_tid, cons2_tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&prod_tid, &attr,producer,NULL);
        pthread_create(&cons1_tid, &attr,consumer1,NULL);
        pthread_create(&cons2_tid, &attr,consumer2,NULL);
        pthread_join(prod_tid,NULL);
        pthread_join(cons1_tid,NULL);
        pthread_join(cons2_tid,NULL);
        return 0;
}