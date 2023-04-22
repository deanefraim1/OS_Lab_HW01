#include <asm/current.h>
#include <linux/string.h>
#include <asm/errno.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/magic_syscall.h>
#include <linux/types.h>

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

void PrintWandStatus(struct wand_struct *wand)
{
    printk("wand power: %d)", wand->power);
    printk("wand health: %d)", wand->health);
    printk("wand secret: %s)", wand->secret);
    PrintStolenSecretList(wand->stolenSecretsListHead);
}

void PrintStolenSecretList(struct list_head* stolenSecretsListHead)
{
    list_t *currentStolenSecretPtr;
    struct stolenSecretListNode *currentStolenSecretNode;
    int i = 1;
    list_for_each(currentStolenSecretPtr, stolenSecretsListHead)
    {
        currentStolenSecretNode = list_entry(currentStolenSecretPtr, struct stolenSecretListNode, ptr);
        printk("stolen secret number %d: %s\n", i, currentStolenSecretNode->secret);
        i++;
    }
}

int magic_get_wand_syscall(int power, char secret[SECRET_MAXSIZE])
{
    if(strlen(secret) == 0)
    {
        return -EINVAL;
    }

    struct task_struct *currentProccess = current;
    if(currentProccess->wand != NULL)
    {
        return -EEXIST;
    }

    printk("before kmalloc\n");
    struct wand_struct *currentProccessWand = (struct wand_struct *)kmalloc(sizeof(struct wand_struct), GFP_KERNEL);
    printk("after kmalloc\n");
    if(currentProccessWand == NULL)
    {
        return -ENOMEM;
    }
        
    currentProccessWand->power = power;
    currentProccessWand->health = 100;
    printk("before strcpy\n");
    if(strcpy(currentProccessWand->secret, secret) == NULL)
    {
        kfree(currentProccessWand); // should we free the wand in every error case?
        printk("strcpy failed\n");
        return -EFAULT;
    }
    printk("after strcpy\n");

    INIT_LIST_HEAD(currentProccessWand->stolenSecretsListHead);

    printk("status of pid:\n", currentProccess->pid);
    PrintWandStatus(currentProccessWand);

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
    
    printk("status of pid:\n", currentProccess->pid);
    PrintWandStatus(currentProccessWand);
    printk("status of pid:\n", proccessToAttack->pid);
    PrintWandStatus(proccessToAttackWand);

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
    struct stolenSecretListNode *newStolenSecretNode = (struct stolenSecretListNode*)kmalloc(sizeof(struct stolenSecretListNode), GFP_KERNEL);
    strcpy(newStolenSecretNode->secret, proccessToStealFromWand->secret);
    list_add(newStolenSecretNode->ptr, currentProccessWand->stolenSecretsListHead);

    printk("status of pid:\n", currentProccess->pid);
    PrintWandStatus(currentProccessWand);
    printk("status of pid:\n", proccessToStealFrom->pid);
    PrintWandStatus(proccessToStealFromWand);
    
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
        int i;
        for (i = numberOfSecretsCopied; i < size; i++)
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

