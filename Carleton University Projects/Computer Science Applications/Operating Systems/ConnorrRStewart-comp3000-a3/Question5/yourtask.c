/* Code derived from:
  https://appusajeev.wordpress.com/2011/06/18/writing-a-linux-character-device-driver/
  and
  http://pete.akeo.ie/2011/08/writing-linux-device-driver-for-kernels.html
*/

#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <asm/uaccess.h>


#define dbg(format, arg...) do { if (debug) pr_info(CLASS_NAME ": %s: " format, __FUNCTION__, ## arg); } while (0)
#define err(format, arg...) pr_err(CLASS_NAME ": " format, ## arg)
#define info(format, arg...) pr_info(CLASS_NAME ": " format, ## arg)
#define warn(format, arg...) pr_warn(CLASS_NAME ": " format, ## arg)


#define DEVICE_NAME "yourtask"
#define CLASS_NAME "comp3000"

static struct class* newgetpid_class = NULL;
static struct device* newgetpid_device = NULL;
static int newgetpid_major;
int global_pid = -1;

static int newgetpid_open(struct inode *the_inode, struct file *f)
{
        return 0;
}

static ssize_t newgetpid_write(struct file *f, const char *buf, size_t len, loff_t *offset) {
	int response, ret;

	ret = kstrtoint_from_user(buf, len, 0, &response);

	global_pid = response;

	return len;
}

static ssize_t newgetpid_read(struct file *f, char *buf, size_t len, loff_t *offset)
{
        size_t i, msglen;
        pid_t thepid;

		int uid, euid, ppid, peuid, taskID;

        char message[255];
		char addition[60];

        if (*offset > 0) {
                return 0;
        }

        thepid = task_tgid_vnr(current);
		uid = thepid;
		euid = (current->cred->euid).val;

		ppid = current->parent->pid;
		peuid = (current->parent->cred->euid).val;
		
		if (global_pid == -1) {
			snprintf(addition, 60, "ERROR: no PID value has been written to /dev/yourtask");
		}
		else if (global_pid == ppid && ppid == peuid) { //1
			snprintf(addition, 60, "Task %d is yours!", global_pid);
		}
		else if (global_pid == euid && euid == peuid) { //4
			snprintf(addition, 60, "Task %d is %d's but acts as you!", global_pid, uid);
		}
		else if (global_pid == euid) { //2
			snprintf(addition, 60, "Task %d belongs to uid %d!", global_pid, uid);
		}
		else if (global_pid == ppid && peuid != ppid) { //3
			snprintf(addition, 60, "Task %d is yours but acts as %d!", global_pid, peuid);
		}
		else {
			snprintf(addition, 60, "ERROR: Unknown PID");
		}

        snprintf(message, 255, "uid: %d; euid: %d; ppud: %d; peuid: %d; message: %d\n%s\n", thepid, euid, ppid, peuid, global_pid, addition);

        msglen = strlen(message);

        if (len < msglen) {
                msglen = len;
        }

        for (i = 0; i < msglen; i++) {
                put_user(message[i], buf++);
        }

        *offset = i;

        return i;
}

static int newgetpid_release(struct inode *the_inode, struct file *f)
{
        printk(KERN_ALERT "device closed\n");
        return 0;
}


static struct file_operations newgetpid_fops = {
        .open = newgetpid_open,
        .read = newgetpid_read,
        .release = newgetpid_release,
	.write = newgetpid_write,
};


static char *newgetpid_devnode(struct device *dev, umode_t *mode)
{
        if (mode)
	        *mode = 0666;
        return NULL;
}

static int __init newgetpid_init(void)
{
        int retval;

        newgetpid_major = register_chrdev(0, DEVICE_NAME, &newgetpid_fops);
        if (newgetpid_major < 0) {
                err("failed to register device: error %d\n", newgetpid_major);
                retval = newgetpid_major;
                goto failed_chrdevreg;
        }

        newgetpid_class = class_create(THIS_MODULE, CLASS_NAME);
        if (IS_ERR(newgetpid_class)) {
                err("failed to register device class '%s'\n", CLASS_NAME);
                retval = PTR_ERR(newgetpid_class);
                goto failed_classreg;
        }

	newgetpid_class->devnode = newgetpid_devnode;

        newgetpid_device = device_create(newgetpid_class, NULL, MKDEV(newgetpid_major, 0),
                                    NULL, DEVICE_NAME);

        if (IS_ERR(newgetpid_device)) {
                err("failed to create device '%s'\n", DEVICE_NAME);
                retval = PTR_ERR(newgetpid_device);
                goto failed_devreg;
        }

        return 0;

 failed_devreg:
        class_unregister(newgetpid_class);
        class_destroy(newgetpid_class);
 failed_classreg:
        unregister_chrdev(newgetpid_major, DEVICE_NAME);
 failed_chrdevreg:
        return -1;
}

static void __exit newgetpid_exit(void)
{
        device_destroy(newgetpid_class, MKDEV(newgetpid_major, 0));
        class_unregister(newgetpid_class);
        class_destroy(newgetpid_class);
        unregister_chrdev(newgetpid_major, "yourtask");
        info("Unloading module.\n");
        return;
}

module_init(newgetpid_init);
module_exit(newgetpid_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anil Somayaji <soma@scs.carleton.ca>");
MODULE_DESCRIPTION("A write newgetpid character device module");
