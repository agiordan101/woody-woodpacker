# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2018/12/10 21:06:37 by pduhard-     #+#   ##    ##    #+#        #
#    Updated: 2019/03/04 11:32:24 by pduhard-    ###    #+. /#+    ###.fr      #
#                                                          /                   #
#                                                         /                    #
# **************************************************************************** #

##__________CONFIG__________#

NAME		=	woody_woodpacker

CC			=	gcc
NASM		= 	nasm
CFLAGS		=	-fPIC -Wall -Werror -Wextra -O3
SFLAGS		=	-f elf64 -O3
INC_DIR		=	./includes/
INCLUDES	=	woody_woodpacker.h

SRC_PATH	=	./srcs/

BIN_PATH	=	./bins/

SRC			=	main.c		\
				parse.c		\
				elf.c		\
				segment.c	\
				section.c	\
				bytecode.c	\

ASM_SRC		=	unpack.s	\

BIN			=	$(SRC:.c=.o)
ASM_BIN		=	$(ASM_SRC:.s=.o)

BINS		=	$(addprefix $(BIN_PATH), $(BIN))
ASM_BINS	=	$(addprefix $(BIN_PATH), $(ASM_BIN))
INCS		=	$(addprefix $(INC_DIR), $(INCLUDES))

.PHONY: all clean fclean re

#__________COLORS__________#

R			=	\033[0;31m
G			=	\033[32;7m
B			=	\033[0;34m
N			=	\33[0m

#__________RULES__________#

all: $(NAME)

$(NAME): $(BINS) # $(ASM_BINS)
	@$(CC) $(CFLAGS) -o $@ $^ -I $(INC_DIR)
	@echo "\n\n$(B)[EXECUTABLE \"$(NAME)\" READY]\n"


$(BIN_PATH)%.o: $(SRC_PATH)%.c $(INCS)

	@mkdir -p $(BIN_PATH) || true
	@$(CC) $(CFLAGS) -I $(INC_DIR) -o $@ -c $< && echo "${G} \c"

$(BIN_PATH)%.o: $(SRC_PATH)%.s $(INCS)

	@mkdir -p $(BIN_PATH) || true
	@$(NASM) $(SFLAGS) -I $(INC_DIR) -o $@ $< && echo "${G} \c"

clean:

	@echo "${R}[CLEANING $(NAME) BINARIES]\n"
	@rm -Rf $(BIN_PATH)

fclean: clean

	@echo "\n${R}[REMOVING \"$(NAME)\"]"
	@rm -f $(NAME)

re: fclean all