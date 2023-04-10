#ifndef MAGIC_API_H
#define MAGIC_API_H

#include <linux/types.h>

int magic_get_wand(int pid, const char *message, ssize_t message_size);
int magic_attack(int pid, const char *message, ssize_t message_size);
int magic_legilimens(int pid, const char *message, ssize_t message_size);
int magic_list_secrets(int pid, const char *message, ssize_t message_size) 

#endif