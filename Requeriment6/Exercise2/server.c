#include "string.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <sys/sem.h>
#include <sys/shm.h>

void conv(char *, int);
void handle(int);

int msgq_id_up,msgq_id_down;
int shmid ,sem;
struct msgbuff
{
    long mtype;
    long work;
};


union Semun
{
    int val;               /* Value for SETVAL */
    struct semid_ds *buf;  /* Buffer for IPC_STAT, IPC_SET */
    unsigned short *array; /* Array for GETALL, SETALL */
    struct seminfo *__buf; /* Buffer for IPC_INFO (Linux-specific) */
};

int main(int argc ,char argv[]){

    signal(SIGINT,handle);
    union Semun semun;
    semun.val = 1;
    
    sem = semget(400, 1, 0666 | IPC_CREAT);
    semctl(sem, 0, SETVAL, semun);
    shmid = shmget(300, 256, IPC_CREAT | 0666);
    if(shmid == -1 )
    {
        printf("Error in creating shared memory");
        exit(-1);
    }
    void * shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (void *)-1)
    {
        perror("Error in attach in Client");
        exit(-1);
    }

    key_t key_id_up,key_id_down;


    key_id_up = ftok("fileUP", 65);
    key_id_down = ftok("fileDOWN", 65);

    msgq_id_up = msgget(key_id_up, 0666 | IPC_CREAT);
    msgq_id_down = msgget(key_id_down, 0666 | IPC_CREAT);

    printf("Shared Memeory ID %d\n",shmid);
    printf("Semaphore ID %d\n",sem);
    printf("Message Queue UP ID = %d\n", msgq_id_up);
    printf("Message Queue DOWN ID = %d\n", msgq_id_down);


    int send_val,rec_val;

    struct msgbuff message_sent;
    struct msgbuff message_recieve;
    while(1){

        rec_val = msgrcv(msgq_id_up, &message_recieve, sizeof(message_recieve.work),0,!IPC_NOWAIT );
        message_sent.mtype = message_recieve.mtype;
        conv((char*)shmaddr,256);
        send_val = msgsnd(msgq_id_down, &message_sent, sizeof(message_sent.work), !IPC_NOWAIT);
    }



    /* 0 -> receive all types of messages */
    // while(1){

    // IPC_NOWAIT: return -1 immediately if the queue is empty and the message can't be received
    // !IPC_NOWAIT : block until the message can be received
    // 0 : ignore
    // MSG_NOERROR : truncate the message if it is too long to fit in the queue (default: return -1 and the msg is not received and stays in queue)


}
void handle(int signum){
        msgctl(msgq_id_up, IPC_RMID, (struct msqid_ds *)0);
        msgctl(msgq_id_down, IPC_RMID, (struct msqid_ds *)0);
        shmctl(shmid, IPC_RMID, (struct shmid_ds *)0);
        semctl(sem, 0, IPC_RMID);
        exit(1);
}


void conv(char *msg, int size) {
int i;
for (i=0; i<size; ++i){
if (islower(msg[i]))
msg[i] = toupper(msg[i]);
else if (isupper(msg[i]))
msg[i] = tolower(msg[i]);
}
return;

}