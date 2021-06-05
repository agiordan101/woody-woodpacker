#include "woody_woodpacker.h"

// 00004000  f3 0f 1e fa 55 ba 01 00  00 00 bf 01 00 00 00 48  |....U..........H|
// 00004010  83 ec 10 64 48 8b 04 25  28 00 00 00 48 89 44 24  |...dH..%(...H.D$|
// 00004020  08 31 c0 48 8d 6c 24 07  c6 44 24 07 38 48 89 ee  |.1.H.l$..D$.8H..|
// 00004030  e8 9b f3 ff ff bf 01 00  00 00 ba 01 00 00 00 48  |...............H|
// 00004040  89 ee c6 44 24 07 0a e8  84 f3 ff ff 31 ff e8 1d  |...D$.......1...|
// 00004050  f4 ff ff 66 66 2e 0f 1f  84 00 00 00 00 00 66 90  |...ff.........f.|


// void    payload()
// {
//     char c = '8';

//     int ret = write(1, &c, 1);
//     c = '\n';
//     ret = write(1, &c, 1);
//     exit(0);
//     (void)ret;
// }

int     setup_payload(t_file *file)
{
    elf_phdr    *phdr_infect;
    // Elf64_Addr  p_vaddr;
    Elf64_Off   end_offset_aligned;
    char        *payload_memaddr;
    Elf64_Off   old_entry_point_offset;

    file->payload = (char *)payload;
    file->payload_filesz = g_payload_len;

    phdr_infect = find_unused_pt_load_space(file, file->payload_filesz);

    end_offset_aligned = get_phdr_end_offset_aligned(phdr_infect);
    file->payload_entry = end_offset_aligned + phdr_infect->p_vaddr - phdr_infect->p_offset + g_payload_offset;
    // file->payload_entry = end_offset_aligned + phdr_infect->p_vaddr - phdr_infect->p_offset;
    phdr_infect->p_filesz += GET_ALIGN16(phdr_infect->p_filesz) + file->payload_filesz;
    phdr_infect->p_memsz += GET_ALIGN16(phdr_infect->p_memsz) + file->payload_filesz;
    phdr_infect->p_flags = 7;

    fprintf(stderr, "old %x new %x\n", g_payload_len, g_payload_offset);

    file->old_entry_point = file->ehdr->e_entry;
    file->ehdr->e_entry = file->payload_entry;

    payload_memaddr = file->bytecode + (end_offset_aligned - sizeof(Elf64_Ehdr) - (file->ehdr->e_phentsize * file->ehdr->e_phnum));
    memcpy(payload_memaddr, file->payload, file->payload_filesz);
    old_entry_point_offset = file->old_entry_point - (file->ehdr->e_entry + 0x34);
    memcpy(payload_memaddr + 0x35, (char *)&old_entry_point_offset, sizeof(Elf64_Off));

    fprintf(stderr, "jmp_offset %x, bytecode %hhu %hhu %hhu %hhu\n", g_payload_jmp_offset, *(payload_memaddr + 0x35), *(payload_memaddr + 0x36), *(payload_memaddr + 0x37), *(payload_memaddr + 0x38));

    fprintf(stderr, "old %lx new %lx\n", file->old_entry_point, file->ehdr->e_entry);

    return 1;
}

// int     setup_payload(t_file *file)
// // old version inject after all pt_load (not working)
// {
//     elf_phdr    *last_pt_load;
//     Elf64_Off   p_offset;
//     Elf64_Addr  p_vaddr;
//     Elf64_Off   page_align;

//     file->payload = (char *)payload;
//     file->payload_filesz = (unsigned long)setup_payload - (unsigned long)payload;

//     last_pt_load = get_last_pt_load(file);
//     p_offset = last_pt_load->p_offset + last_pt_load->p_filesz;

//     if (p_offset % getpagesize())
//         page_align = getpagesize() - p_offset % getpagesize();
//     else
//         page_align = 0;
//     p_offset = p_offset + page_align;
//     // battage de couille
//     p_vaddr = last_pt_load->p_vaddr + last_pt_load->p_filesz + page_align;

//     file->payload_phdr = (elf_phdr) {
//         PT_LOAD,
//         7,
//         p_offset,
//         p_vaddr,
//         p_vaddr,
//         file->payload_filesz,
//         file->payload_filesz,
//         getpagesize()
//     };
//     return 0;
// }