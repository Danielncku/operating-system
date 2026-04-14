#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "../include/command.h"
#include "../include/builtin.h"

// ======================= requirement 2.3 =======================
/**
 * @brief 
 * Redirect command's stdin and stdout to the specified file descriptor
 * If you want to implement ( < , > ), use "in_file" and "out_file" included the cmd_node structure
 * If you want to implement ( | ), use "in" and "out" included the cmd_node structure.
 *
 * @param p cmd_node structure
 * 
 */
void redirection(struct cmd_node *p){
	int fd; //open 的檔案編號

    // 處理 input redirection:  < infile
    if (p->in_file != NULL) {
        fd = open(p->in_file, O_RDONLY);
        if (fd < 0) {
            perror("open infile");
            exit(1);
        }
        dup2(fd, STDIN_FILENO);   // stdin = infile（原本是鍵盤）
        close(fd);
    }

    // 處理 output redirection: > outfile
    if (p->out_file != NULL) {
        fd = open(p->out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            perror("open outfile");
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);  // stdout = outfile
        close(fd);
    }
}
// ===============================================================

// ======================= requirement 2.2 =======================
/**
 * @brief 
 * Execute external command
 * The external command is mainly divided into the following two steps:
 * 1. Call "fork()" to create child process
 * 2. Call "execvp()" to execute the corresponding executable file
 * @param p cmd_node structure
 * @return int 
 * Return execution status
 */
int spawn_proc(struct cmd_node *p)
{
  	//fork a child process
	pid_t pid = fork();
	// error check
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    // ---------- child ----------
    if (pid == 0) {
        // 處理 < 與 >
        redirection(p);

        // 執行外部指令
        execvp(p->args[0], p->args); /‌/ p指向下一個args

        // execvp 失敗才會跑到這裡
        perror("execvp");
        exit(1);
    }

    // ---------- parent ----------
    waitpid(pid, NULL, 0);
    return 1;       // 1 = shell 繼續跑
}
// ===============================================================


// ======================= requirement 2.4 =======================
/**
 * @brief 
 * Use "pipe()" to create a communication bridge between processes
 * Call "spawn_proc()" in order according to the number of cmd_node
 * @param cmd Command structure  
 * @return int
 * Return execution status 
 */
int fork_cmd_node(struct cmd *cmd)
{
	struct cmd_node *curr = cmd->head;
    int in_fd = STDIN_FILENO; // 初始 input 來自鍵盤
    int fd[2];
    pid_t pid;
    int status;

    while (curr != NULL) {
        // 不是最後一個 → 建 pipe
        if (curr->next != NULL) {
            if (pipe(fd) < 0) {
                perror("pipe");
                exit(1);
            }
        }

        // fork child
        pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        }

        // ------------------- child process -------------------
        if (pid == 0) {

            // 1. stdin 來自上一個 pipe 的讀端
            if (in_fd != STDIN_FILENO) {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }

            // 2. 如果不是最後一個 → stdout 指向 pipe 的寫端
            if (curr->next != NULL) {
                dup2(fd[1], STDOUT_FILENO);
                close(fd[0]);
                close(fd[1]);
            }

            // 3. 處理 < >
            redirection(curr);

            // 4. 執行外部指令
            execvp(curr->args[0], curr->args);
            perror("execvp");
            exit(1);
        }

        // ------------------- parent process -------------------
        // parent 不用寫入端 → 關掉
        if (curr->next != NULL) {
            close(fd[1]);
        }

        // 下一個指令的 stdin 來自 pipe 的讀端
        if (in_fd != STDIN_FILENO)
            close(in_fd);

        in_fd = (curr->next != NULL) ? fd[0] : STDIN_FILENO;

        curr = curr->next;
    }

    // 等所有 child 結束
    while (wait(&status) > 0);

    return 1;
}
// ===============================================================


void shell()
{
	while (1) {
		printf(">>> $ ");
		char *buffer = read_line();
		if (buffer == NULL)
			continue;

		struct cmd *cmd = split_line(buffer);
		
		int status = -1;
		// only a single command
		struct cmd_node *temp = cmd->head;
		
		if(temp->next == NULL){
			status = searchBuiltInCommand(temp);
			if (status != -1){
				int in = dup(STDIN_FILENO), out = dup(STDOUT_FILENO);
				if( in == -1 | out == -1)
					perror("dup");
				redirection(temp);
				status = execBuiltInCommand(status,temp);

				// recover shell stdin and stdout
				if (temp->in_file)  dup2(in, 0);
				if (temp->out_file){
					dup2(out, 1);
				}
				close(in);
				close(out);
			}
			else{
				//external command
				status = spawn_proc(cmd->head);
			}
		}
		// There are multiple commands ( | )
		else{
			
			status = fork_cmd_node(cmd);
		}
		// free space
		while (cmd->head) {
			
			struct cmd_node *temp = cmd->head;
      		cmd->head = cmd->head->next;
			free(temp->args);
   	    	free(temp);
   		}
		free(cmd);
		free(buffer);
		
		if (status == 0)
			break;
	}
}
