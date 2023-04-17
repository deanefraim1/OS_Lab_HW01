#include "magic_api.h"
#include <stdio.h>
#include <stdlib.h>

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
        int actNumber;
        scanf("%d", &actNumber);
        switch (actNumber)
        {
            case 1:
            {
                printf("Please enter power: \n");
                int power;
                scanf("%d", &power);
                printf("Please enter secret: \n");
                char secret[SECRET_MAXSIZE];
                scanf("%s", secret);
                magic_get_wand(power, secret);
            }
            case 2:
            {
                printf("Please enter pid: \n");
                int pid;
                scanf("%d", &pid);
                magic_attack(pid);
            }
            case 3:
            {
                printf("Please enter pid: \n");
                int pid;
                scanf("%d", &pid);
                magic_legilimens(pid);
            }
            case 4:
            {
                printf("Please enter size: \n");
                size_t size;
                scanf("%zu", &size);


                char** secrets = (char**)malloc(size * sizeof(char*));
                int i;
                for (i = 0; i < size; i++)
                {
                    secrets[i] = (char*)malloc(SECRET_MAXSIZE * sizeof(char));
                }
                magic_list_secrets(secrets, size);
                for (i = 0; i < size; i++)
                {
                    printf("stolen secret number %d: %s\n",i, secrets[i]);
                }
                for (i = 0; i < size; i++)
                {
                    free(secrets[i]);
                }
                free(secrets);
            }
            case 5:
            {
                return 0;
            }
        }
    }
}
