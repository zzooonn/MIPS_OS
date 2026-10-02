#include "elf.h"
#include <stdio.h>

int is_elf_format(const void *binary, size_t size) {
    Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;
    return size >= sizeof(Elf32_Ehdr) && ehdr->e_ident[EI_MAG0] == ELFMAG0 &&
           ehdr->e_ident[EI_MAG1] == ELFMAG1 && ehdr->e_ident[EI_MAG2] == ELFMAG2 &&
           ehdr->e_ident[EI_MAG3] == ELFMAG3;
}

int readelf(const void *binary, size_t size) {
    Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;

    if (!is_elf_format(binary, size)) {
        fputs("not an elf file\n", stderr);
        return -1;
    }

    const void *sh_table = binary + ehdr->e_shoff;
    Elf32_Half sh_entry_count = ehdr->e_shnum;
    Elf32_Half sh_entry_size = ehdr->e_shentsize;

    for (int i = 0; i < sh_entry_count; i++) {
        const Elf32_Shdr *shdr = (const Elf32_Shdr *)((const char *)sh_table + i * sh_entry_size);
        unsigned int addr = shdr->sh_addr;

        printf("%d:0x%x\n", i, addr);
    }

    return 0;
}

