#include <linux/magic_syscall.h>
#include <asm/current.h>
#include <linux/string.h>
#include <asm/errno.h>
#include <linux/sched.h>

#define TRUE 1
#define FALSE 0
#define SUCCESS 0

int IsSecretInList(struct list_head* secretsList, char secret[SECRET_MAXSIZE])
{
    struct list_head *currentSecret;
    list_for_each(currentSecret, secretsList)
    {
        if(strcmp(list_entry(currentSecret, struct wand_struct, stolen_secrets)->secret, secret) == 0) 
            return TRUE;
    }
    return FALSE;
}

int magic_get_wand_syscall(int power, char secret[SECRET_MAXSIZE])
{
    if(strlem(secret) == 0)
    {
        return -EINVAL;
    }

    struct task_struct *currentProccess = current;
    if(currentProccess->wand != NULL)
    {
        return -EEXIST;
    }
        
    struct wand_struct *wand = (struct wand_struct*)malloc(sizeof(struct wand_struct));
    if(wand == NULL)
    {
        return -ENOMEM;
    }
        
    wand->power = power;
    wand->health = 100;
    if(strcpy(wand->secret, secret) == NULL)
    {
        free(wand); // should we free the wand in every error case?
        return -EFAULT;
    }

    INIT_LIST_HEAD(wand->stolen_secrets);
    return SUCCESS;
}

int magic_attack_syscall(pid_t pid)
{
    struct task_struct *currentProccess = current;
    struct task_struct *proccessToAttack = find_task_by_pid(pid);
    if(proccessToAttack == NULL)
    {
        return -ESRCH;
    }
    struct wand_struct *currentProccessWand = currentProccess->wand;
    struct wand_struct *proccessToAttackWand = proccessToAttack->wand;
    if(proccessToAttackWand == NULL || currentProccessWand == NULL)
    {
        return -EPERM;
    }
    if(proccessToAttackWand->health == 0)
    {
        return -EHOSTDOWN;
    }
    if(pid == currentProccess->pid || IsSecretInList(proccessToAttack->stolen_secrets, currentProccess->secret) == TRUE)
    {
        return -CONNREFUSED;
    }
    proccessToAttackWand->health  = proccessToAttack->health - currentProccessWand->power > 0 ? proccessToAttack->health - currentProccessWand->power : 0;
    return SUCCESS;
}

int magic_legilimens_syscall(pid_t pid)
{
    struct task_struct *currentProccess = current;
    struct task_struct *proccessToAttack = find_task_by_pid(pid);
    if(proccessToAttack == NULL)
    {
        return -ESRCH;
    }
    struct wand_struct *currentProccessWand = currentProccess->wand;
    struct wand_struct *proccessToAttackWand = proccessToAttack->wand;
    if(proccessToAttackWand == NULL || currentProccessWand == NULL)
    {
        return -EPERM;
    }
    if(pid == currentProccess->pid)
    {
        return SUCCESS;
    }
    if(IsSecretInList(currentProccess->stolen_secrets, proccessToAttack->secret))
    {
        return -EEXIST;
    }
    list_add(currentProccess->stolen_secrets, proccessToAttack->secret);
    return SUCCESS;
}

int magic_list_secrets_syscall(char secrets[][SECRET_MAXSIZE], size_t size)
{
    if(secrets == NULL)
    {
        return -EFAULT;
    }
    struct task_struct *currentProccess = current;
    struct wand_struct *currentProccessWand = currentProccess->wand;
    if(currentProccessWand == NULL)
    {
        return -EPERM;
    }
    int i = 0;
    int totalSecrets = 0;
    struct list_head *currentSecret;
    list_for_each(currentSecret, currentProccess->stolen_secrets)
    {
        totalSecrets++;
        if (i < size)
        {
            if(strcpy(secrets[i], strcmp(list_entry(currentSecret, struct wand_struct, stolen_secrets)->secret) == NULL)) // should we use GET_LIST_ENTRY?
            {
                return -EFAULT;
            }
            i++;
            continue;
        }
        else if(i >= size)
        {
            for(int j = i; j < size; j++)
            {
                secrets[j][0] = '\0';
            }
            return 0;
        }
    }
    return totalSecrets-size;
}

