/* mknames.c: Extracting macro names.
 * Copyright (C) 1999,2011 by Brian Raiter <breadbox@muppetlabs.com>
 * License GPLv2+: GNU GPL version 2 or later.
 * This is free software; you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 *
 * This program expects to receive a list of defined macros on
 * standard input. From this it extracts the macro names that have one
 * of a predefined set of prefixes, and output some simple C source
 * code listing all of the identified macros, organized by prefixes.
 * This output is then #included in names.c so that the code can
 * access all of the macro values defined on the local machine's
 * version of elf.h.
 *
 * The input to this program is generated by making use of gcc's -dM
 * flag, which causes the preprocessor to output all macros defined in
 * the source code.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* If there's a macro with an 80-character-long name, I don't want to
 * know about it, much less make use of it.
 */
#define MACRO_MAXLEN 80

/*
 * The prefixes array is a list of prefixes used by elf.h macros. Note
 * that the order of prefixes is important, in that a prefix that is a
 * proper subset of another prefix (e.g. "DF_" and "DF_1_") must be
 * listed first.
 */

struct prefix {
    char const *str;
    int len;
};

#define _(PREFIX) { PREFIX, (int)(sizeof PREFIX - 1) }

static struct prefix prefixes[] = {
    _("AT_"),
    _("DF_"),
    _("DF_1_"),
    _("DTF_"),
    _("DT_"),
    _("EF_"),
    _("ELFCLASS"),
    _("ELFDATA"),
    _("ELFMAG"),
    _("ELFOSABI_"),
    _("ELF_NOTE_OS_"),
    _("EM_"),
    _("ET_"),
    _("EV_"),
    _("NT_"),
    _("NT_GNU_"),
    _("PF_"),
    _("PT_"),
    _("R_"),
    _("SHF_"),
    _("SHN_"),
    _("SHT_"),
    _("STB_"),
    _("STN_"),
    _("STT_"),
    _("STV_"),
    _("SYMINFO_BT_"),
    _("SYMINFO_FLG_"),
    _("VER_DEF_"),
    _("VER_FLG_"),
    _("VER_NDX_"),
    _("VER_NEED_")
};

#undef _

#define prefixcount ((int)(sizeof prefixes / sizeof *prefixes))

/*
 * machineids is an array of machine architectures. Many macros are
 * used only for certain architectures, with the various architectures
 * reusing the same values. So in order to avoid using the wrong macro
 * definition, the program must look for a machine-specific infix
 * following the prefix. A machine ID of zero indicates a macro that
 * is not machine-specific. A couple of architectures use more than
 * one identifier, so the code needs to accept either one or two names
 * per machine.
 */

struct machineid {
    char const *str;
    int len;
    char const *str2;
    int len2;
    char const *macro;
};

