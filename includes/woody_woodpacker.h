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

typedef struct  s_file
{
    char            *mapped_file;
    Elf64_Ehdr      *ehdr;
    Elf64_Phdr      *phdr;
    Elf64_Shdr      *shdr;
}               t_file;

int     parse_mapped_file(t_file *file);
void    print_ehdr(t_file *file);
void    print_phdr(t_file *file);
void    print_shdr(t_file *file);

#endif