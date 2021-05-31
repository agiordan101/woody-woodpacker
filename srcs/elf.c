#include "woody_woodpacker.h"

elf_ehdr    *parse_64ehdr(t_file *file)
{
    elf_ehdr    *ehdr;

    ehdr = calloc(1, sizeof(elf_ehdr));
    memcpy(ehdr, file->mapped_file, sizeof(elf_ehdr));
    return ehdr;
}

void    print_64ehdr(t_file *file)
{
    Elf64_Ehdr  *hdr;

    hdr = file->ehdr;
    fprintf(stderr, "e_type: %hu\ne_machine %hu\ne_version %u\ne_entry %lu\n", hdr->e_type, hdr->e_machine, hdr->e_version, hdr->e_entry);
    fprintf(stderr, "e_phoff: %lx\ne_shoff %lx\ne_vflags %u\ne_phnum %hx\ne_shnum %hx\n", hdr->e_phoff, hdr->e_shoff, hdr->e_flags, hdr->e_phnum, hdr->e_shnum);
    fprintf(stderr, "e_shstrndx: %x e_shentsize: %u sizeof(Elf64_Shdr): %zu\n", hdr->e_shstrndx, hdr->e_shentsize, sizeof(Elf64_Shdr));
}
