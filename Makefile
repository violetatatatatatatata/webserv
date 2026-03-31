# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/16 14:57:51 by avelandr          #+#    #+#              #
#    Updated: 2026/03/31 18:45:21 by avelandr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = webserv

CC          = cc
CFLAGS      = -Wall -Werror -Wextra -g -fsanitize=address
INCLUDES    = -I./inc

SRC_DIR     = srcs
OBJ_DIR     = objs

SRC         = $(shell find $(SRC_DIR) -name "*.c")

OBJ         = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
DEP 		= $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.d, $(SRC))

BOLD      = \033[1m
WHITE	  = \033[37m
BLACK     = \033[1;30m
GREEN     = \033[1;32m
YELLOW    = \033[1;33m
BLUE      = \033[1;34m
MAGENTA   = \033[1;35m
CYAN      = \033[1;36m
RESET     = \033[0m

TOTAL_SRCS := $(words $(SRC))

all: print $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@curr=$$(find $(OBJ_DIR) -type f -name "*.o" 2>/dev/null | wc -l); \
	curr=$$((curr + 1)); \
	percent=$$(( $$curr * 100 / $(TOTAL_SRCS) )); \
	bar_len=$$(( $$percent / 10 )); \
	bar_str=""; i=0; \
	while [ $$i -lt $$bar_len ]; do bar_str="$${bar_str}█"; i=$$((i+1)); done; \
	spaces=""; i=0; \
	rest=$$((10 - $$bar_len)); \
	while [ $$i -lt $$rest ]; do spaces="$${spaces}▒"; i=$$((i+1)); done; \
	printf "\r\033[K$(YELLOW)Generating $(NAME) objects... %-38.38s $(CYAN)$$bar_str$$spaces $(GREEN)$$percent%%$(RESET)" $@
	@$(CC) $(CFLAGS) $(INCLUDES) -MMD -c $< -o $@

print:
	@echo "$(YELLOW)"
	@echo '██╗    ██╗███████╗██████╗ ███████╗███████╗██████╗ ██╗   ██╗'
	@echo '██║    ██║██╔════╝██╔══██╗██╔════╝██╔════╝██╔══██╗██║   ██║'
	@echo '██║ █╗ ██║█████╗  ██████╔╝███████╗█████╗  ██████╔╝██║   ██║'
	@echo '██║███╗██║██╔══╝  ██╔══██╗╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝'
	@echo '╚███╔███╔╝███████╗██████╔╝███████║███████╗██║  ██║ ╚████╔╝ '
	@echo " ╚══╝╚══╝ ╚══════╝╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝  $(RESET) $(CYAN)"
	@echo "==========================================================="
	@echo "           𝙝𝙚𝙘𝙝𝙤 𝙥𝙤𝙧: 𝙘𝙤𝙘𝙤, 𝙙𝙖𝙣𝙞 𝙮 𝙫𝙞𝙤𝙡𝙚𝙩𝙖 :)"
	@echo "===========================================================$(RESET)$(BOLD)"
	@echo '                 _                      _                 '
	@echo '          _     /||       .   .        ||\     _          '
	@echo '         ( }    \||D    `   `     .   C||/    { )         '
	@echo '       | /\__,=_[_]   `  .   . `       [_]_=,__/\ |       '
	@echo '       |_\_  |----|                    |----|  _/_|       '
	@echo '       |  |/ |    |                    |    | \|  |       '
	@echo "       |  /_ |    |                    |    | _\  |       $(RESET)"

# Reglas obligatorias

$(NAME): $(OBJ)
	@echo "\n"
	@echo "$(BLUE)Compiling $(NAME)...$(RESET)"
	@echo ""
	@$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)Done !$(RESET)"

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(BLUE)Objects cleaned.$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(BLUE)Executable cleaned.$(RESET)"

re: fclean all

bonus: all
	@echo ""
	@echo "$(MAGENTA)Dear evaluator: mandatory is bonus now :)$(RESET)"
	
-include $(DEP)
.PHONY: all clean fclean re bonus
