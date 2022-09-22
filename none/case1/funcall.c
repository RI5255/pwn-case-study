// gcc -O0 funcall.c
#include <stdio.h>

int func(int a, int b, int c, int d, int e, int f, int g){
    return a + b + c + d + e + f + g;
}

int main(){
    return func(1,2,3,4,5,6,7);
}