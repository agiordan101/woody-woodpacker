#include "woody_woodpacker.h"

int     parse_elf(t_file *file)
{
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
        file->ehdr = parse_64ehdr(file);
        file->phdr = parse_64phdr(file);
        file->shdr = parse_64shdr(file);
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
    parse_bytecode(file);
  
    fprintf(stdout, "section header: %lx\n", file->ehdr->e_shoff + file->ehdr->e_shnum * sizeof(Elf64_Shdr));

    print_64ehdr(file);
    print_64phdr(file);
    print_64shdr(file);
    return 1;
}
