/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secure_syscall_dir.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:38:44 by ugerkens          #+#    #+#             */
/*   Updated: 2024/03/27 14:09:18 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

DIR	*opendir_s(const char *name, t_shell *sh)
{
	DIR	*ret;

	errno = 0;
	ret = opendir(name);
	if (!ret && errno != 0)
		syscall_error("opendir", errno, sh);
	return (ret);
}

struct dirent	*readdir_s(DIR *dirp, t_shell *sh)
{
	struct dirent	*ret;

	errno = 0;
	ret = readdir(dirp);
	if (!ret && errno != 0)
		syscall_error("readdir", errno, sh);
	return (ret);
}

int	closedir_s(DIR *dirp, t_shell *sh)
{
	int	ret;

	errno = 0;
	ret = closedir(dirp);
	if (ret == -1 && errno != 0)
		syscall_error("closedir", errno, sh);
	return (ret);
}
