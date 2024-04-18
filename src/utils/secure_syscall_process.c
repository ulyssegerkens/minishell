/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secure_syscall_process.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:38:45 by ugerkens          #+#    #+#             */
/*   Updated: 2024/02/01 16:38:46 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execve_s(const char *pathname, char *const argv[], char *const envp[],
		t_shell *sh)
{
	int	ret;

	ret = execve(pathname, argv, envp);
	if (ret == -1)
		syscall_error("execve", errno, sh);
	return (ret);
}

pid_t	fork_s(t_shell *sh)
{
	pid_t	ret;

	ret = fork();
	if (ret == -1)
		syscall_error("fork", errno, sh);
	return (ret);
}

int	pipe_s(int pipefd[2], t_shell *sh)
{
	int	ret;

	ret = pipe(pipefd);
	if (ret == -1)
		syscall_error("pipe", errno, sh);
	return (ret);
}

pid_t	wait_s(int *wstatus, t_shell *sh)
{
	pid_t	ret;

	ret = wait(wstatus);
	if (ret == -1)
		syscall_error("wait", errno, sh);
	return (ret);
}