#define _(INFIX, MACRO) { INFIX, (int)(sizeof INFIX - 1), "", 1, #MACRO }
#define __(IN1, IN2, MACRO) \
    { IN1, (int)(sizeof IN1 - 1), IN2, (int)(sizeof IN2 - 1), #MACRO }

static struct machineid machineids[] = {
    _("", EM_NONE),
    _("386_", EM_386),
    _("390_", EM_S390),
    _("68K_", EM_68K),
    _("ALPHA_", EM_ALPHA),
    _("ARM_", EM_ARM),
    _("CRIS_", EM_CRIS),
    __("HP_", "PARISC_", EM_PARISC),
    __("IA_64_", "IA64_", EM_IA_64),
    _("M32R_", EM_M32R),
    _("MIPS_", EM_MIPS),
    _("MN10300_", EM_MN10300),
    _("PPC_", EM_PPC),
    _("PPC64_", EM_PPC64),
    _("SH_", EM_SH),
    _("SPARC_", EM_SPARC),
    _("X86_64_", EM_X86_64),
};

#define machineidcount ((int)(sizeof machineids / sizeof *machineids))

/* A macro is identified entirely by its name, as this program neither
 * has nor needs access to the values of the macros. If hi is nonzero,
 * then this macro is recognized as defining the lower bound of a
 * range of values, and hi is the offset to the macro defining the
 * range's upper bound.
 */
struct name {
    char const *str;
    int	hi;
};

/* Each list of macro names is stored in separate array, which can be
 * different in size from any other list.
 */
struct namelist {
    struct name *names;
    int count;
};

/* The repository: this array contains all identified macro names,
 * broken down by prefix and machine architecture.
 */
static struct namelist namelists[prefixcount][machineidcount];

/* Adds a macro name to the global array, under the given prefix and
 * machine subsection.
 */
static void addnametolist(char *name, int p, int m)
{
    struct namelist *list;
    int n;

    list = &namelists[p][m];
    n = list->count++;
    list->names = realloc(list->names, list->count * sizeof *list->names);
    list->names[n].str = name;
    list->names[n].hi = 0;
}

/* Extracts the macro name from a single line of input. Function-like
 * macros, macros that define string values, and macros that do not
 * begin with one of the prefixes are all ignored. Macros that pass
 * these filters are checked for a machine architecture infix and then
 * added to the global array.
 */
static int readdefine(char const *line)
{
    char *name, *infix;
    int namepos, endpos, size;
    char sep;
    int p, m;

    sep = '(';
    sscanf(line, "# define %n%*[^ \t(]%n%c", &namepos, &endpos, &sep);
    if (sep == '(')
	return 0;
    if (strchr(line + endpos, '"'))
	return 0;
    size = endpos - namepos;
    if (size >= MACRO_MAXLEN)
	return 0;
    name = malloc(size + 1);
    memcpy(name, line + namepos, size);
    name[size] = '\0';
    for (p = prefixcount - 1 ; p >= 0 ; --p)
	if (!memcmp(name, prefixes[p].str, prefixes[p].len))
	    break;
    if (p < 0) {
	free(name);
	return 0;
    }
    m = 0;
    if (strcmp(prefixes[p].str, "EM_")) {
	infix = name + prefixes[p].len;
	for (m = machineidcount - 1 ; m > 0 ; --m) {
	    if (!memcmp(infix, machineids[m].str, machineids[m].len))
		break;
	    else if (!memcmp(infix, machineids[m].str2, machineids[m].len2))
		break;
	}
    }
    addnametolist(name, p, m);
    return 1;
}

/* Extracts all macro names from the given input file. (Note that long
 * lines need to be handled carefully, since function-like macros are
 * sometimes quite sizeable.)
 */
static void readinput(FILE *file)
{
    char buf[1024];
    int n;

    while (fgets(buf, sizeof buf, file)) {
	readdefine(buf);
	n = strlen(buf);
	if (n == sizeof buf - 1 && buf[n - 1] != '\n') {
	    int ch;
	    while ((ch = fgetc(file)) != EOF && ch != '\n') ;
	}
    }
}

/* Takes a list of macros and the index of one macro that contains the
 * substring "LO", and searches for a macro with the same name but
 * with the "LO" replaced with "HI". If one is found, the offset to
 * the other macro in the array is returned.
 */
static int findhirange(struct name const *names, int count, int lo)
{
    char hiname[MACRO_MAXLEN];
    char *p;
    int hi;

    strcpy(hiname, names[lo].str);
    p = strstr(hiname, "LO");
    p[0] = 'H';
    p[1] = 'I';
    for (hi = 0 ; hi < count ; ++hi)
	if (hi != lo && !strcmp(names[hi].str, hiname))
	    return hi - lo;
    return 0;
}

/* Identifies all range-definition macros in a list.
 */
static void findranges(struct name *names, int count)
{
    int i;

    for (i = 0 ; i < count ; ++i)
	if (strstr(names[i].str, "LO"))
	    names[i].hi = findhirange(names, count, i);
}

/* Identifies all range-definition macros in the global array.
 */
static void findallranges(void)
{
    int p, m;

    for (p = 0 ; p < prefixcount ; ++p)
	for (m = 0 ; m < machineidcount ; ++m)
	    findranges(namelists[p][m].names, namelists[p][m].count);
}

/* Outputs the macro names from a namelist in the form of C code
 * initializing an array of structs.
 */
static void outputnamelist(FILE *file, struct namelist const *list)
{
    int i;

    for (i = 0 ; i < list->count ; ++i) {
	if (list->names[i].hi)
	    fprintf(file, "  { \"%s\", %s, %s - %s },\n",
			  list->names[i].str,
			  list->names[i].str,
			  list->names[i + list->names[i].hi].str,
			  list->names[i].str);
	else
	    fprintf(file, "  { \"%s\", %s, 0 },\n",
			  list->names[i].str,
			  list->names[i].str);
    }
}

/* Outputs all the macro names found in the input as C source code.
 * First the prefixes and the machine architecture IDs are output as
 * simple arrays. Then each list of macro names is output as a
 * separate array. Finally the source for a single two-dimensional
 * array is output, using pointers to the name lists defined
 * previously to populate the array.
 */
static void outputsource(FILE *file)
{
    int p, m, i;

    fprintf(file, "/* This file is automatically generated by mknames. */\n");
    fprintf(file, "enum { prefixcount = %d };\n", prefixcount);
    fprintf(file, "static char const *prefixes[prefixcount] = {\n");
    for (i = 0 ; i < prefixcount ; ++i)
	fprintf(file, "  \"%s\",\n", prefixes[i].str);
    fprintf(file, "};\n");
    fprintf(file, "enum { machineidcount = %d };\n", machineidcount);
    fprintf(file, "static int machineids[machineidcount] = {\n");
    for (i = 0 ; i < machineidcount ; ++i)
	fprintf(file, "  %s,\n", machineids[i].macro);
    fprintf(file, "};\n");

    for (p = 0 ; p < prefixcount ; ++p) {
	for (m = 0 ; m < machineidcount ; ++m) {
	    if (namelists[p][m].count == 0)
		continue;
	    fprintf(file, "static struct name _na_%s%s[] = {\n",
			  prefixes[p].str, machineids[m].str);
	    outputnamelist(file, &namelists[p][m]);
	    fprintf(file, "};\n");
	}
    }


    fprintf(file, "static struct nameset"
		  " namesets[prefixcount][machineidcount] = {\n");
    for (p = 0 ; p < prefixcount ; ++p) {
	fprintf(file, "  {\n");
	for (m = 0 ; m < machineidcount ; ++m) {
	    if (namelists[p][m].count) {
		fprintf(file, "    { _na_%s%s,"
			      " sizeof _na_%s%s / sizeof *_na_%s%s },\n",
			      prefixes[p].str, machineids[m].str,
			      prefixes[p].str, machineids[m].str,
			      prefixes[p].str, machineids[m].str);
	    } else {
		fprintf(file, "    { NULL, 0 },\n");
	    }
	}
	fprintf(file, "  },\n");
    }
    fprintf(file, "};\n");
}

/* main().
 */
int main(int argc, char *argv[])
{
    FILE *file;

    if (argc != 2)
	return 1;
    readinput(stdin);
    findallranges();
    file = fopen(argv[1], "w");
    if (!file) {
	perror(argv[1]);
	return 1;
    }
    outputsource(file);
    return 0;
}
