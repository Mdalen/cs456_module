#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/mm.h>
#include <linux/vmstat.h>

#define procfs_name "pagefaults"

/*
 * CS 456 Pagefault Module
 * This module, when loaded, will create a file called /proc/num_pagefaults
 * This file will return the number of page faults when it is read by a process
 *
 */

MODULE_LICENSE("GPL");

struct proc_dir_entry *Our_Proc_File;
long unsigned long *error_num;

int  procfile_read(char *buffer, char **buffer_location, 
        off_t offset, int buffer_length, int *eof, void * data){
    
    int ret;
    

    printk(KERN_INFO "procfile_read (/proc/%s) called\n", procfs_name);
    
    all_vm_events(error_num);

    if (offset > 0) {
        ret = 0;
    }else{
        ret = sprintf(buffer, "%lu Page Fault Errors\n", error_num[PGFAULT]);
    }
    return ret;
}

int init_module(void){
    
    Our_Proc_File = create_proc_entry(procfs_name, 0644, NULL);
    
    error_num = kmalloc(NR_VM_ZONE_STAT_ITEMS * sizeof(unsigned long) + 
            sizeof(struct vm_event_state) + 100, GFP_KERNEL);


    if (Our_Proc_File == NULL) {
        remove_proc_entry(procfs_name, NULL);
        printk(KERN_ALERT "ERROR: Could not initialize /proc/%s\n", procfs_name);
        return -ENOMEM;
    }
    
    Our_Proc_File->read_proc     = procfile_read;
    //Our_Proc_File->owner         = THIS_MODULE;
    Our_Proc_File->mode          = S_IFREG | S_IRUGO;
    Our_Proc_File->uid           = 0;
    Our_Proc_File->gid           = 0;
    Our_Proc_File->size          = 37;

    printk(KERN_INFO "CS456 module loaded successfully and created proc/%s\n", procfs_name);
    return 0;
}


void cleanup_module(void){
    remove_proc_entry(procfs_name, NULL);
    printk(KERN_INFO"CS456 module unloaded and successfully removed /proc/%s\n", procfs_name);


}
