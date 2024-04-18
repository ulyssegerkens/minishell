/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_node_builders.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:37:57 by ugerkens          #+#    #+#             */
/*   Updated: 2024/02/01 16:37:58 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*create_ast_command(char **argv, t_shell *sh)
{
	t_ast	*node;

	init_ast_node(&node, AST_COMMAND, sh);
	node->u_data.command.args = argv;
	return (node);
}

t_ast	*create_ast_subshell(t_ast *child, t_shell *sh)
{
	t_ast	*node;

	if (!child)
		return (set_syntax_error("(", sh));
	init_ast_node(&node, AST_SUBSHELL, sh);
	node->u_data.subshell.child = child;
	return (node);
}

t_ast	*create_ast_redirection(t_token_type direction, t_list *tk_filename,
		t_ast *child, t_shell *sh)
{
	t_ast	*node;

	if (!tk_filename || tk_type(tk_filename) != TK_WORD)
		return (set_syntax_error(tk_type_to_string(direction), sh));
	init_ast_node(&node, AST_REDIRECTION, sh);
	node->u_data.redirection.direction = direction;
	node->u_data.redirection.file = tk_value(tk_filename);
	node->u_data.redirection.child = child;
	return (node);
}

t_ast	*create_ast_pipeline(t_ast *left, t_ast *right, t_shell *sh)
{
	t_ast	*node;

	if (!left || !right)
		return (set_syntax_error("|", sh));
	init_ast_node(&node, AST_PIPELINE, sh);
	node->u_data.pipeline.left = left;
	node->u_data.pipeline.right = right;
	return (node);
}

t_ast	*create_ast_logical(t_ast *left, t_token_type operator, t_ast *right,
	t_shell *sh)
{
	t_ast	*node;

	if (!left || !right)
		return (set_syntax_error(tk_type_to_string(operator), sh));
	init_ast_node(&node, AST_LOGICAL, sh);
	node->u_data.logical.operator = operator;
	node->u_data.logical.left = left;
	node->u_data.logical.right = right;
	return (node);
}
