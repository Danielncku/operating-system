#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <asm/current.h>

#define procfs_name "Mythread_info"
#define BUFSIZE  1024
char buf[BUFSIZE];

static ssize_t Mywrite(struct file *fileptr, const char __user *ubuf, size_t buffer_len, loff_t *offset){
    /* Do nothing */
	return 0;
}


static ssize_t Myread(struct file *fileptr, char __user *ubuf, size_t buffer_len, loff_t *offset){
    /*Your code here*/
    int len = 0;
    struct task_struct *task;
    struct task_struct *thread;

    /* 只讓第一次 read 有內容（避免 cat 重複印） */
    if (*offset > 0)
        return 0;

    task = current;

    /* 印出 process PID */
    len += snprintf(buf + len, BUFSIZE - len,
                    "Process PID: %d\n", task->pid);

    /* 列出所有 threads */
    for_each_thread(task, thread) {
        len += snprintf(buf + len, BUFSIZE - len,
                        "Thread TID: %d | Priority: %d | State: %ld\n",
                        thread->pid,
                        thread->prio,
                        thread->__state);
    }

    /* 複製 kernel buffer 到 user space */
    if (copy_to_user(ubuf, buf, len))
        return -EFAULT;

    *offset = len;   // 更新 offset，表示已讀
    return len;      // 回傳實際讀取的長度
    /****************/
}

static struct proc_ops Myops = {
    .proc_read = Myread,
    .proc_write = Mywrite,
};

static int My_Kernel_Init(void){
    proc_create(procfs_name, 0644, NULL, &Myops);   
    pr_info("My kernel says Hi");
    return 0;
}

static void My_Kernel_Exit(void){
    pr_info("My kernel says GOODBYE");
}

module_init(My_Kernel_Init);
module_exit(My_Kernel_Exit);

MODULE_LICENSE("GPL");
