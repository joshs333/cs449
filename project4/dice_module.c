/**
 * @file dice_module.c
 * @brief generates die rolls
 **/
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/random.h>
#include <asm/uaccess.h>

static unsigned int current_faces = 6;

static unsigned char get_random_byte(int max) {
    unsigned char c;
    get_random_bytes(&c, 1);
    return (c % max) + 1;
}

static ssize_t dice_read(struct file * file, char * buf, size_t count, loff_t *ppos) {
    count = (count > 512) ? 512 : count;
    char my_buffer[count];
    int i = 0;
    while(i < count) {
        my_buffer[i] = get_random_byte(current_faces);
        ++i;
    }
    if( copy_to_user(buf, my_buffer, count) != 0) {
        printk(KERN_ERR "Error sending die rolls, read(file, 0x%p, %d, %lld)\n", buf, count, *ppos);
        return -1;
    }
    *ppos = count;
    return count;
}

static ssize_t dice_write(struct file * file, const char * buf, size_t count, loff_t *ppos) {
    unsigned char my_buffer[count];
    if(copy_from_user(my_buffer, buf, count) != 0) {
        printk(KERN_ERR "Error in copying from user, write(file, 0x%p, %d, %lld)\n", buf, count, *ppos);
        return -1;
    }
    int i = 0;
    while(i < count) {
        current_faces = my_buffer[i];
        ++i;
    }
    return count;
}

static const struct file_operations dice_fops = {
    .owner  = THIS_MODULE,
    .read   = dice_read,
    .write  = dice_write,
};

static struct miscdevice dice_device = {
    .minor  = MISC_DYNAMIC_MINOR, // kernel picks minor number
    .name   = "dice", // /dev/dice
    .fops   = &dice_fops,
    .mode   = 0666,
};

static int __init dice_init(void) {
    int ret = misc_register(&dice_device);
    if(ret) {
        printk(KERN_ERR "Unable to add device.");
    } else {
        printk(KERN_ERR "Created device!");
    }
    return ret;
}

static void __exit dice_exit(void) {
    misc_deregister(&dice_device);
}

module_init(dice_init);
module_exit(dice_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Joshua Spisak <joshs333@live.com>");
MODULE_DESCRIPTION("Dice rolling module for CS 449 project 4.");
MODULE_VERSION("dev");
