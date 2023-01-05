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
    Elf64_Shdr *shstrtab, *shdr, *symtab, *strtab;
    Elf64_Sym *sym;

    // ELF形式をメモリに展開
    fd = open("a.out", O_RDONLY);
    fstat(fd, &sb);
    head = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    
    ehdr = (Elf64_Ehdr*)head;
    
    // .shstrtabセクションのエントリを特定
    shstrtab = (Elf64_Shdr*)(head + ehdr->e_shoff + ehdr->e_shentsize * ehdr->e_shstrndx);

    for(int i = 0; i < ehdr->e_shnum; i++) {
        shdr = (Elf64_Shdr*)(head + ehdr->e_shoff + ehdr->e_shentsize * i);
        // .symtabセクションのエントリを特定
        if(!strcmp(head + shstrtab->sh_offset + shdr->sh_name, ".symtab"))
            symtab = shdr;
        // .strtabセクションのエントリを特定
        if(!strcmp(head + shstrtab->sh_offset + shdr->sh_name, ".strtab"))
            strtab = shdr;
    }

    // .symtabセクションのシンボルテーブルをみてシンボル名を列挙
    for(int i = 0; i < symtab->sh_size / symtab->sh_entsize; i++) {
        sym = (Elf64_Sym*)(head + symtab->sh_offset + symtab->sh_entsize * i);
        if(!sym->st_name) continue;
        printf("%s\n", head + strtab->sh_offset + sym->st_name);
    }

    munmap(head, sb.st_size);
    close(fd);
    return 0;

}