#include <linux/magic_syscall.h>
#include <asm/current.h>
#include <linux/string.h>
#include <asm/errno.h>
#include <linux/sched.h>
#include <linux/list.h>

#define TRUE 1
#define FALSE 0
#define SUCCESS 0

int IsSecretInList(struct list_head* secretsList, char secret[SECRET_MAXSIZE])
{
    list_t *currentStolenSecretPtr;
    struct stolenSecretListNode *currentStolenSecretNode;
    list_for_each(currentStolenSecretPtr, secretsList)
    {
        currentStolenSecretNode = list_entry(currentStolenSecretPtr, struct stolenSecretListNode, ptr);
        if(strcmp(currentStolenSecretNode->secret, secret) == 0) 
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
    
    struct wand_struct *wand = (struct wand_struct*)kalloc(sizeof(struct wand_struct));
    if(wand == NULL)
    {
        return -ENOMEM;
    }
        
    wand->power = power;
    wand->health = 100;
    if(strcpy(wand->secret, secret) == NULL)
    {
        kfree(wand); // should we free the wand in every error case?
        return -EFAULT;
    }

    INIT_LIST_HEAD(wand->stolenSecretsListHead);
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
    if(pid == currentProccess->pid || IsSecretInList(proccessToAttackWand->stolenSecretsListHead, currentProccessWand->secret) == TRUE)
    {
        return -CONNREFUSED;
    }
    proccessToAttackWand->health  = proccessToAttackWand->health - currentProccessWand->power > 0 ? proccessToAttackWand->health - currentProccessWand->power : 0;
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
    if(IsSecretInList(currentProccessWand->stolenSecretsListHead, proccessToAttackWand->secret))
    {
        return -EEXIST;
    }
    list_add(currentProccessWand->stolenSecretsListHead, proccessToAttackWand->secret);
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
    list_t *currentStolenSecretPtr;
    struct stolenSecretListNode *currentStolenSecretNode;
    list_for_each(currentStolenSecretPtr, currentProccess->wand->stolenSecretsListHead)
    {
        currentStolenSecretNode = list_entry(currentStolenSecretPtr, struct stolenSecretListNode, ptr);
        totalSecrets++;
        if (i < size)
        {
            if(strcpy(secrets[i], list_entry(currentStolenSecretNode, struct stolenSecretListNode, secret)) == NULL) // FIXME
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

