#include "woody_woodpacker.h"

elf_ehdr    *parse_64ehdr(t_file *file)
{
    elf_ehdr    *ehdr;

    ehdr = calloc(1, sizeof(elf_ehdr));
    memcpy(ehdr, file->mapped_file, sizeof(elf_ehdr));
    return ehdr;
}

int         update_ehdr(t_file *file)
{
    // memcpy(file->bytecode + file->ehdr->e_entry - 0x318, file->payload, file->payload_filesz);
    file->old_entry_point = file->ehdr->e_entry;
    file->ehdr->e_entry = file->payload_phdr.p_vaddr;
    // file->ehdr->e_shoff = 0x0;
    // file->ehdr->e_shnum = 0;
    // file->ehdr->e_shstrndx = 0;
    return 0;
}

// int         update_ehdr(t_file *file)
// // old version inject after all pt_load (not working)
// {
//     // memcpy(file->bytecode + file->ehdr->e_entry - 0x318, file->payload, file->payload_filesz);
//     file->old_entry_point = file->ehdr->e_entry;
//     file->ehdr->e_entry = file->payload_phdr.p_vaddr;
//     file->ehdr->e_shoff = 0x0;
//     file->ehdr->e_shnum = 0;
//     file->ehdr->e_shstrndx = 0;
//     return 0;
// }

void    print_64ehdr(t_file *file)
{
    Elf64_Ehdr  *hdr;

    hdr = file->ehdr;
    fprintf(stderr, "e_type: %hu\ne_machine %hu\ne_version %u\ne_entry %lu\n", hdr->e_type, hdr->e_machine, hdr->e_version, hdr->e_entry);
    fprintf(stderr, "e_phoff: %lx\ne_shoff %lx\ne_vflags %u\ne_phnum %hx\ne_shnum %hx\n", hdr->e_phoff, hdr->e_shoff, hdr->e_flags, hdr->e_phnum, hdr->e_shnum);
    fprintf(stderr, "e_shstrndx: %x e_shentsize: %u sizeof(Elf64_Shdr): %zu\n", hdr->e_shstrndx, hdr->e_shentsize, sizeof(Elf64_Shdr));
}
