#include "sender.h"
sem_t *sender_sem, *receiver_sem;

void send(message_t message, mailbox_t* mailbox_ptr){
    /*  TODO: 
        1. Use flag to determine the communication method
        2. According to the communication method, send the message
    */
    if (mailbox_ptr->flag == MSG_PASSING) {
        // Message Passing
        msgsnd(mailbox_ptr->storage.msqid, &message, sizeof(message.msgText), 0);
        
    } 
    else if (mailbox_ptr->flag == SHARED_MEM) {
        // Shared Memory
        strcpy(mailbox_ptr->storage.shm_addr, message.msgText);
        
    }
    
    
    
    
}

int main(int argc, char* argv[]){
    /*  TODO: 
        1) Call send(message, &mailbox) according to the flow in slide 4
        2) Measure the total sending time
        3) Get the mechanism and the input file from command line arguments
            • e.g. ./sender 1 input.txt
                    (1 for Message Passing, 2 for Shared Memory)
        4) Get the messages to be sent from the input file
        5) Print information on the console according to the output format
        6) If the message form the input file is EOF, send an exit message to the receiver.c
        7) Print the total sending time and terminate the sender.c
    */
    // 1. 解析命令列參數
    
    int mechanism = atoi(argv[1]);  // 1 或 2
    char* filename = argv[2];        // input.txt
    
    // 2. 初始化 mailbox
    mailbox_t mailbox;
    mailbox.flag = mechanism;
    
    if (mechanism == MSG_PASSING) 
        printf("\033[36mMessage Passing\033[0m\n");
    else if (mechanism == SHARED_MEM) 
        printf("\033[36mShared Memory\033[0m\n");
    
    // 建立資源
    
    key_t key = 12345;
    if (mechanism == MSG_PASSING) {
        mailbox.storage.msqid = msgget(key, 0666 | IPC_CREAT);
        if (mailbox.storage.msqid == -1) {
            perror("msgget");
            return 1;
        }
    } else {
        int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
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
    
    // 開啟 semaphores（receiver 應該已經建立）
    sender_sem = sem_open("/sender_sem", 0);
    receiver_sem = sem_open("/receiver_sem", 0);
    
    if (sender_sem == SEM_FAILED || receiver_sem == SEM_FAILED) {
        printf("Error: Please start receiver first!\n");
        return 1;
    }
    
    // 開啟檔案
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("fopen");
        return 1;
    }
    
    // 計時
    struct timespec start, end;
    double total_time = 0;
    
    char buffer[1024];
    int count = 0;
    
    // 讀取並發送訊息
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // 移除換行符
        buffer[strcspn(buffer, "\n")] = 0;
        
        message_t msg;
        msg.mType = 1;
        strcpy(msg.msgText, buffer);
        
        // 計時開始
        clock_gettime(CLOCK_MONOTONIC, &start);
        
        send(msg, &mailbox);
        
        
        
        // 計時結束
        clock_gettime(CLOCK_MONOTONIC, &end);
        
        total_time += (end.tv_sec - start.tv_sec) + 
                      (end.tv_nsec - start.tv_nsec) / 1e9;
                      
        sem_post(receiver_sem);  //  通知 receiver
        
        printf("Send message: %s\n", msg.msgText);
        count++;
        // 等待 receiver 處理
        sem_wait(sender_sem);
    }
    
    // 發送結束訊息
    message_t exit_msg;
    exit_msg.mType = 1;
    strcpy(exit_msg.msgText, "exit");
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    send(exit_msg, &mailbox);
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    total_time += (end.tv_sec - start.tv_sec) + 
                  (end.tv_nsec - start.tv_nsec) / 1e9;
                
    sem_post(receiver_sem);
    sem_wait(sender_sem);
    
    
    printf("Total message sent: %d\n", count);
    printf("Total sending time: %.6f seconds\n", total_time);
    
    // 清理
    fclose(fp);
    sem_close(sender_sem);
    sem_close(receiver_sem);
    
    if (mechanism == MSG_PASSING) {
        msgctl(mailbox.storage.msqid, IPC_RMID, NULL);
    } else {
        shmdt(mailbox.storage.shm_addr);
    }
    
    return 0;
    
}
