# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yhsu <yhsu@hive.student.fi>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/15 16:52:08 by yhsu              #+#    #+#              #
#    Updated: 2024/04/15 17:43:42 by yhsu             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Name of the program
NAME = pipex

# Compiler and flags
CC = CC
CFLAGS = -Wall -Werror -Wextra

# Color scheme
BLUE = \033[0;34m
GREEN = \033[0;32m
BRIGHT_WHITE := \033[0;97m
END = \033[0m

# Directory locations
LIBFT_DIR = ./libft
SRCS_BONUS_DIR = ./SRCS_BONUS_DIR


# Source file names for mandatory and bonus parts
SRCS_BONUS = pipex_bonus.c \
				error_free_bonus.c \

# Object files including paths
OBJECTS_BONUS = ${SRCS_BONUS:.c=.o}
LIBFT = $(LIBFT_DIR)libft.and

all: ${NAME}

#$(NAME): $(OBJS_DIR) $(OBJS) .base
#	@$(MAKE) -s -C $(LIBFT_DIR)
#	@$(CC) -o $(NAME) $(OBJS) -L$(LIBFT_DIR) -lft
#	@echo "$(COLOR_BRIGHT_WHITE)$(NAME) created (for mandatory part)$(COLOR_END)"

#.base:
#	@rm -f .bonus
#	@touch .base
#
#bonus: .bonus

.bonus: $(OBJS_DIR) $(OBJS_BONUS)
	@$(MAKE) -s -C $(LIBFT_DIR)
	@$(CC) -o $(NAME) $(OBJS_BONUS) -L$(LIBFT_DIR) -lft
	@rm -f .base
	@touch .bonus
	@echo "$(COLOR_BRIGHT_WHITE)$(NAME) created (for bonus part)$(COLOR_END)"

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)
	@echo "$(COLOR_WHITE)$(OBJS_DIR) directory created$(COLOR_END)"

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)%.o: $(SRCS_BONUS_DIR)%.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -fr $(OBJS_DIR)
	@$(MAKE) -s -C $(LIBFT_DIR) fclean
	@rm -f .bonus
	@rm -f .base
	@echo "$(COLOR_BLUE)$(OBJS_DIR) directory and object files removed$(COLOR_END)"
	@echo "$(COLOR_BLUE)$(LIBFT) and libft object files removed$(COLOR_END)"
 
fclean: clean
	@rm -f $(NAME)
	@echo "$(COLOR_BLUE)$(NAME) removed$(COLOR_END)"

re: fclean all