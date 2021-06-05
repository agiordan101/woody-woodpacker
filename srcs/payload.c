#include "woody_woodpacker.h"

void    payload()
{
    char c = '8';

    int ret = write(1, &c, 1);
    c = '\n';
    ret = write(1, &c, 1);
    exit(0);
    (void)ret;
}

int     setup_payload(t_file *file)
{
    elf_phdr    *last_pt_load;
    Elf64_Off   p_offset;
    Elf64_Addr  p_vaddr;
    Elf64_Off   page_align;

    file->payload = (char *)payload;
    file->payload_filesz = (unsigned long)setup_payload - (unsigned long)payload;

    last_pt_load = get_last_pt_load(file);
    p_offset = last_pt_load->p_offset + last_pt_load->p_filesz;

    if (p_offset % getpagesize())
        page_align = getpagesize() - p_offset % getpagesize();
    else
        page_align = 0;
    p_offset = p_offset + page_align;
    // battage de couille
    p_vaddr = last_pt_load->p_vaddr + last_pt_load->p_filesz + page_align;

    file->payload_phdr = (elf_phdr) {
        PT_LOAD,
        7,
        p_offset,
        p_vaddr,
        p_vaddr,
        file->payload_filesz,
        file->payload_filesz,
        getpagesize()
    };
    return 0;
}