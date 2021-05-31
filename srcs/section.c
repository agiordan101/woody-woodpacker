#include "woody_woodpacker.h"

elf_shdr    *parse_64shdr(t_file *file)
{
    elf_shdr    *shdr;

    shdr = calloc(file->ehdr->e_shnum + 1, sizeof(elf_shdr));
    memcpy(shdr, file->mapped_file + file->ehdr->e_shoff, file->ehdr->e_shnum * sizeof(elf_shdr));
    return shdr;
}

void    print_64shdr(t_file *file)
{
    Elf64_Shdr *shdr;
    Elf64_Shdr *shdr_str_table;
    char        *str_table;

    shdr = file->shdr;
    shdr_str_table = (Elf64_Shdr *)(file->mapped_file + file->ehdr->e_shoff + file->ehdr->e_shstrndx * sizeof(Elf64_Shdr));
    str_table = file->mapped_file + shdr_str_table->sh_offset;

    for (Elf64_Half i = 0; i < file->ehdr->e_shnum; i++)
    {
        fprintf(stderr, "name: %s ", str_table + shdr->sh_name);
        // fprintf(stderr, "name: %c\n", shdr->sh_name);
        // fprintf(stderr, "name: %s\n", file->mapped_file + file->ehdr->e_shstrndx + shdr->sh_name);
        fprintf(stderr, "sh_type: %x sh_flags: %lx sh_addr: %lx sh_offset: %lx sh_size: %lx sh_addralign: %lx sh_entsize: %lx\n", shdr->sh_type, shdr->sh_flags, shdr->sh_addr, shdr->sh_offset, shdr->sh_size, shdr->sh_addralign, shdr->sh_entsize);
        shdr++;
    }
}
