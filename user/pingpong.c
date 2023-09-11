#include "kernel/types.h"
#include <user/user.h>


int
main(int argc, char *argv[])
{
    int p1[2], p2[2];
    char buf;
    pipe(p1);
    pipe(p2);
    int pid = fork();
    if(pid == 0){
        close(p1[1]);
        close(p2[0]);
        int isread = read(p1[0], &buf, 1);
        if (isread > 0)
            printf("%d: received ping\n", getpid());
        int iswrite = write(p2[1], &buf, 1);
        if (iswrite <= 0){
            fprintf(2, "child's writing is wrong");
            exit(1);
        }
        else if (isread <= 0){
            fprintf(2, "child's reading is wrong");
            exit(1);
        }
        exit(0);
    }
    else{
        close(p1[0]);
        close(p2[1]);
        int iswrite = write(p1[1], &buf, 1);
        int isread = read(p2[0], &buf, 1);
        
        if (isread > 0)
            printf("%d: received pong\n", getpid());
        else if (iswrite <= 0){
            fprintf(2, "parent's writing is wrong");
            exit(1);
        }
        else if (isread <= 0){
            fprintf(2, "parent's reading is wrong");
            exit(1);
        }
        exit(0);
    }
    // else{
    //     fprintf(2, "forking is wrong");
    //     exit(1);
    // }
}
