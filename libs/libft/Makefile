# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: avelandr <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/29 12:07:13 by avelandr          #+#    #+#              #
#    Updated: 2026/01/28 17:29:51 by avelandr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BOLD      = \033[1m
BLACK     = \033[1;30m
GREEN     = \033[1;32m
YELLOW    = \033[1;33m
BLUE      = \033[1;34m
MAGENTA   = \033[1;35m
CYAN      = \033[1;36m
RESET     = \033[0m

NAME        = libft.a
CC          = cc
CFLAGS      = -Wall -Wextra -Werror

SRCS = \
	convs/ft_atoi.c convs/ft_itoa.c \
	mem/ft_bzero.c mem/ft_calloc.c mem/ft_memchr.c mem/ft_memcmp.c \
	mem/ft_memcpy.c mem/ft_memmove.c mem/ft_memset.c \
	comps/ft_isalnum.c comps/ft_isalpha.c comps/ft_isascii.c \
	comps/ft_isdigit.c comps/ft_isprint.c \
	fd/ft_putchar_fd.c fd/ft_putendl_fd.c fd/ft_putnbr_fd.c fd/ft_putstr_fd.c \
	strings/ft_strdup.c strings/ft_strchr.c strings/ft_strlcat.c \
	strings/ft_strlcpy.c strings/ft_strlen.c strings/ft_strncmp.c \
	strings/ft_strnstr.c strings/ft_tolower.c strings/ft_toupper.c \
	strings/ft_substr.c strings/ft_strjoin.c strings/ft_strtrim.c \
	strings/ft_strcmp.c strings/ft_split.c strings/ft_strrchr.c \
	list/ft_lstnew_bonus.c list/ft_lstadd_back_bonus.c \
	list/ft_lstclear_bonus.c list/ft_lstdelone_bonus.c \
	list/ft_lstiter_bonus.c list/ft_lstlast_bonus.c list/ft_lstsize_bonus.c \
	get_next_line/ft_get_next_line.c get_next_line/ft_get_next_line_utils.c \
	get_next_line/ft_get_next_line_utils_2.c is_definition/ft_isblank.c is_definition/ft_ismeta.c \
	is_definition/ft_is_redirection.c is_definition/is_controlop.c is_definition/is_redop.c is_definition/is_word.c is_definition/ft_is_builtin.c \
	is_definition/ft_is_pipe.c is_definition/ft_isspace.c is_definition/is_filename.c is_definition/is_token.c

OBJS = $(SRCS:.c=.o)
TOTAL_SRCS := $(words $(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $@ $^
	@echo ""
	@echo "$(BLUE)Libft done! $(RESET)"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@curr=$$(find . -name "*.o" | wc -l); \
	percent=$$(( $$curr * 100 / $(TOTAL_SRCS) )); \
	bar_len=$$(( $$percent / 2 )); \
	bar_str=""; \
	i=0; \
	while [ $$i -lt $$bar_len ]; do bar_str="$${bar_str}▓"; i=$$((i+1)); done; \
	spaces=""; \
	i=0; \
	rest=$$((50 - $$bar_len)); \
	while [ $$i -lt $$rest ]; do spaces="$${spaces}░"; i=$$((i+1)); done; \
	printf "\r$(BLUE)Compiling Libft:     $(YELLOW)[$$bar_str$$spaces] $(YELLOW)$$percent%% $(RESET)"

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
