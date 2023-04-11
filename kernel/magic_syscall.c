#include <asm/current.h>
#include <linux/string.h>
#include <asm/errno.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/magic_syscall.h>

#define TRUE 1
#define FALSE 0
#define SUCCESS 0

struct stolenSecretListNode
{
    char secret[SECRET_MAXSIZE];
    list_t* ptr;
};

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
        return -ECONNREFUSED;
    }
    proccessToAttackWand->health  = proccessToAttackWand->health - currentProccessWand->power > 0 ? proccessToAttackWand->health - currentProccessWand->power : 0;
    return SUCCESS;
}

int magic_legilimens_syscall(pid_t pid)
{
    struct task_struct *currentProccess = current;
    struct task_struct *proccessToStealFrom = find_task_by_pid(pid);
    if(proccessToStealFrom == NULL)
    {
        return -ESRCH;
    }
    struct wand_struct *currentProccessWand = currentProccess->wand;
    struct wand_struct *proccessToStealFromWand = proccessToStealFrom->wand;
    if(proccessToStealFromWand == NULL || currentProccessWand == NULL)
    {
        return -EPERM;
    }
    if(pid == currentProccess->pid)
    {
        return SUCCESS;
    }
    if(IsSecretInList(currentProccessWand->stolenSecretsListHead, proccessToStealFromWand->secret))
    {
        return -EEXIST;
    }
    struct stolenSecretListNode *newStolenSecretNode = (struct stolenSecretListNode*)kalloc(sizeof(struct stolenSecretListNode));
    strcpy(newStolenSecretNode->secret, proccessToStealFromWand->secret);
    list_add(newStolenSecretNode->ptr, currentProccessWand->stolenSecretsListHead);
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
    int numberOfSecretsCopied = 0;
    int totalSecrets = 0;
    list_t *currentStolenSecretPtr;
    struct stolenSecretListNode *currentStolenSecretNode;
    list_for_each(currentStolenSecretPtr, currentProccessWand->stolenSecretsListHead)
    {
        totalSecrets++;
        if(numberOfSecretsCopied < size)
        {
            currentStolenSecretNode = list_entry(currentStolenSecretPtr, struct stolenSecretListNode, ptr);
            if(secrets[numberOfSecretsCopied] == NULL || strcpy(secrets[numberOfSecretsCopied], currentStolenSecretNode->secret) == NULL)
            {
                return -EFAULT;
            }
            numberOfSecretsCopied++;
        }
        continue;
    }
    if(size > numberOfSecretsCopied)
    {
        for(int i = numberOfSecretsCopied; i < size; i++)
        {
            if(secrets[i] == NULL)
            {
                return -EFAULT;
            }
            secrets[i][0] = '/0';
        }
    }
    return totalSecrets-numberOfSecretsCopied;
}

