#include "woody_woodpacker.h"

char *mmap_file(char *file_name)
{
	int			fd;
	struct stat	stat_buf;
	char		*mapped_file;
	off_t		file_size;

	mapped_file = NULL;
	if ((fd = open(file_name, O_RDONLY)) < 0 || fstat(fd, &stat_buf) < 0)
	{
		fprintf(stderr, "error: woody_woodpacker: can't open file: %s\n", file_name);
		return (NULL);
	}
	file_size = stat_buf.st_size;
	if (!file_size)
		fprintf(stderr, "error: woody_woodpacker: empty file: %s\n", file_name);
	else if ((mapped_file = (char *)mmap(NULL, stat_buf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		fprintf(stderr, "error: woody_woodpcker: can't map file: %s\n", file_name);
		return (NULL);
	}
	close(fd);
	return (mapped_file);
}



int main(int argc, char **argv)
{
	t_file	file;

	if (argc != 2)
	{
		fputs(USAGE, stdout);
		exit(EXIT_FAILURE);
	}
	file.mapped_file = mmap_file(argv[1]);
	if (!parse_elf(&file))
	{
		fprintf(stdout, "pas fou\n");
		exit(EXIT_FAILURE);
	}
	return (0);
}