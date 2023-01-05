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
    Elf64_Shdr *shstrtab, *shdr, *dynsym, *dynstr, *relaplt;
    Elf64_Rela *rela;
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
         // .dynsymセクションのエントリを特定
        if(!strcmp(head + shstrtab->sh_offset + shdr->sh_name, ".dynsym"))
            dynsym = shdr;
        // .dynstrセクションのエントリを特定
        if(!strcmp(head + shstrtab->sh_offset + shdr->sh_name, ".dynstr"))
            dynstr = shdr;
        // .rela.pltセクションのエントリを特定
        if(!strcmp(head + shstrtab->sh_offset + shdr->sh_name, ".rela.plt"))
            relaplt = shdr;
    }

    // .rela.pltセクションの再配置テーブルを見て再配置情報を列挙
    for(int i = 0; i < relaplt->sh_size / relaplt->sh_entsize; i++) {
        rela = (Elf64_Rela*)(head + relaplt->sh_offset + relaplt->sh_entsize * i);
        // 参照するべきELF64_Sym構造体を特定
        sym = (Elf64_Sym*)(head + dynsym->sh_offset + dynsym->sh_entsize * ELF64_R_SYM(rela->r_info));
        //再配置情報を表示
        printf("\t%s\n\tr_offset: %016lx\n\tr_info(SYM): %08lx\n\tr_info(TYPE): %08lx\n",
        head + dynstr->sh_offset + sym->st_name, 
        rela->r_offset, 
        ELF64_R_SYM(rela->r_info), 
        ELF64_R_TYPE(rela->r_info));
    }

    munmap(head, sb.st_size);
    close(fd);
    return 0;

}