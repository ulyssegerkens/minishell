/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_pipeline_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:37:12 by ugerkens          #+#    #+#             */
/*   Updated: 2024/02/01 16:37:13 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_for_next_command(int *prev_read_end, int p[2], t_shell *sh)
{
	if (*prev_read_end != -1)
		close_s(*prev_read_end, sh);
	close_s(p[WRITE_END], sh);
	*prev_read_end = p[READ_END];
}

int	wait_for_children(pid_t last_pid, int n_pipeline, t_shell *sh)
{
	pid_t	child_pid;
	int		status;
	int		last_cmd_status;
	bool	new_line;

	new_line = false;
	last_cmd_status = 0;
	while (n_pipeline--)
	{
		child_pid = wait_s(&status, sh);
		if (child_pid == last_pid)
			last_cmd_status = check_process_child_exit(status, &new_line, sh);
		else
			check_process_child_exit(status, &new_line, sh);
	}
	return (last_cmd_status);
}
