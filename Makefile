# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: avelandr <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/02 17:53:24 by avelandr          #+#    #+#              #
#    Updated: 2026/04/02 17:57:42 by avelandr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = webserv

CXX         = c++
CXXFLAGS    = -Wall -Werror -Wextra -g -fsanitize=address -std=c++98
INCLUDES    = -Iinc -Iinc/http -Iinc/core -Iinc/parse

SRC_DIR     = srcs
OBJ_DIR     = objs

SRC         = $(shell find $(SRC_DIR) -name "*.cpp")

OBJ         = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))
DEP 		= $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.d, $(SRC))

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

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@curr=$$(find $(OBJ_DIR) -type f -name "*.o" 2>/dev/null | wc -l); \
	curr=$$((curr + 1)); \
	percent=$$(( $$curr * 100 / $(TOTAL_SRCS) )); \
	bar_len=$$(( $$percent * 59 / 100 )); \
	bar_str=""; i=0; \
	while [ $$i -lt $$bar_len ]; do bar_str="$${bar_str}█"; i=$$((i+1)); done; \
	spaces=""; i=0; \
	rest=$$((59 - $$bar_len)); \
	while [ $$i -lt $$rest ]; do spaces="$${spaces}▒"; i=$$((i+1)); done; \
	printf "\r\033[K$(YELLOW)Generating $(NAME) objects... %-38.38s\n\r\033[K$(CYAN)$$bar_str$$spaces $(GREEN)$$percent%%$(RESET)\033[A" $@
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -c $< -o $@

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
	@echo '                  _                      _                 '
	@echo '          _     /||.   .        ||\     _          '
	@echo '         ( }    \||D    `   `     .   C||/    { )         '
	@echo '       |/\__,=_[_]   `  .   . `       [_]_=,__/\ |'
	@echo '       |_\_  |----|                    |----|  _/_|'
	@echo '       |  |/ |    |                    |    | \|  |'
	@echo "       |  /_ |    |                    |    | _\  |       $(RESET)"
	@echo ""

$(NAME): $(OBJ)
	@echo ""
	@echo "\n"
	@echo "$(BLUE)Compiling $(NAME)...$(RESET)"
	@echo ""
	@$(CXX) $(CXXFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME)
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
