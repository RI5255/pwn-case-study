#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <elf.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(void) { 
    int fd;
    struct stat sb;
    char *head;
    Elf64_Ehdr *ehdr;
    Elf64_Shdr *shstrtab, *shdr;

    // ELF形式をメモリに展開
    fd = open("a.out", O_RDONLY);
    fstat(fd, &sb);
    head = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);

    // セクションヘッダテーブルのアドレスを特定
    ehdr = (Elf64_Ehdr*)head;
    printf("section header table@%p\n", head + ehdr->e_shoff);

    // .shstrtabセクションのエントリを特定
    shstrtab = (Elf64_Shdr*)(head + ehdr->e_shoff + ehdr->e_shentsize * ehdr->e_shstrndx);

    // セクション名を列挙
    for(int i = 0; i < ehdr->e_shnum; i++) {
        shdr = (Elf64_Shdr*)(head + ehdr->e_shoff + ehdr->e_shentsize * i);
        printf("%s\n", head + shstrtab->sh_offset + shdr->sh_name);
    }

    munmap(head, sb.st_size);
    close(fd);
    return 0;

}