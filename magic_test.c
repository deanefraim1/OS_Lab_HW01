#include "magic_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SECRET_MAXSIZE 32

int main()
{
    while(1)
    {
        printf("Please enter desired act: \n");
        printf("1. Get wand\n");
        printf("2. Attack\n");
        printf("3. Legilimens\n");
        printf("4. List secrets\n");
        printf("5. Exit\n");
        printf("6. Get PID\n");
        int actNumber;
        scanf("%d", &actNumber);
        switch (actNumber)
        {
            case 1:
            {
                printf("Please enter power: ");
                int power;
                scanf("%d", &power);
                printf("Please enter secret: ");
                char secret[SECRET_MAXSIZE];
                scanf("%s", secret); 
                magic_get_wand(power, secret);
                printf("errno = %d\n", errno);
                break;
            }
            case 2:
            {
                printf("Please enter pid: ");
                int pid;
                scanf("%d", &pid);
                magic_attack(pid);
                printf("errno = %d\n", errno);
                break;
            }
            case 3:
            {
                printf("Please enter pid: ");
                int pid;
                scanf("%d", &pid);
                magic_legilimens(pid);
                printf("errno = %d\n", errno);
                break;
            }
            case 4:
            {
                printf("Please enter size: ");
                size_t size;
                scanf("%zu", &size);


                char** secrets = (char**)malloc(size * sizeof(char*));
                int i;
                for (i = 0; i < size; i++)
                {
                    secrets[i] = (char*)malloc(SECRET_MAXSIZE * sizeof(char));
                }
                int remainingSecretsNumber = magic_list_secrets(secrets, size);
                printf("remainingSecretsNumber = %d\n", remainingSecretsNumber);
                printf("100\n");
                for (i = 0; i < size; i++)
                {
                    printf("stolen secret number %d: %s\n",i, secrets[i]);
                }
                printf("101\n");
                for (i = 0; i < size; i++)
                {
                    free(secrets[i]);
                }
                free(secrets);
                printf("errno = %d\n", errno);
                break;
            }
            case 5:
            {
                return 0;
            }
            case 6:
            {
                printf("pid = %d\n", getpid());
                break;
            }
        }
    }
}
