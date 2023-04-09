#include "magic_syscall.h"

bool IsSecretInList(struct list_head* secretsList, char secret[SECRET_MAXSIZE])
{
    list_for_each(struct list_head *currentSecret, secretsList)
    {
        if(strcmp(currentSecret, secret) == 0)
            true;
    }
    return false;
}

int magic_get_wand(int power, char secret[SECRET_MAXSIZE])
{
    if(strlem(secret) == 0)
    {
        errno = EINVAL;
        return -1;
    }

    struct task_struct *currentProccess = current;
    if(currentProccess->wand != NULL)
    {
        errno = EEXIST;
        return -1;
    }
        
    struct wand_struct *wand = (struct wand_struct*)malloc(sizeof(struct wand_struct));
    if(wand == NULL)
    {
        errno = ENOMEM;
        return -1;
    }
        
    wand->power = power;
    wand->health = 100;
    if(strcpy(wand->secret, secret) == NULL)
    {
        free(wand);
        errno = EFAULT;
        return -1;
    }

    INIT_LIST_HEAD(wand->stolen_secrets); // is this the way to init a list_head? 
    return 0;
}

int magic_attack(pid_t pid)
{
    struct task_struct *currentProccess = current;
    struct task_struct *proccessToAttack = find_task_by_pid(pid);
    if(proccessToAttack == NULL)
    {
        errno = ESRCH;
        return -1;
    }
    struct wand_struct *currentProccessWand = currentProccess->wand;
    struct wand_struct *proccessToAttackWand = proccessToAttack->wand;
    if(proccessToAttackWand == NULL || currentProccessWand == NULL)
    {
        errno = EPERM;
        return -1;
    }
    if(proccessToAttackWand->health == 0)
    {
        errno = EHOSTDOWN;
        return -1;
    }
    if(pid == currentProccess->pid || IsSecretInList(proccessToAttack->stolen_secrets, currentProccess->secret))
    {
        errno = ECONNREFUSED;
        return -1;
    }
    proccessToAttackWand->health  = proccessToAttack->health - currentProccessWand->power > 0 ? proccessToAttack->health - currentProccessWand->power : 0;
    return 0;
}

int magic_legilimens(pid_t pid)
{
    struct task_struct *currentProccess = current;
    struct task_struct *proccessToAttack = find_task_by_pid(pid);
    if(proccessToAttack == NULL)
    {
        errno = ESRCH;
        return -1;
    }
    struct wand_struct *currentProccessWand = currentProccess->wand;
    struct wand_struct *proccessToAttackWand = proccessToAttack->wand;
    if(proccessToAttackWand == NULL || currentProccessWand == NULL)
    {
        errno = EPERM;
        return -1;
    }
    if(pid == currentProccess->pid)
    {
        return 0;
    }
    if(IsSecretInList(currentProccess->stolen_secrets, proccessToAttack->secret))
    {
        errno = EEXIST;
        return -1;
    }
    list_add(currentProccess->stolen_secrets, proccessToAttack->secret);
    return 0;
}

int magic_list_secrets(char secrets[][SECRET_MAXSIZE], size_t size)
{
    if(secrets == NULL)
    {
        errno = EFAULT;
        return -1;
    }
    struct task_struct *currentProccess = current;
    struct wand_struct *currentProccessWand = currentProccess->wand;
    if(currentProccessWand == NULL)
    {
        errno = EPERM;
        return -1;
    }
    int i = 0;
    int totalSecrets = 0;
    list_for_each(struct list_head * currentSecret, currentProccess->stolen_secrets)
    {
        totalSecrets++;
        if (i < size)
        {
            if(strcpy(secrets[i], currentSecret) == NULL)
            {
                errno = EFAULT;
                return -1;
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

