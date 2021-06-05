#include "woody_woodpacker.h"

int     parse_bytecode(t_file *file)
{
    file->b_offset = file->ehdr->e_phoff + file->ehdr->e_phnum * file->ehdr->e_phentsize;
    if (file->ehdr->e_shoff <= file->b_offset)
        file->b_filesz = file->size - file->b_offset;
    else
        file->b_filesz = file->ehdr->e_shoff - file->b_offset;
    file->bytecode = malloc(file->b_filesz);
    if (!file->bytecode)
    {
        fprintf(stderr, "woody_woodpacker: malloc error %zu %zu\n", file->ehdr->e_shoff, file->b_offset);
        exit(EXIT_FAILURE);
    }
    memcpy(file->bytecode, file->mapped_file + file->b_offset, file->b_filesz);
    return 0;
}

// char    *empty_bytecode_space(t_file *file, Elf64_Off payload_size)
// {
//     elf_phdr *phdr;
//     elf_phdr *last_phdr;
    
//     phdr = file->phdr;
//     for (Elf64_Half i = 0; i < file->ehdr->e_phnum; i++)
//     {
//         if (phdr->p_type == PT_LOAD)
//             last_phdr = phdr;
//         phdr++;
//     }
//     fprintf(stderr, "type: %x flags %u off %lx v_addr %lx paddr %lx p_filesz %lx p_memsz %lx p_align %lu\n", last_phdr->p_type, last_phdr->p_flags, last_phdr->p_offset, last_phdr->p_vaddr, last_phdr->p_paddr, last_phdr->p_filesz, last_phdr->p_memsz, last_phdr->p_align);

//     // print_64phdr()
//     return NULL;
// }
