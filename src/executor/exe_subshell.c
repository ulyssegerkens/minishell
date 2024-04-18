/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_subshell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:37:19 by ugerkens          #+#    #+#             */
/*   Updated: 2024/02/01 16:37:20 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_subshell(t_ast_subshell *subshell, t_shell *sh)
{
	pid_t	pid;
	int		status;

	pid = fork_s(sh);
	if (pid == 0)
	{
		sh->in_main_process = false;
		set_signal_child_process();
		execute(subshell->child, O_EXIT, sh);
	}
	wait_s(&status, sh);
	status = check_process_child_exit(status, NULL, sh);
	return (status);
}
