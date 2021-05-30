#include "woody_woodpacker.h"

void    print_ehdr(t_file *file)
{
    Elf64_Ehdr  *hdr;

    hdr = file->ehdr;
    fprintf(stderr, "e_type: %hu\ne_machine %hu\ne_version %u\ne_entry %lu\n", hdr->e_type, hdr->e_machine, hdr->e_version, hdr->e_entry);
    fprintf(stderr, "e_phoff: %lu\ne_shoff %lu\ne_vflags %u\ne_phnum %hu\ne_shnum %hu\n", hdr->e_phoff, hdr->e_shoff, hdr->e_flags, hdr->e_phnum, hdr->e_shnum);
    fprintf(stderr, "e_shstrndx: %x e_shentsize: %u sizeof(Elf64_Shdr): %zu\n", hdr->e_shstrndx, hdr->e_shentsize, sizeof(Elf64_Shdr));
    
}

void    print_phdr(t_file *file)
{
    Elf64_Phdr *phdr;

    phdr = file->phdr;
    for (Elf64_Half i = 0; i < file->ehdr->e_phnum; i++)
    {
        fprintf(stderr, "type :%x flags %u off %lx v_addr %lx paddr %lx p_filesz %lx p_memsz %lx p_align %lu\n", phdr->p_type, phdr->p_flags, phdr->p_offset, phdr->p_vaddr, phdr->p_paddr, phdr->p_filesz, phdr->p_memsz, phdr->p_align);
        phdr++;
    }
}

void    print_shdr(t_file *file)
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
