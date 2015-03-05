/*
 * Programming assignment at the end of chapter 02
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/slab.h>

struct birthday {
    int day;
    int month;
    int year;
    struct list_head list;
};

static LIST_HEAD(birthday_list);

int simple_init(void) {
    struct birthday *person;
    int i;

    printk(KERN_INFO "Loading Module\n");

    for(i = 0; i < 5; i++) {
        person = kmalloc(sizeof(*person), GFP_KERNEL);
        person->day = 2 + i;
        person->month = 8;
        person->year = 1995;
        INIT_LIST_HEAD(&person->list);
        list_add_tail(&person->list, &birthday_list);
    }

    return 0;
}

void simple_exit(void) {
    struct birthday *ptr, *next;

    printk(KERN_INFO "Removing Module\n");

    list_for_each_entry_safe(ptr, next, &birthday_list, list) {
        printk(KERN_INFO "Removing birthday %d-%d-%d\n", ptr->year, ptr->month, ptr->day);
        list_del(&ptr->list);
        kfree(ptr);
    }

    printk(KERN_INFO "Done\n");
    return;
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
