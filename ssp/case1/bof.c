/* sudo sysctl -w kernel.randomize_va_space=0 */
/* gcc -Wl,-z,norelro -no-pie bof.c */
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char buf[100];
    gets(buf);
    printf(buf);
    putchar('\n');
    return 0;
}