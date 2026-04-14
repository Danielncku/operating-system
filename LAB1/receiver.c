#include "receiver.h"
sem_t *sender_sem, *receiver_sem;
void receive(message_t* message_ptr, mailbox_t* mailbox_ptr){
    /*  TODO: 
        1. Use flag to determine the communication method
        2. According to the communication method, receive the message
    */
    
    // (1) 判斷模式
    if (mailbox_ptr->flag == MSG_PASSING) {
        // 從 message queue 接收
        msgrcv(mailbox_ptr->storage.msqid, message_ptr, 
               sizeof(message_ptr->msgText), 1, 0);
    } 
    else if (mailbox_ptr->flag == SHARED_MEM) {
        // 從 shared memory 讀取
        strcpy(message_ptr->msgText, mailbox_ptr->storage.shm_addr);
    }
}

int main(int argc, char* argv[]){
    /*  TODO: 
        1) Call receive(&message, &mailbox) according to the flow in slide 4
        2) Measure the total receiving time
        3) Get the mechanism from command line arguments
            • e.g. ./receiver 1
        4) Print information on the console according to the output format
        5) If the exit message is received, print the total receiving time and terminate the receiver.c
    */
    // 1. 解析參數
    int mechanism = atoi(argv[1]);
    
    // 2. 初始化 mailbox（連接到 sender 建立的資源）
    mailbox_t mailbox;
    mailbox.flag = mechanism;
    
    if (mechanism == MSG_PASSING) 
        printf("\033[36mMessage Passing\033[0m\n");
    else if (mechanism == SHARED_MEM) 
        printf("\033[36mShared Memory\033[0m\n");
        
    
    // 連接資源
    
    key_t key = 12345;
    if (mechanism == MSG_PASSING) {
        mailbox.storage.msqid = msgget(key, 0666 | IPC_CREAT);
        if (mailbox.storage.msqid == -1) {
            perror("msgget");
            return 1;
        }
    } else {
        int shmid = shmget(key, 1024, 0666 );
        if (shmid == -1) {
            perror("shmget");
            return 1;
        }
        mailbox.storage.shm_addr = (char*)shmat(shmid, NULL, 0);
        if (mailbox.storage.shm_addr == (char*)-1) {
            perror("shmat");
            return 1;
        }
    }
    
    // 建立 semaphores
    //sem_unlink("/sender_sem");
    //sem_unlink("/receiver_sem");
    
    sender_sem = sem_open("/sender_sem", O_CREAT, 0666, 0);
    receiver_sem = sem_open("/receiver_sem", O_CREAT, 0666, 0);
    
    if (sender_sem == SEM_FAILED || receiver_sem == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }
    
    printf("Receiver ready, waiting for messages...\n");
    
    // 計時
    struct timespec start, end;
    double total_time = 0;
    int count = 0;
    
    // 接收訊息迴圈
    while (1) {
        // 等待 sender
        sem_wait(receiver_sem);
        
        message_t msg;
        
        // 計時開始
        clock_gettime(CLOCK_MONOTONIC, &start);
        
        receive(&msg, &mailbox);
        
        // 計時結束
        clock_gettime(CLOCK_MONOTONIC, &end);
        
        total_time += (end.tv_sec - start.tv_sec) + 
                      (end.tv_nsec - start.tv_nsec) / 1e9;
        
        // 檢查結束
        if (strcmp(msg.msgText, "exit") == 0) {
            printf("Receive exit message\n");
            sem_post(sender_sem);
            break;
        }
        
        printf("Receive message: %s\n", msg.msgText);
        count++;
        
        // 通知 sender
        sem_post(sender_sem);
    }
    
    printf("Total message received: %d\n", count);
    printf("Total receiving time: %.6f seconds\n", total_time);
    
    // 清理
    sem_close(sender_sem);
    sem_close(receiver_sem);
    sem_unlink("/sender_sem");
    sem_unlink("/receiver_sem");
    
    if (mechanism == MSG_PASSING) {
        msgctl(mailbox.storage.msqid, IPC_RMID, NULL);
    } else {
        shmdt(mailbox.storage.shm_addr);
        key_t key = ftok(".", 'B');
        int shmid = shmget(key, 1024, 0666);
        shmctl(shmid, IPC_RMID, NULL);
    }
    
    return 0;
}
