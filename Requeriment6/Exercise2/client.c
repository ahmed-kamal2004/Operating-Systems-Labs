#include "string.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>

void handle(int);


union Semun
{
    int val;               /* Value for SETVAL */
    struct semid_ds *buf;  /* Buffer for IPC_STAT, IPC_SET */
    unsigned short *array; /* Array for GETALL, SETALL */
    struct seminfo *__buf; /* Buffer for IPC_INFO (Linux-specific) */
};

void down(int sem)
{
    struct sembuf op;

    op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = !IPC_NOWAIT;

    if (semop(sem, &op, 1) == -1)
    {
        perror("Error in down()");
        exit(-1);
    }
}

void up(int sem)
{
    struct sembuf op;

    op.sem_num = 0;
    op.sem_op = 1;
    op.sem_flg = !IPC_NOWAIT;

    if (semop(sem, &op, 1) == -1)
    {
        perror("Error in up()");
        exit(-1);
    }
}

struct msgbuff
{
    long mtype;
    long work;
};

void *shmaddr;

int main(int argc ,char argv[]){

    signal(SIGINT,handle);

    key_t key_id_up,key_id_down;
    int msgq_id_up,msgq_id_down;

    union Semun semun;
    semun.val = 1;
    int sem = semget(400, 1, 0666 | IPC_CREAT);
    int shmid = shmget(300, 256, IPC_CREAT | 0666);
    int sem_shmid = shmget(500, 256, IPC_CREAT | 0666);
    if(shmid == -1 || sem == -1 || semctl(sem, 0, SETVAL, semun) == -1)
    {
        printf("Error in creating shared memory || semaphore");
        exit(-1);
    }
    shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (void *)-1)
    {
        perror("Error in attach in Client");
        exit(-1);
    }
    key_id_up = ftok("fileUP", 65);
    key_id_down = ftok("fileDOWN", 65);

    msgq_id_up = msgget(key_id_up, 0666 | IPC_CREAT);
    msgq_id_down = msgget(key_id_down, 0666 | IPC_CREAT);

    printf("Message Queue UP ID = %d\n", msgq_id_up);
    printf("Message Queue DOWN ID = %d\n", msgq_id_down);
    printf("Shared Memeory ID %d\n",shmid);
    printf("Semaphore ID %d\n",sem);

    int send_val,rec_val;

    struct msgbuff message_sent;
    message_sent.mtype = getpid()%10000;
    struct msgbuff message_recieve;
    message_recieve.mtype = getpid()%10000;


    char name[250];


    while(1){
        printf("Sending to server.....(%d)",getpid());
        printf("Enter Sentence : ");
        fflush(stdin);
        fgets(name, 250, stdin);
        sleep(5);
        down(sem);
        strcpy((char*)shmaddr,name);
        send_val = msgsnd(msgq_id_up, &message_sent, sizeof(message_sent.work), !IPC_NOWAIT);
        rec_val = msgrcv(msgq_id_down, &message_recieve, sizeof(message_recieve.work), getpid()%10000,!IPC_NOWAIT );
        up(sem);
        printf("Output After Conversion >>>> %s\n",(char*)shmaddr);
    }



    /* 0 -> receive all types of messages */
    // while(1){

    // IPC_NOWAIT: return -1 immediately if the queue is empty and the message can't be received
    // !IPC_NOWAIT : block until the message can be received
    // 0 : ignore
    // MSG_NOERROR : truncate the message if it is too long to fit in the queue (default: return -1 and the msg is not received and stays in queue)
}

void handle(int signum){
      shmdt(shmaddr);
      exit(0);
}