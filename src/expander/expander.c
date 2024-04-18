/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvastena <fvastena@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:37:54 by fvastena          #+#    #+#             */
/*   Updated: 2024/02/01 16:37:54 by fvastena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	@brief Expand command and redirection node (WORD tokens) in the AST
*/
t_ast	*expand_node(t_ast *node, t_shell *sh)
{
	if (node->type == AST_COMMAND)
		expand_command(node, sh);
	else if (node->type == AST_REDIRECTION)
		expand_redirection(node, sh);
	return (node);
}

/**
	@brief Expand every arguments of a command node
*/
void	expand_command(t_ast *node, t_shell *sh)
{
	char	**args;
	t_list	*expanded_args;

	expanded_args = NULL;
	args = node->u_data.command.args;
	while (*args)
	{
		expand_one_arg(*args, &expanded_args, sh);
		args++;
	}
	node->u_data.command.args = convert_list_to_array(&expanded_args, sh);
}

/**
	@brief Expand every arguments of a redirection node
	If it result in more than one argument, raise an error
	@note As we consider the Heredoc expansion out of the scope 
		of the project, it is ignored
*/
void	expand_redirection(t_ast *node, t_shell *sh)
{
	char	*str;
	t_list	*expanded_args;

	if (node->u_data.redirection.direction == TK_HEREDOC)
		return ;
	expanded_args = NULL;
	str = node->u_data.redirection.file;
	expand_one_arg(str, &expanded_args, sh);
	if (ft_lstsize(expanded_args) != 1 || !ft_strcmp(expanded_args->content,
			""))
		report_error("redirection:", NULL, " ambiguous redirect", sh);
	else
		node->u_data.redirection.file = expanded_args->content;
}
