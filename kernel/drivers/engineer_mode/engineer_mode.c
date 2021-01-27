/*
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/percpu.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <linux/syscalls.h>
#include <linux/mount.h>
#include <linux/delay.h>

#define ENG_VENDOR_DEV "/dev/eng_vendor"
#define ENG_SYSTEM_DEV "/dev/eng_system"

#define ENG_VENDOR_MOUNT_POINT "/root/eng/vendor"
#define ENG_SYSTEM_MOUNT_POINT "/root/eng"
#define ENG_MODE_MAX_RETRY_TIMES 5

static int engineer_mode = 1;

static char __initdata saved_eng_vendor_name[64];
static char __initdata saved_eng_system_name[64];

static dev_t eng_vendor_dev;
static dev_t eng_system_dev;

static int engineer_mode_mount_flags = MS_RDONLY | MS_SILENT;
static char *engineer_mode_mount_data = NULL;

static inline int create_dev(char *name, dev_t dev)
{
	sys_unlink(name);
	return sys_mknod(name, S_IFBLK|0600, new_encode_dev(dev));
}

static int __init setup_eng_vendor(char *line)
{
	strlcpy(saved_eng_vendor_name, line, sizeof(saved_eng_vendor_name));
	return 1;
}
__setup("eng_vendor=", setup_eng_vendor);

static int __init setup_eng_system(char *line)
{
	strlcpy(saved_eng_system_name, line, sizeof(saved_eng_system_name));
	return 1;
}
__setup("eng_system=", setup_eng_system);

static int __init engineer_mode_init(void)
{
	int i = 0;
	for(i=0;i<ENG_MODE_MAX_RETRY_TIMES;i++){
		eng_vendor_dev = name_to_dev_t(saved_eng_vendor_name);
		if(eng_vendor_dev != 0){
			break;
		}
		msleep(100);
		printk(KERN_ERR"%s : init fail retry times %d\n", __FUNCTION__,i);
	}

	if (eng_vendor_dev == 0) {
		printk(KERN_ERR"%s : fail to find dev %s\n", __FUNCTION__,
			saved_eng_vendor_name);
		goto skip_engineer_mode;
	}

	eng_system_dev = name_to_dev_t(saved_eng_system_name);
	if (eng_system_dev == 0) {
		printk(KERN_ERR"%s : fail to find dev %s\n", __FUNCTION__,
			saved_eng_system_name);
		goto skip_engineer_mode;
	}
	goto success;

skip_engineer_mode:
	engineer_mode = 0;
success:
	return 0;
}
late_initcall(engineer_mode_init);

void engineer_mode_mount(void)
{
	int err;

	if (engineer_mode == 0)
		return;

	err = create_dev(ENG_VENDOR_DEV, eng_vendor_dev);
	if (err < 0) {
		printk(KERN_ERR"%s : fail to create /dev/eng_vendor err(%d)\n",
			__FUNCTION__, err);
		return;
	}

	err = create_dev(ENG_SYSTEM_DEV, eng_system_dev);
	if (err < 0) {
		printk(KERN_ERR"%s : fail to create /dev/eng_system err(%d)\n",
			__FUNCTION__, err);
		return;
	}

	err = sys_mount(ENG_SYSTEM_DEV, ENG_SYSTEM_MOUNT_POINT, "ext4",
			engineer_mode_mount_flags, engineer_mode_mount_data);
	if (err < 0) {
		printk(KERN_ERR"%s : fail to mount %s err(%d)\n", ENG_SYSTEM_DEV,
			__FUNCTION__, err);
		return;
	}

	err = sys_mount(ENG_VENDOR_DEV, ENG_VENDOR_MOUNT_POINT, "ext4",
			engineer_mode_mount_flags, engineer_mode_mount_data);
	if (err < 0) {
		printk(KERN_ERR"%s : fail to mount %s err(%d)\n", ENG_VENDOR_DEV,
			__FUNCTION__, err);
		return;
	}

	printk(KERN_INFO"%s : success to mount engineer mode partitions\n",
		__FUNCTION__);

	return;
}

MODULE_DESCRIPTION("Hisilicon Engineer Mode Driver");
MODULE_LICENSE("GPL V2");
