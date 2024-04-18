/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:37:08 by ugerkens          #+#    #+#             */
/*   Updated: 2024/02/01 16:37:09 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command(t_ast_command *cmd, t_execute_end end, t_shell *sh)
{
	int				status;
	t_builtin_func	builtin;

	status = EXIT_SUCCESS;
	if (cmd->args[0] == NULL)
		return (status);
	builtin = get_builtin_function(cmd->args[0]);
	if (builtin)
		return (builtin(cmd, sh));
	else
		status = fork_command(cmd, end, sh);
	return (status);
}

int	fork_command(t_ast_command *cmd, t_execute_end end, t_shell *sh)
{
	pid_t	pid;
	int		status;

	status = 0;
	if (end == O_EXIT)
		execve_s(get_bin_path(cmd->args[0], sh), cmd->args,
			env_to_char_array(sh), sh);
	else
	{
		pid = fork_s(sh);
		if (pid == 0)
		{
			sh->in_main_process = false;
			set_signal_child_process();
			execve_s(get_bin_path(cmd->args[0], sh), cmd->args,
				env_to_char_array(sh), sh);
		}
		wait_s(&status, sh);
		status = check_process_child_exit(status, NULL, sh);
	}
	return (status);
}
