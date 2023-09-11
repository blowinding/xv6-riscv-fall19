#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if (argc <= 0){
        fprintf(2, "no time input");
        exit(1);
    }
    int t = atoi(argv[1]);
    sleep(t);
    exit(0);
}