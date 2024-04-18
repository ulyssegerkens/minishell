/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secure_syscall_file.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvastena <fvastena@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:38:44 by ugerkens          #+#    #+#             */
/*   Updated: 2024/02/02 13:38:05 by fvastena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_s(const char *pathname, int flags, mode_t mode, t_shell *sh)
{
	int	ret;

	ret = open(pathname, flags, mode);
	if (ret == -1)
		syscall_error(pathname, errno, sh);
	return (ret);
}

int	close_s(int fd, t_shell *sh)
{
	int	ret;

	ret = close(fd);
	if (ret == -1)
		syscall_error("close", errno, sh);
	return (ret);
}

int	dup2_s(int oldfd, int newfd, t_shell *sh)
{
	int	ret;

	ret = dup2(oldfd, newfd);
	if (ret == -1)
		syscall_error("dup2", errno, sh);
	return (ret);
}

int	stat_s(const char *restrict path, struct stat *restrict buf, t_shell *sh)
{
	int	ret;

	ret = stat(path, buf);
	if (ret == -1)
		syscall_error("stat", errno, sh);
	return (ret);
}
