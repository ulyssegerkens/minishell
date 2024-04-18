/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tk_redirection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:38:08 by ugerkens          #+#    #+#             */
/*   Updated: 2024/02/01 16:38:09 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	@brief Parse all redirections associated to a command
	@param token as `> file1 ls-option < file2 << end`
	@result t_ast_command as `{> file1} -> {< file2} -> {<< end} -> {ls-option}`
	@details
		1. Parse prefix redirections
		2. Parse the command
		3. Parse suffix redirections
		4. Merge prefix, command and suffix
*/
t_ast	*parse_redirection(t_list **token, t_shell *sh)
{
	t_ast	*prefix;
	t_ast	*suffix;
	t_ast	*command;

	prefix = parse_redirection_list(token, NULL, sh);
	if (sh->parsing_error)
		return (NULL);
	command = parse_subshell(token, sh);
	suffix = parse_redirection_list(token, command, sh);
	if (sh->parsing_error)
		return (NULL);
	return (build_redirected_command(prefix, suffix, command));
}

bool	parse_new_command_arg(t_list **token, t_ast *command, t_shell *sh)
{
	if (is_tk_type(token, 1, TK_WORD) && command)
	{
		add_arg_to_array(&command->u_data.command.args, tk_value(*token), sh);
		*token = (*token)->next;
		return (true);
	}
	return (false);
}

bool	is_in_sequential_redirection_list(t_list **token, t_ast *command)
{
	if (is_tk_type(token, 4, TK_REDIRECT_IN, TK_REDIRECT_OUT, TK_APPEND_OUT,
			TK_HEREDOC))
		return (true);
	if (is_tk_type(token, 1, TK_WORD) && command)
		return (true);
	return (false);
}

/**
	@brief Parse an uninterrupted sequence of redirections
*/
t_ast	*parse_redirection_list(t_list **token, t_ast *command, t_shell *sh)
{
	t_ast	*first;
	t_ast	*last;
	t_ast	*new;

	first = NULL;
	while (is_in_sequential_redirection_list(token, command))
	{
		if (parse_new_command_arg(token, command, sh))
			continue ;
		new = create_ast_redirection(tk_type(*token), (*token)->next, NULL, sh);
		if (!new)
			return (NULL);
		if (!first)
		{
			first = new;
			last = new;
		}
		else
		{
			last->u_data.redirection.child = new;
			last = new;
		}
		*token = (*token)->next->next;
	}
	return (first);
}
