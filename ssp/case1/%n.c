#include <stdio.h>

int main(){
    int i;
    printf("%20c%2$n",'a', &i);
    printf("%d\n", i); // 20
    return 0;
}