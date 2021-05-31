#include "woody_woodpacker.h"

int     parse_bytecode(t_file *file)
{
    file->b_offset = file->ehdr->e_phoff + file->ehdr->e_phnum * file->ehdr->e_phentsize;
    file->b_filesz = file->ehdr->e_shoff - file->b_offset;
    file->bytecode = malloc(file->b_filesz);
    if (!file->bytecode)
    {
        fprintf(stderr, "woody_woodpacker: malloc error\n");
        exit(EXIT_FAILURE);
    }
    memcpy(file->bytecode, file->mapped_file + file->b_offset, file->b_filesz);
    return 0;
}

char    *empty_bytecode_space(t_file *file)
{
    Elf64_Phdr *phdr;

    phdr = file->phdr;
    for (Elf64_Half i = 0; i < file->ehdr->e_phnum; i++)
    {
        // if (phdr)
        
        phdr++;
    }
    return NULL;
}
