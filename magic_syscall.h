#include <linux/sched.h>
#include <linux/list.h>
#include <errno.h>
#include <linux/string.h>
#include <asm-i386/current.h>

#define SECRET_MAXSIZE 32
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