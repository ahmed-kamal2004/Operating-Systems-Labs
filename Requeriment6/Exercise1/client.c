#include "string.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>



struct msgbuff
{
    long mtype;
    char mtext[256];
};

int main(int argc ,char argv[]){
    key_t key_id_up,key_id_down;
    int msgq_id_up,msgq_id_down;

    key_id_up = ftok("fileUP", 65);
    key_id_down = ftok("fileDOWN", 65);

    msgq_id_up = msgget(key_id_up, 0666 | IPC_CREAT);
    msgq_id_down = msgget(key_id_down, 0666 | IPC_CREAT);

    printf("Message Queue UP ID = %d\n", msgq_id_up);
    printf("Message Queue DOWN ID = %d\n", msgq_id_down);

    int send_val,rec_val;

    struct msgbuff message_sent;
    message_sent.mtype = getpid()%10000;
    struct msgbuff message_recieve;
    message_recieve.mtype = getpid()%10000;


    char name[250];
    printf("Enter Sentence : ");
    fflush(stdin);
    fgets(name, 250, stdin);

    while(1){

        printf("Enter String : for process PID :  %d\n",getpid());

        printf("Sending to server.....(%d)",getpid());
        strcpy(message_sent.mtext,name);

        send_val = msgsnd(msgq_id_up, &message_sent, sizeof(message_sent.mtext), !IPC_NOWAIT);
        if(send_val == -1)
        {
            printf("Error in sending ! (%d)",getpid());
            perror("");
        }

        printf("Recieving from server......(%d)",getpid());
        rec_val = msgrcv(msgq_id_down, &message_recieve, sizeof(message_recieve.mtext), getpid()%10000,!IPC_NOWAIT );
        if(rec_val == -1)
        {
            printf("Error in recieving ! (%d)",getpid());
            perror("");
        }

        printf("Output After Conversion >>>> %s\n",message_recieve.mtext);

        
    }



    /* 0 -> receive all types of messages */
    // while(1){

    // IPC_NOWAIT: return -1 immediately if the queue is empty and the message can't be received
    // !IPC_NOWAIT : block until the message can be received
    // 0 : ignore
    // MSG_NOERROR : truncate the message if it is too long to fit in the queue (default: return -1 and the msg is not received and stays in queue)


}