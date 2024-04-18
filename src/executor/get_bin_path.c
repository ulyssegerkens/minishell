/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_bin_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:37:29 by ugerkens          #+#    #+#             */
/*   Updated: 2024/03/27 15:12:17 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	@brief Get the path of a binary
	@param bin binary name as `ls` or `./bin/ls`
	@result path to the binary, an error is raised if not found
*/
char	*get_bin_path(char *bin, t_shell *sh)
{
	if (ft_strncmp(bin, "", 1) == 0)
		error("command not found", bin, 127, sh);
	else if (ft_strchr(bin, '/'))
		return (check_preset_bin_path(bin, sh));
	else if (ft_strncmp(bin, ".", 1) == 0 || ft_strncmp(bin, "..", 2) == 0)
		error("command not found", bin, 127, sh);
	else
		return (find_bin_path(bin, sh));
	return (NULL);
}

/**
	@brief Check if a binary path is valid
	@param bin_path direct path to a binary as `/bin/ls`, given by user
	@result path to the binary, an error is raised if not found
*/
char	*check_preset_bin_path(char *bin_path, t_shell *sh)
{
	struct stat	path_stat;

	if (access(bin_path, X_OK) == -1)
	{
		if (errno == EACCES)
			error("Permission denied", bin_path, 126, sh);
		else if (errno == ENOENT)
			error("No such file or directory", bin_path, 127, sh);
		else
			syscall_error(bin_path, errno, sh);
	}
	stat_s(bin_path, &path_stat, sh);
	if (S_ISDIR(path_stat.st_mode))
		error("Is a directory", bin_path, 126, sh);
	return (bin_path);
}

/**
	@brief Find a binary path in PATH environment variable
	@param bin binary name as `ls`
	@result path to the binary as `/bin/ls`, an error is raised if not found
*/
char	*find_bin_path(char *bin_name, t_shell *sh)
{
	char	*bin_path;
	bool	permission_denied;
	char	**paths_array;

	paths_array = build_paths_array(sh);
	if (!paths_array)
		error("command not found", bin_name, 127, sh);
	permission_denied = false;
	while (*paths_array)
	{
		bin_path = join_path(*paths_array, bin_name, sh);
		if (access(bin_path, X_OK) == EXIT_SUCCESS)
			return (bin_path);
		else if (errno == EACCES)
			permission_denied = true;
		else if (errno != ENOENT)
			syscall_error("access", errno, sh);
		paths_array++;
	}
	if (permission_denied)
		error("Permission denied", bin_name, 126, sh);
	else
		error("command not found", bin_name, 127, sh);
	return (NULL);
}
