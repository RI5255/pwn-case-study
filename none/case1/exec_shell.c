#include <unistd.h>

int main(){
    char* arg[] = {"/bin/sh", NULL};
    execve(arg[0], arg, NULL);
}