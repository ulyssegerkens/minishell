/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tk_redirection_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:38:06 by ugerkens          #+#    #+#             */
/*   Updated: 2024/02/01 16:38:07 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	@brief Merge prefix, command and suffix after parsing redirections
*/
t_ast	*build_redirected_command(t_ast *prefix, t_ast *suffix, t_ast *command)
{
	if (prefix && suffix)
	{
		get_last_redirection_list(prefix)->u_data.redirection.child = suffix;
		get_last_redirection_list(suffix)->u_data.redirection.child = command;
		return (prefix);
	}
	else if (prefix && !suffix)
	{
		get_last_redirection_list(prefix)->u_data.redirection.child = command;
		return (prefix);
	}
	else if (!prefix && suffix)
	{
		get_last_redirection_list(suffix)->u_data.redirection.child = command;
		return (suffix);
	}
	else
		return (command);
}

t_ast	*get_last_redirection_list(t_ast *node)
{
	while (node && node->type == AST_REDIRECTION
		&& node->u_data.redirection.child
		&& node->u_data.redirection.child->type == AST_REDIRECTION)
		node = node->u_data.redirection.child;
	return (node);
}
