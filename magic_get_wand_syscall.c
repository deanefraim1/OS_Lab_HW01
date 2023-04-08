#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/list.h>

int magic_get_wand(int power, char secret[SECRET_MAXSIZE])
{
    struct task_struct *task;
    struct list_head *list;
    int i = 0;

    list_for_each(list, &current->children) {
        task = list_entry(list, struct task_struct, sibling);
        if (task->pid == power) {
            for (i = 0; i < SECRET_MAXSIZE; i++) {
                secret[i] = task->secret[i];
            }
            return 0;
        }
    }
    return -1;
}
