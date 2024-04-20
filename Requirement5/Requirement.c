#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


void parent_handler(int signum);

int arr_pid[2];
int search_val;

int main(int size,char*data[]){
    int*ptr = (int*) malloc((size-1) * sizeof(int));
    for(int counter = 1;counter < size-1;counter++){
        ptr[counter] = atoi(data[counter+1]);
    }
    int arr_size = size-2;
    search_val = atoi(data[1]);
    printf("I am the parent, PID = %d\n",getpid());

    signal(SIGUSR1,parent_handler); // here is better bec the child may exit before entering the parent
    int pid = fork();
    arr_pid[0] = pid;
    if(pid == 0){
        // First child
        printf("I am the first child, PID = %d, PPID = %d\n",getpid(),getppid());

        int X;
        int process_arr_size = arr_size/2;
        for(int looper = 1;looper < process_arr_size;looper++){
            if(ptr[looper] == search_val){
                X = looper;
                kill(getppid(),SIGUSR1); // if found
                exit(X);
            }
        }
        // if not found
        sleep(3);
        printf("Child 1 Terminates\n");
    }
    else{
        pid = fork();
        arr_pid[1] = pid;
        if(pid == 0){
            //second child
            printf("I am the second child, PID = %d, PPID = %d\n",getpid(),getppid());

            int X;
            int process_arr_size = arr_size/2;
            for(int looper = process_arr_size;looper <= arr_size+1;looper++){
                if(ptr[looper] == search_val){
                    X = looper;
                    kill(getppid(),SIGUSR1); // if found
                    exit(X);
                }
            }
            // if not found
            sleep(3);
            printf("Child 2 Terminates\n");
        }
        else{
            //parent
            sleep(5);
            int status=0;
            for(int i = 0;i<2;i++)
                waitpid(arr_pid[i],&status,1);
            printf("Value Not found\n");
        }
    }

}


void parent_handler(int signum){
    int  status_loc = 0;
    for(int i = 0;i<2;i++){
        waitpid(arr_pid[i],&status_loc,0);
        if (!(status_loc & 0x00FF) && status_loc > 0){
	    	printf("Child %d: Value %d found at position %d\n",i+1, search_val ,(status_loc >> 8)  -1);
            killpg(getpgrp(),SIGINT);
            break;
        }
    }

}