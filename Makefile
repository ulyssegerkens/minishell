# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ugerkens <ugerkens@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/20 00:10:02 by fvastena          #+#    #+#              #
#    Updated: 2024/04/18 12:45:26 by ugerkens         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and flags
CC				=	cc
CFLAGS			=	-Wall -Wextra -Werror -O3 -MMD -MP
RM				=	rm -rf
NAME			=	minishell

# Libraries
LIBFT_DIR		=	lib/libft
LIBFT			=	$(LIBFT_DIR)/libft.a
READLINE_DIR	=	$(HOME)/.brew/opt/readline
READLINE_LIB	=	-L$(READLINE_DIR)/lib -lreadline

# Include directories
INC_DIRS		=	include src $(LIBFT_DIR)/include $(READLINE_DIR)/include/readline
CFLAGS			+=	$(addprefix -I, $(INC_DIRS))
LDFLAGS			=	-L$(LIBFT_DIR) -lft $(READLINE_LIB)

# Source and Object Files
VPATH			=	src src/core src/lexer src/parser \
					src/expander src/executor src/builtins \
					src/environment src/signals src/utils

# Source and Object Files
SRC_CORE		=	main.c init_shell.c interactive_shell.c prompt_execution.c \
					quit_shell.c
SRC_LEXER		=	lexer.c lexer_utils.c scan_next_token.c
SRC_PARSER		=	parser.c parse_tk_link.c parse_tk_exe.c \
					parse_tk_redirection.c parse_tk_redirection_utils.c \
					utils_token.c ast_node_init.c ast_node_builders.c
SRC_EXPANDER	=	expander.c expand_one_arg.c expander_utils.c \
					expand_var.c expand_var_env.c \
					expand_filename.c expand_filename_pattern.c expand_filename_utils.c 
SRC_EXECUTOR	=	executor.c read_heredocs.c \
					exe_command.c exe_pipeline.c exe_pipeline_utils.c \
					exe_redirection.c \
					exe_logical.c exe_subshell.c \
					get_bin_path.c get_bin_path_utils.c get_builtin_fct.c
SRC_BUILTINS	=	cd.c echo.c env.c exit.c export.c pwd.c unset.c
SRC_ENV			=	init_env.c access_env.c export_env.c
SRC_SIGNALS		=	set_signals.c handle_signals.c
SRC_UTILS		=	error.c error_syntax.c \
					secure_allocation.c secure_list.c \
					secure_libft.c \
					secure_syscall_file.c secure_syscall_process.c secure_syscall_dir.c \
					utils_general.c

SRC				=	$(SRC_CORE) $(SRC_LEXER) $(SRC_PARSER) \
					$(SRC_EXPANDER) $(SRC_EXECUTOR) $(SRC_BUILTINS) \
					$(SRC_ENV) $(SRC_SIGNALS) $(SRC_UTILS)

# Source and Object Files
OBJ			 	=	$(SRC:%.c=build/%.o)
DEP			 	=	$(OBJ:%.o=%.d)
OBJ_DIR_NAME	=	build

# Rules
all:			$(NAME)

$(NAME):		$(LIBFT) $(OBJ_DIR_NAME) $(OBJ)
				$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@

$(LIBFT):
				$(MAKE) --no-print-directory -C $(LIBFT_DIR)

$(OBJ_DIR_NAME):
				mkdir -p $(OBJ_DIR_NAME)

build/%.o:		%.c
				$(CC) $(CFLAGS) -c $< -o $@

clean:
				$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean
				$(RM) build

fclean:			clean
				$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean
				$(RM) $(NAME)

re:				fclean all

-include $(DEP)

.PHONY:		 all clean fclean re

.SILENT:
