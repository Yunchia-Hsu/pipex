# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yhsu <yhsu@hive.student.fi>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/15 16:52:08 by yhsu              #+#    #+#              #
#    Updated: 2024/04/19 16:26:08 by yhsu             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#Name of the program
NAME = pipex

# Compiler and flags
CC = cc
CFLAGS =  -Wall -Werror -Wextra -I./libft -g

BLUE = \033[0;34m
GREEN = \033[0;32m
BRIGHT_WHITE = \033[0;97m
YELLOW = \033[0;33m
END = \033[0m

# Directory locations
LIBFT_DIR = ./libft

SRCS_DIR = srcs/
SRCS_BONUS_DIR = srcs_bonus/
OBJS_DIR = objs/
OBJS_BONUS_DIR = objs_bonus/


# Source file names for mandatory and bonus parts
SRC =  pipex.c \
			free.c \
			error.c \
			init.c \
			child.c \


SRCS_BONUS = pipex_bonus.c \
			free_bonus.c \
			error_bonus.c \
			init_bonus.c \
			child_bonus.c \

# Object files including paths
OBJS = ${addprefix ${OBJS_DIR}, ${SRC:.c=.o}}
#OBJS    = $(SRCS:.c=.o)
OBJS_BONUS = ${addprefix ${OBJS_BONUS_DIR}, ${SRCS_BONUS:.c=.o}}

LIBFT = make -C ./libft

all: ${NAME}

${NAME}: ${OBJS_DIR} ${OBJS} .base #-s silence not print out all files
	@${MAKE} -s -C ${LIBFT_DIR}   
	@${CC} -o ${NAME} ${OBJS} -L ${LIBFT_DIR} -lft
	@echo "${YELLOW} ${NAME} created {mandatory ✅} ${END}"

.base: #check is .base exists if yes clean it and create it
	@rm -f .bonus
	@touch .base

bonus: .bonus

.bonus: ${OBJS_BONUS_DIR} ${OBJS_BONUS}
	@${MAKE} -s -C ${LIBFT_DIR}
	@${CC} -o ${NAME} ${OBJS_BONUS} -L${LIBFT_DIR} -lft
	@rm -f .base
	@touch .bonus
	@echo "${YELLOW} ${NAME} created {bonus ✅} ${END}"

${OBJS_DIR}:
	@mkdir -p ${OBJS_DIR}
	@echo "${GREEN}${OBJS_DIR} directory created${END}"

${OBJS_DIR}%.o: ${SRCS_DIR}%.c
	@${CC} ${CFLAGS} -c $< -o $@

${OBJS_BONUS_DIR}:
	@mkdir -p ${OBJS_BONUS_DIR}
	@echo "${GREEN}${OBJS_BONUS_DIR} directory created${END}"

${OBJS_BONUS_DIR}%.o: ${SRCS_BONUS_DIR}%.c
	@${CC} ${CFLAGS} -c $< -o $@

clean:
	@rm -f ${OBJS} ${BONUS_OBJS}
	@${make} -C ${LIBFT_DIR} clean
	@rm -f .bonus .base
	@echo "${BLUE} $(OBJS_DIR) directory + object files have been removed ${END}"
	@echo "${BLUE} ${OBJS_BONUS_DIR} directory + object files have been removed ${END}"
	@echo "${BLUE} ${LIBFT} and libft object files have been removed ${END}"
 
fclean: clean
	@rm -f ${NAME}
	@echo "${BLUE} ${NAME} removed${END}"

re: fclean all