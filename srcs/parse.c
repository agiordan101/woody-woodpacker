#include "woody_woodpacker.h"

void     parse_hdr(t_file *file)
{
    Elf64_Ehdr      *ehdr;
    unsigned char	*e_ident;

    e_ident = (unsigned char *)file->mapped_file;

    // Magic numbers
    if (strncmp((char *)e_ident, ELFMAG, SELFMAG))
    {
        fprintf(stderr, "Wrong magic numbers, not an ELF file\n");
        exit(EXIT_FAILURE);
    }

    // File class byte
    if (e_ident[EI_CLASS] == ELFCLASS64)
    {
        ehdr = (Elf64_Ehdr *)e_ident;
        file->ehdr = ehdr;
        file->phdr = (Elf64_Phdr *)(file->mapped_file + file->ehdr->e_phoff);
        file->shdr = (Elf64_Shdr *)(file->mapped_file + file->ehdr->e_shoff);
    // file->phdr = phdr;
    }
    else if (e_ident[EI_CLASS] == ELFCLASS32)
    {
        fprintf(stderr, "32-bit objects are not supported\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        fprintf(stderr, "Invalid file class byt index: %hhu\n", e_ident[EI_CLASS]);
        exit(EXIT_FAILURE);
    }
}

// void    parse_phdr(t_file *file)
// {
//     Elf64_Phdr *phdr;

//     phdr = (Ekf64_Phdr *)(file->mapped_file + file->ehdr->e_phoff);
//     file->phdr = phdr;
//     for (Elf64_Half i = 0; i < file->ehdr->e_phnum; i++)
//     {

//         phdr++;
//     }
// }

int    parse_mapped_file(t_file *file)
{
    parse_hdr(file);
    print_ehdr(file);
    print_phdr(file);
    print_shdr(file);
    // fprintf(stdout, "hello %p %p %lx\n", file, file->ehdr, ((Elf64_Ehdr *)file->ehdr)->e_entry);
    return 0;
}
