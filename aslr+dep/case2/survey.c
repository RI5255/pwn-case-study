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
    Elf64_Shdr *shstr, *shdr, *rela_plt, *dynsym, *dynstr;
    Elf64_Rela *rela, *write_rela;
    Elf64_Sym *sym;

    fd = open("a.out", O_RDONLY);
    fstat(fd, &sb);
    head = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);

    // セクションヘッダテーブルのアドレスを特定する。
    ehdr = (Elf64_Ehdr*)head;
    printf("section header table@%p\n", head + ehdr -> e_shoff);

    // セクションヘッダから.rela.plt, .dynsym, .dynstrセクションを探してアドレスを特定する
    shstr = (Elf64_Shdr*)(head + ehdr->e_shoff + ehdr->e_shentsize * ehdr->e_shstrndx);
    for(int i = 0; i < ehdr -> e_shnum; i++) {
        shdr = (Elf64_Shdr*)(head + ehdr->e_shoff + ehdr->e_shentsize * i);
        if(!strcmp(head + shstr->sh_offset + shdr->sh_name, ".rela.plt"))
            rela_plt = shdr;
         if(!strcmp(head + shstr->sh_offset + shdr->sh_name, ".dynsym"))
            dynsym = shdr;
        if(!strcmp(head + shstr->sh_offset + shdr->sh_name, ".dynstr"))
            dynstr = shdr;
        
    }
    printf(".rela.plt@0x%p .dynsym@%p .dynstr@%p\n", head + shdr->sh_offset, head + dynsym->sh_offset, head + dynstr->sh_offset); 

    // write@pltに飛んだ時に参照されるELF64_Rela構造体を読んでみる。あれ？なんかr_offsetに既視感が..?
    rela = (Elf64_Rela*)(head + rela_plt->sh_offset);
    printf("r_offset: 0x%08lx, r_info(SYM): 0x%08lx\n", rela->r_offset, ELF64_R_SYM(rela->r_info));

    // このELF64_Rela構造体から参照されているシンボルテーブルエントリを見てみる
    sym = (Elf64_Sym*)(head + dynsym->sh_offset + dynsym->sh_entsize * ELF64_R_SYM(rela->r_info));
    printf("%s\n", head + dynstr->sh_offset + sym -> st_name);

    munmap(head, sb.st_size);
    close(fd);
    return 0;
}