/* sudo sysctl -w kernel.randomize_va_space=0 */
/* gcc -fno-stack-protector -Wl,-z,norelro -no-pie bof.c */
#include <stdio.h>

int main()
{
    char buf[100];
    setlinebuf(stdout);
    gets(buf);
    puts(buf);
    return 0;
}