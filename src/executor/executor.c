/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:37:22 by ugerkens          #+#    #+#             */
/*   Updated: 2024/03/27 15:25:19 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute(t_ast *node, t_execute_end end, t_shell *sh)
{
	int	status;

	if (!node)
		return (EXIT_SUCCESS);
	if (node->type == AST_REDIRECTION || node->type == AST_COMMAND)
		expand_node(node, sh);
	if (node->type == AST_LOGICAL)
		status = execute_logical(&node->u_data.logical, sh);
	else if (node->type == AST_PIPELINE)
		status = execute_pipeline(node, sh);
	else if (node->type == AST_REDIRECTION)
		status = execute_redirection(&node->u_data.redirection, sh);
	else if (node->type == AST_SUBSHELL)
		status = execute_subshell(&node->u_data.subshell, sh);
	else if (node->type == AST_COMMAND)
		status = execute_command(&node->u_data.command, end, sh);
	else
		error("execute", "illegal node type", EXIT_FAILURE, sh);
	if (end == O_EXIT)
		exit(status);
	return (status);
}

int	check_process_child_exit(int status, bool *new_line, t_shell *sh)
{
	int	signal;

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		signal = WTERMSIG(status);
		if (signal == SIGQUIT)
			write_s("Quit: 3", STDERR_FILENO, sh);
		if (signal == SIGQUIT || signal == SIGINT)
		{
			if (!new_line || (new_line && *new_line == false))
				write_s("\n", STDERR_FILENO, sh);
			if (new_line && *new_line == false)
				*new_line = true;
		}
		return (128 + signal);
	}
	else
		return (EXIT_FAILURE);
}
