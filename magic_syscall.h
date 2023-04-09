#include <linux/sched.h>
#include <linux/list.h>

#define SECRET_MAXSIZE 128
int magic_get_wand(int power, char secret[SECRET_MAXSIZE]);
int magic_attack(pid_t pid);
int magic_legilimens(pid_t pid);
int magic_list_secrets(char secrets[][SECRET_MAXSIZE], size_t size);

struct wand_struct
{
    int power;
    int health;
    char secret[SECRET_MAXSIZE];
    struct list_head* stolen_secrets;
};

int magic_get_wand(int power, char secret[SECRET_MAXSIZE])
{
    struct task_struct *p = current;
    list_entery(p->wand, struct wand_struct, health);

}

int magic_attack(pid_t pid)
{

}

int magic_legilimens(pid_t pid)
{
    
}

int magic_list_secrets(char secrets[][SECRET_MAXSIZE], size_t size)
{
    
}