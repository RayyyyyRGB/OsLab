
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int ac, char** av)
{
    if (ac < 1)
    {
        printf("\npar number<1 error!");
        return -1;
    }
    if (av[1] == 0)
    {
        printf("\nNo par error!");
        return -1;
    }
    int tic = atoi(av[1]);
    sleep(tic);
    
    exit(0);
}
