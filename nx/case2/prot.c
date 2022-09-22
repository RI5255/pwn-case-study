#include <sys/mman.h>
#include <stdio.h>

int main(){
    printf("%d\n", PROT_WRITE|PROT_READ|PROT_EXEC);
    return 0;
}
