#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <asm/current.h>

#define procfs_name "Mythread_info"
#define BUFSIZE  1024
static char write_buf[BUFSIZE];  // 存使用者 write 的字串
static char read_buf[BUFSIZE];   // 組合 read 要輸出的內容
static pid_t target_pid = -1;


static ssize_t Mywrite(struct file *fileptr, const char __user *ubuf, size_t buffer_len, loff_t *offset){
    /*Your code here*/
    size_t n = buffer_len;
    if (n >= BUFSIZE) n = BUFSIZE - 1;

    if (copy_from_user(write_buf, ubuf, n))
        return -EFAULT;

    write_buf[n] = '\0';
    return buffer_len;
    /****************/
}


static ssize_t Myread(struct file *fileptr,
                      char __user *ubuf,
                      size_t buffer_len,
                      loff_t *offset)
{
    int len = 0;
    unsigned long long time_ms;

    if (*offset > 0)
        return 0;

    time_ms = (unsigned long long)(current->utime) / 100 / 1000;

    len = snprintf(read_buf, BUFSIZE,
        "String: %s\n"
        "Process PID: %d\n"
        "Thread TID: %d\n"
        "Time(ms): %llu\n",
        write_buf,
        current->tgid,
        current->pid,
        time_ms);

    if (copy_to_user(ubuf, read_buf, len))
        return -EFAULT;

    *offset = len;
    return len;
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
