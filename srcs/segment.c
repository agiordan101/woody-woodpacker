#include "woody_woodpacker.h"

elf_phdr    *parse_64phdr(t_file *file)
{
    elf_phdr    *phdr;

    phdr = calloc(file->ehdr->e_phnum + 1, sizeof(elf_phdr));
    memcpy(phdr, file->mapped_file + file->ehdr->e_phoff, file->ehdr->e_phnum * sizeof(elf_phdr));
    return phdr;
}

elf_phdr    *get_last_pt_load(t_file *file)
{
    elf_phdr *phdr;
    elf_phdr *last_pt_load;
    
    last_pt_load = NULL;
    phdr = file->phdr;
    for (Elf64_Half i = 0; i < file->ehdr->e_phnum; i++)
    {
        if (phdr->p_type == PT_LOAD)
            last_pt_load = phdr;
        phdr++;
    }
    return last_pt_load;
}

void        add_phdr(t_file *file)
{
    elf_phdr *last_pt_load;

    last_pt_load = get_last_pt_load(file);

}
// void        

void    print_64phdr(t_file *file)
{
    Elf64_Phdr *phdr;

    phdr = file->phdr;
    for (Elf64_Half i = 0; i < file->ehdr->e_phnum; i++)
    {
        fprintf(stderr, "type: %x flags %u off %lx v_addr %lx paddr %lx p_filesz %lx p_memsz %lx p_align %lu\n", phdr->p_type, phdr->p_flags, phdr->p_offset, phdr->p_vaddr, phdr->p_paddr, phdr->p_filesz, phdr->p_memsz, phdr->p_align);
        phdr++;
    }
}
