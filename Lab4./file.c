#include <linux/fs.h>
#include <linux/uaccess.h>
#include "osfs.h"

/**
 * Function: osfs_read
 * Description: Reads data from a file.
 * Inputs:
 *   - filp: The file pointer representing the file to read from.
 *   - buf: The user-space buffer to copy the data into.
 *   - len: The number of bytes to read.
 *   - ppos: The file position pointer.
 * Returns:
 *   - The number of bytes read on success.
 *   - 0 if the end of the file is reached.
 *   - -EFAULT if copying data to user space fails.
 */
static ssize_t osfs_read(struct file *filp, char __user *buf, size_t len, loff_t *ppos)
{
    struct inode *inode = file_inode(filp);
    struct osfs_inode *osfs_inode = inode->i_private;
    struct osfs_sb_info *sb_info = inode->i_sb->s_fs_info;
    ssize_t bytes_read = 0;

    // 1. 基本檢查：如果檔案沒分配區塊或讀取位置已到底，直接回傳 0
    if (osfs_inode->i_blocks == 0 || *ppos >= osfs_inode->i_size)
        return 0;

    // 2. 限制讀取長度不能超過檔案實際大小
    if (*ppos + len > osfs_inode->i_size)
        len = osfs_inode->i_size - *ppos;

    // 3. 跨區塊循環讀取 (與 Write 邏輯對應)
    while (len > 0) {
        // A. 計算邏輯區塊與塊內偏移
        uint32_t logical_block = *ppos / BLOCK_SIZE;
        uint32_t offset_in_block = *ppos % BLOCK_SIZE;

        // B. 定位實體區塊編號 (起始塊 + 邏輯位移)
        // 注意：這裡要用 i_block 作為起始點
        uint32_t phys_block = osfs_inode->i_block + logical_block;
        void *data_ptr = sb_info->data_blocks + phys_block * BLOCK_SIZE + offset_in_block;

        // C. 計算本次能讀多少 (不能跨越塊邊界)
        size_t space_in_block = BLOCK_SIZE - offset_in_block;
        size_t chunk = (len < space_in_block) ? len : space_in_block;

        // D. 搬運資料到 User Space (使用 copy_to_user)
        if (copy_to_user(buf + bytes_read, data_ptr, chunk)) {
            return -EFAULT;
        }

        // E. 更新狀態
        bytes_read += chunk;
        *ppos += chunk;
        len -= chunk;
    }

    return bytes_read;
}
/**
 * Function: osfs_write
 * Description: Writes data to a file.
 * Inputs:
 *   - filp: The file pointer representing the file to write to.
 *   - buf: The user-space buffer containing the data to write.
 *   - len: The number of bytes to write.
 *   - ppos: The file position pointer.
 * Returns:
 *   - The number of bytes written on success.
 *   - -EFAULT if copying data from user space fails.
 *   - Adjusted length if the write exceeds the block size.
 */
static ssize_t osfs_write(struct file *filp, const char __user *buf, size_t len, loff_t *ppos)
{   
    //Step1: Retrieve the inode and filesystem information
    struct inode *inode = file_inode(filp);
    struct osfs_inode *osfs_inode = inode->i_private;
    struct osfs_sb_info *sb_info = inode->i_sb->s_fs_info;
    void *data_block;
    ssize_t bytes_written;
    int ret;

    // Step2: Check if a data block has been allocated; if not, allocate one
    if (osfs_inode->i_blocks == 0) {
        ret = osfs_alloc_data_block(sb_info, &osfs_inode->i_block);
        if (ret) {
            return ret; 
        }
        osfs_inode->i_blocks = 1;
    }

    // Step3: Limit the write length to fit within one data block
    size_t max_allowed_size = osfs_inode->i_blocks * BLOCK_SIZE;
    if (*ppos >= max_allowed_size) {
        return -ENOSPC; // 偏移量已超過分配的總區塊範圍
    }
    if (*ppos + len > max_allowed_size) {
        len = max_allowed_size - *ppos;
    }

    // Step4: Write data from user space to the data block
    while (len > 0) {
        // 計算目前 ppos 對應的 邏輯區塊索引 與 區塊內位移
        uint32_t current_block_idx = *ppos / BLOCK_SIZE;
        uint32_t offset_in_block = *ppos % BLOCK_SIZE;
        
        // 找出對應的實體記憶體位址
        // 實體區塊編號 = 起始塊編號 + 第幾塊
        uint32_t phys_block_no = osfs_inode->i_block + current_block_idx;
        void *target_ptr = sb_info->data_blocks + phys_block_no * BLOCK_SIZE + offset_in_block;

        // 計算本次循環要在這個 block 寫入多少 (不能超過當前 block 邊界)
        size_t space_left_in_block = BLOCK_SIZE - offset_in_block;
        size_t chunk_to_write = (len < space_left_in_block) ? len : space_left_in_block;

        // 從使用者空間複製資料
        if (copy_from_user(target_ptr, buf + bytes_written, chunk_to_write)) {
            return -EFAULT;
        }

        // 更新狀態指標
        bytes_written += chunk_to_write;
        *ppos += chunk_to_write;
        len -= chunk_to_write;
    }

    // Step5: Update inode & osfs_inode attribute
    if (*ppos > osfs_inode->i_size) {
        osfs_inode->i_size = *ppos;
        inode->i_size = osfs_inode->i_size;
    }

    struct timespec64 now = current_time(inode);
    inode_set_mtime_to_ts(inode, now);
    inode_set_ctime_to_ts(inode, now);
    osfs_inode->__i_mtime = now;
    osfs_inode->__i_ctime = now;
    
    mark_inode_dirty(inode);
    
    // 如果寫入後的位置超過原檔案大小，更新大小
    if (*ppos > osfs_inode->i_size) {
        osfs_inode->i_size = *ppos;
        inode->i_size = osfs_inode->i_size;
    }

    // 更新時間戳記
    struct timespec64 now = current_time(inode);
    inode_set_mtime_to_ts(inode, now);
    inode_set_ctime_to_ts(inode, now);
    
    osfs_inode->__i_mtime = now;
    osfs_inode->__i_ctime = now;
    // ----------------------------
    
    mark_inode_dirty(inode);

    // Step6: Return the number of bytes written
    
    
    return bytes_written;
}

/**
 * Struct: osfs_file_operations
 * Description: Defines the file operations for regular files in osfs.
 */
const struct file_operations osfs_file_operations = {
    .open = generic_file_open, // Use generic open or implement osfs_open if needed
    .read = osfs_read,
    .write = osfs_write,
    .llseek = default_llseek,
    // Add other operations as needed
};

/**
 * Struct: osfs_file_inode_operations
 * Description: Defines the inode operations for regular files in osfs.
 * Note: Add additional operations such as getattr as needed.
 */
const struct inode_operations osfs_file_inode_operations = {
    // Add inode operations here, e.g., .getattr = osfs_getattr,
};




// 在 file.c 中加入
static const char *osfs_get_link(struct dentry *dentry, struct inode *inode, struct delayed_call *done)
{
    struct osfs_inode *osfs_inode = inode->i_private;
    struct osfs_sb_info *sb_info = inode->i_sb->s_fs_info;

    if (!dentry)
        return ERR_PTR(-ECHILD);

    // 從該 Inode 的資料區塊中讀取儲存的路徑字串
    return (char *)(sb_info->data_blocks + osfs_inode->i_block * BLOCK_SIZE);
}

// 定義連結檔專用的 inode 操作表
const struct inode_operations osfs_symlink_inode_operations = {
    .get_link = osfs_get_link,
};
