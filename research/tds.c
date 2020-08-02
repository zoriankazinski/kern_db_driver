#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/stat.h>

/* Networking */
// this is needed to create a kernel level tds protocol functions...
//#include <linux/net.h>
//#include <net/sock.h>
//#include <linux/tcp.h>
//#include <linux/socket.h>
//#include <linux/in.h>
//#include <linux/file.h>

// device_create wont run on any other license other than GPL.... damn.
MODULE_LICENSE("GPL");

struct class *cl;       // device class under /sys
struct cdev devices;    // <-- add a read on config to check how many files needed.
dev_t dev_num;		// current device number
dev_t curr_dev;

// ########################################
static int device_open(struct inode *inode, struct file *filp)
{
	return 0;
	//return -EBUSY;
}

static int device_release(struct inode *inode, struct file *flip)
{
	return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
	return 0;
}

static ssize_t device_write(struct file *filp, const char *buf, size_t len, loff_t *off)
{
	return -1;
}

static struct file_operations fops = {
        .read = device_read,
        .write = device_write,
        .open = device_open,
        .release = device_release
};
// ########################################

/*

// -- move charecter device file creation to another function --
// TODO: UNTESTED - TEST.
int create_device(class cls, c_dev device_list, dev_t current_device, dev_t device_number)
{
  alloc_chrdev_region(&device_number,0,1,"tds");
  cls = class_create(THIS_MODULE,"tds_file");
  if (IS_ERR(cls))
  {
    class_destroy(cls);
    unregister_chrdev_region(device_number,1);
    return -1;
  }
  cdev_init(&device_list,&fops);
  current_device = MKDEV(MAJOR(device_number),MINOR(device_number)+0);
  device_create(cls,NULL,current_device,NULL,"tds_file%d",0);
  cdev_add(&device_list,current_device,1);
  return 0;

	
}

// -- read file of unknown size on fs (for needed devices)
// TODO: UNTESTED - TEST.
// probably doesnt work, needs debug...
static char *load_file(char* filename, int *input_size)
{
  struct kstat *stat;
  struct file *fp;
  mm_segment_t fs;
  loff_t pos = 0;
  char *buf;

  fp = filp_open(filename,O_RDWR,0655);
  if (IS_ERR(fp)) {
	//handle file open issue.
	return ERR_PTR(-ENOENT);
  }
  fs = get_fs();
  set_fs(KERNEL_DS);
  stat = (struct kstat *) kmalloc(sizeof(struct kstat), GFP_KERNEL);
  if (!stat)
	  return ERR_PTR(-ENOMEM);
  vfs_stat(filename,stat);
  *input_size = stat->size;
  buf = kmalloc(*input_size,GFP_KERNEL);
  if (!buf) {
    kfree(stat);
    return ERR_PTR(-ENOMEM);
  }
  kernel_read(fp,buf,*input_size,&pos);
  filp_close(fp,NULL);
  set_fp(fs);
  kfree(stat);
  return buf;
}
*/

static int tds_init(void)
{
	alloc_chrdev_region(&dev_num,0,1,"tds");
	cl = class_create(THIS_MODULE,"tds_file");
	if (IS_ERR(cl)) {
		// device_destroy(cl,dev_num);
		class_destroy(cl);
        	unregister_chrdev_region(dev_num,1);
		return -1;
	}
	cdev_init(&devices,&fops);
	curr_dev = MKDEV(MAJOR(dev_num),MINOR(dev_num)+0);
	device_create(cl,NULL,curr_dev,NULL,"tds_file%d",0);
	cdev_add(&devices,curr_dev,1);
	return 0;
}

static void tds_exit(void)
{
	device_destroy(cl,dev_num);
	class_destroy(cl);
	unregister_chrdev_region(dev_num,1);
}

module_init(tds_init);
module_exit(tds_exit);
