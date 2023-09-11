#include "kernel/types.h"
#include <user/user.h>



void forkPrimes(int prime){
    int p[2];
    pipe(p);

    int pid = fork();

    if (pid == 0)
    {
        int buf;
        close(p[1]);
        read(p[0], &buf, sizeof(int));
        printf("prime %d\n",buf);
        int judge = 1;
        int new_buf = buf + 1;
        for ( ; ; new_buf++)
        {
            if(new_buf >= 35)
                exit(0);
            for (int j = 2; j < new_buf; j++)
            {
                judge = new_buf % j;
                if(!judge)
                    break;
            }
            if(judge){
                break;
            }
        }
        forkPrimes(new_buf);
        exit(0);
    }
    else{
        close(p[0]);
        write(p[1], &prime, sizeof(int));
        wait(0);  // 非常有必要，保证生命周期链的正确性
        exit(0);
    }
    
}

int main(int argc, char *argv[])
{
    forkPrimes(2);
    exit(0);
}
