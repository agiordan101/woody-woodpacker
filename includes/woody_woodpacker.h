#ifndef WOODY_WOODPACKER_H
# define WOODY_WOODPACKER_H


# include <sys/mman.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <elf.h>

# define USAGE "./woody_woodpacker <file_to_pack>"

typedef Elf64_Ehdr elf_ehdr;
typedef Elf64_Phdr elf_phdr;
typedef Elf64_Shdr elf_shdr;

typedef struct      s_file
{
    char            *mapped_file;
    char            *bytecode;
    elf_ehdr        *ehdr;
    elf_phdr        *phdr;
    elf_shdr        *shdr;
    Elf64_Off       b_offset;
    Elf64_Off       b_filesz;
    char            *payload;
    elf_phdr        *payload_phdr;
    Elf64_Off       payload_filesz;
    Elf64_Off       size;
}                   t_file;



int         parse_elf(t_file *file);
elf_ehdr    *parse_64ehdr(t_file *file);
elf_phdr    *parse_64phdr(t_file *file);
elf_shdr    *parse_64shdr(t_file *file);
int         parse_bytecode(t_file *file);

char    *empty_bytecode_space(t_file *file, Elf64_Off payload_size);


void    print_64ehdr(t_file *file);
void    print_64phdr(t_file *file);
void    print_64shdr(t_file *file);

#endif