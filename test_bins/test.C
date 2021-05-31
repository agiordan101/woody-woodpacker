#include <unistd.h>
#include <stdio.h>

void    print_yo(void)
{
    write(1, "yo\n", 3);
    write(1, "superyo\n", 8);
}

int main(void)
{
    char *tst = (char *)print_yo;
    int len = (unsigned long)main - (unsigned long)print_yo;
    for (int i = 0; i < len; i++)
    {
        printf("%02hhx ", tst[i]);
    }
    printf("\n %p\n", print_yo);
    // printf("%s\n", (char *)print_yo);
    print_yo();

}