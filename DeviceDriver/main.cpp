#ifndef MY_DEVICE_DRIVER_H
#define MY_DEVICE_DRIVER_H

#include <linux/module.h>   // 包含Linux模块基本功能
#include <linux/kernel.h>   // 包含内核打印函数等
#include <linux/fs.h>       // 包含文件操作相关的函数和结构

// 设备结构体，用于存储设备的状态和信息
struct my_device {
    int some_data; // 示例数据
    // 其他设备特定的数据
};

// 设备文件操作结构体，定义了可以对设备执行的操作
static const struct file_operations my_device_fops = {
    .owner = THIS_MODULE,
    .open = my_device_open,      // 打开设备时的操作
    .release = my_device_release, // 关闭设备时的操作
    .read = my_device_read,      // 读取设备时的操作
    .write = my_device_write,    // 写入设备时的操作
    // 其他可能的操作，如ioctl等
};

// 设备驱动程序的初始化函数
static int __init my_device_init(void) {
    int result;
    // 注册设备，分配主设备号等
    result = register_chrdev(0, "my_device", &my_device_fops);
    if (result < 0) {
        printk(KERN_ALERT "Failed to register character device\n");
        return result;
    }
    // 其他初始化操作，如分配内存、初始化设备等
    return 0;
}

// 设备驱动程序的清理函数
static void __exit my_device_exit(void) {
    // 注销设备，释放资源等
    unregister_chrdev(MAJOR(my_device_num), "my_device");
    // 其他清理操作
}

// 模块加载和卸载的宏定义
module_init(my_device_init);
module_exit(my_device_exit);

MODULE_LICENSE("GPL"); // 声明模块许可证为GPL
#endif MY_DEVICE_DRIVER_H

int main() {
    return 0;
}
