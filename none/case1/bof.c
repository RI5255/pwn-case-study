/* sudo sysctl -w kernel.randomize_va_space=0 
/* gcc -z execstack -fno-stack-protector -Wl,-z,norelro -no-pie bof.c */
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    char buf[100];
    setlinebuf(stdout);
    printf("buf = %p\n", buf);
    gets(buf);
    puts(buf);
    return 0;
}