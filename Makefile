# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/16 14:57:51 by avelandr          #+#    #+#              #
#    Updated: 2026/03/28 14:17:27 by avelandr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = webserv

CC          = cc
CFLAGS      = -Wall -Werror -Wextra -g -fsanitize=address
INCLUDES    = -I./inc -I./libs/libft/Includes

SRC_DIR     = srcs
OBJ_DIR     = objs

SRC         = $(shell find $(SRC_DIR) -name "*.c")

OBJ         = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
DEP 		= $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.d, $(SRC))

BOLD      = \033[1m
BLACK     = \033[1;30m
GREEN     = \033[1;32m
YELLOW    = \033[1;33m
BLUE      = \033[1;34m
MAGENTA   = \033[1;35m
CYAN      = \033[1;36m
RESET     = \033[0m

TOTAL_SRCS := $(words $(SRC))

all: ws print libft $(NAME)

ws:
	@echo "$(YELLOW)"
	@echo '██╗    ██╗███████╗██████╗ ███████╗███████╗██████╗ ██╗   ██╗'
	@echo '██║    ██║██╔════╝██╔══██╗██╔════╝██╔════╝██╔══██╗██║   ██║'
	@echo '██║ █╗ ██║█████╗  ██████╔╝███████╗█████╗  ██████╔╝██║   ██║'
	@echo '██║███╗██║██╔══╝  ██╔══██╗╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝'
	@echo '╚███╔███╔╝███████╗██████╔╝███████║███████╗██║  ██║ ╚████╔╝ '
	@echo ' ╚══╝╚══╝ ╚══════╝╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝  '	
	@echo ""
	@echo "========================================================="
	@echo "  by: coco dani y violeta :p (poner logins aqui)"
	@echo "========================================================="
	@echo "$(RESET)"

print:
	@echo "$(YELLOW)"
	@echo '           _                      _           '
	@echo '    _     /||       .   .        ||\     _    '
	@echo '   ( }    \||D    '   '     '   C||/    { )   '
	@echo ' | /\__,=_[_]   '  .   . '       [_]_=,__/\ | '
	@echo ' |_\_  |----|                    |----|  _/_| '
	@echo ' |  |/ |    |                    |    | \|  | '
	@echo ' |  /_ |    |                    |    | _\  | '
	@echo "$(RESET)"

libft:
	@echo ""
	@echo "$(BLUE)Building libft...$(RESET)"
	@make -C ./libs/libft --no-print-directory

$(NAME): $(OBJ)
	@echo ""
	@echo "$(BLUE)$(NAME) done!$(RESET)"
	@echo ""
	@echo "$(BLUE)Linking objects...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)Exercise $(NAME) compiled successfully!$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -MMD -c $< -o $@
	@curr=$$(find $(OBJ_DIR) -type f -name "*.o" 2>/dev/null | wc -l); \
	percent=$$(( $$curr * 100 / $(TOTAL_SRCS) )); \
	bar_len=$$(( $$percent / 2 )); \
	bar_str=""; \
	i=0; \
	while [ $$i -lt $$bar_len ]; do bar_str="$${bar_str}▓"; i=$$((i+1)); done; \
	spaces=""; \
	i=0; \
	rest=$$((50 - $$bar_len)); \
	printf "\r$(BLUE)Compiling $(NAME): $(YELLOW)[$$bar_str$$spaces] $(YELLOW)$$percent%% $(RESET)"

clean:
	@rm -rf $(OBJ_DIR)
	@make -C ./libs/libft clean  --no-print-directory
	@echo "$(BLUE)Objects cleaned.$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@make -C ./libs/libft fclean  --no-print-directory
	@if [ -f "./libs/MLX42/libmlx42.a" ]; then rm -f ./libs/MLX42/libmlx42.a; fi
	@echo "$(BLUE)Executable cleaned.$(RESET)"

re: fclean all

bonus: all
	@echo ""
	@echo "$(MAGENTA)Dear evaluator: mandatory is bonus now :)$(RESET)"
	
-include $(DEP)
.PHONY: all clean fclean re libft bonus
