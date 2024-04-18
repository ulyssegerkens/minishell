/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:58:39 by ugerkens          #+#    #+#             */
/*   Updated: 2024/02/16 22:42:28 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

# include "libraries.h"

extern sig_atomic_t			g_signal_value;

// ----- CORE ----- //
typedef struct s_shell
{
	t_list					*env;
	char					*home;
	t_list					*allocated_pointers[3];
	t_list					*temporary_files;
	char					*parsing_error;
	bool					in_main_process;
	int						last_status;
}							t_shell;

typedef enum e_prompt_mode
{
	MAIN_PROMPT,
	HEREDOC_PROMPT
}							t_prompt_mode;

// ----- LEXER ----- //
typedef enum e_token_type
{
	TK_ERROR,
	TK_WORD,
	TK_PIPE,
	TK_AND,
	TK_OR,
	TK_REDIRECT_IN,
	TK_REDIRECT_OUT,
	TK_APPEND_OUT,
	TK_HEREDOC,
	TK_SUBSHELL_OPEN,
	TK_SUBSHELL_CLOSE
}							t_token_type;

typedef struct s_token
{
	t_token_type			type;
	char					*value;
}							t_token;

// ----- PARSER ----- //
typedef enum e_ast_type
{
	AST_COMMAND,
	AST_SUBSHELL,
	AST_LOGICAL,
	AST_PIPELINE,
	AST_REDIRECTION,
	AST_SYNTAX_ERROR
}							t_ast_type;

typedef struct s_ast_command
{
	char					**args;
}							t_ast_command;

typedef struct s_ast_pipeline
{
	struct s_ast			*left;
	struct s_ast			*right;
}							t_ast_pipeline;

typedef struct s_ast_logical
{
	t_token_type			operator;
	struct s_ast			*left;
	struct s_ast			*right;
}							t_ast_logical;

typedef struct s_ast_redirection
{
	t_token_type			direction;
	struct s_ast			*child;
	char					*file;
}							t_ast_redirection;

typedef struct s_ast_subshell
{
	struct s_ast			*child;
}							t_ast_subshell;

typedef struct s_ast_syntax_error
{
	char					*unexpected_token;
}							t_ast_syntax_error;

typedef struct s_ast
{
	t_ast_type				type;
	union
	{
		t_ast_command		command;
		t_ast_pipeline		pipeline;
		t_ast_logical		logical;
		t_ast_redirection	redirection;
		t_ast_subshell		subshell;
		t_ast_syntax_error	s_error;
	} u_data;
}							t_ast;

typedef enum e_exp_context
{
	NO_QUOTE,
	IN_SINGLE_QUOTE,
	IN_DOUBLE_QUOTE
}							t_exp_context;

typedef struct s_expander
{
	t_exp_context			context;
	int						i;
	char					*buf;
	int						buf_size;
	int						buf_i;
	t_list					**tokens;
	bool					empty_quotes;
	t_list					*wildcards_position;
}							t_expander;

// ----- EXECUTION ----- //
typedef enum e_execute_end
{
	O_RETURN,
	O_EXIT
}							t_execute_end;

# define READ_END 0
# define WRITE_END 1

// ----- BUILTINS ----- //
typedef int					(*t_builtin_func)(t_ast_command *, t_shell *);

typedef struct s_builtin
{
	char					*name;
	t_builtin_func			func;
}							t_builtin;

// ----- ENVIRONMENT ----- //
# define DEFAULT_PATH "/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin\
:/usr/local/sbin:/opt/bin:/opt/sbin"

typedef struct s_var
{
	char					*name;
	char					*value;
}							t_var;

// ----- ERRORS ----- //
typedef enum e_tracking_scope
{
	ONLY_CHECK,
	SH,
	PROMPT
}							t_tracking_scope;

#endif
