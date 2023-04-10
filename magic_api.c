#include <asm/errno.h>

#define SECRET_MAXSIZE 32

int magic_get_wand(int pid, const char *message, ssize_t message_size) {
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
        :"=m" (res)
        :"m" (pid) ,"m" (message) ,"m"(message_size) 
    );

    if (res < 0)
    {
        errno = -res;
        res = -1;
    }
    return res; 
}

int magic_attack(int pid, const char *message, ssize_t message_size) {
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
        :"=m" (res)
        :"m" (pid) ,"m" (message) ,"m"(message_size) 
    );

    if (res < 0)
    {
        errno = -res;
        res = -1;
    }
    return res; 
}

int magic_legilimens(int pid, const char *message, ssize_t message_size) {
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
        :"=m" (res)
        :"m" (pid) ,"m" (message) ,"m"(message_size) 
    );

    if (res < 0)
    {
        errno = -res;
        res = -1;
    }
    return res; 
}

int magic_list_secrets(int pid, const char *message, ssize_t message_size) {
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
        :"=m" (res)
        :"m" (pid) ,"m" (message) ,"m"(message_size) 
    );

    if (res < 0)
    {
        errno = -res;
        res = -1;
    }
    return res; 
}