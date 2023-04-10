#ifndef MAGIC_API_H
#define MAGIC_API_H

#include <linux/types.h>
#include <errno.h>

#define SECRET_MAXSIZE 32

int magic_get_wand(int power, char secret[SECRET_MAXSIZE]) {
    int res;
    __asm__
    (
        "pushl %%eax;"
        "pushl %%ebx;"
        "pushl %%ecx;"
        "pushl %%edx;"
        "movl $243, %%eax;"
        "movl %1, %%ebx;"
        "movl %2, %%ecx;"
        "movl %3, %%edx;"
        "int $0x80;"
        "movl %%eax,%0;"
        "popl %%edx;"
        "popl %%ecx;"
        "popl %%ebx;"
        "popl %%eax;"
        : "=m"(res)
        : "m"(power), "m"(secret)
    );

    if (res < 0)
    {
        errno = -res;
        res = -1;
    }
    return res; 
}

int magic_attack(int pid) {
    int res; 
    __asm__
    (
        "pushl %%eax;"
        "pushl %%ebx;" 
        "pushl %%ecx;" 
        "pushl %%edx;" 
        "movl $244, %%eax;" 
        "movl %1, %%ebx;" 
        "movl %2, %%ecx;" 
        "movl %3, %%edx;" 
        "int $0x80;"
        "movl %%eax,%0;" 
        "popl %%edx;" 
        "popl %%ecx;" 
        "popl %%ebx;" 
        "popl %%eax;"
        :"=m" (res)
        :"m" (pid)
    );

    if (res < 0)
    {
        errno = -res;
        res = -1;
    }
    return res; 
}

int magic_legilimens(int pid) {
    int res; 
    __asm__
    (
        "pushl %%eax;"
        "pushl %%ebx;" 
        "pushl %%ecx;" 
        "pushl %%edx;" 
        "movl $245, %%eax;" 
        "movl %1, %%ebx;" 
        "movl %2, %%ecx;" 
        "movl %3, %%edx;" 
        "int $0x80;"
        "movl %%eax,%0;" 
        "popl %%edx;" 
        "popl %%ecx;" 
        "popl %%ebx;" 
        "popl %%eax;"
        :"=m" (res)
        :"m" (pid)
    );

    if (res < 0)
    {
        errno = -res;
        res = -1;
    }
    return res; 
}

int magic_list_secrets(char secrets[][SECRET_MAXSIZE], size_t size) {
    int res; 
    __asm__
    (
        "pushl %%eax;"
        "pushl %%ebx;" 
        "pushl %%ecx;" 
        "pushl %%edx;" 
        "movl $246, %%eax;" 
        "movl %1, %%ebx;" 
        "movl %2, %%ecx;" 
        "movl %3, %%edx;" 
        "int $0x80;"
        "movl %%eax,%0;" 
        "popl %%edx;" 
        "popl %%ecx;" 
        "popl %%ebx;" 
        "popl %%eax;"
        :"=m" (res)
        :"m" (secrets) ,"m" (size)
    );

    if (res < 0)
    {
        errno = -res;
        res = -1;
    }
    return res; 
}

#endif