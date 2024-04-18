/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_bin_path_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:37:25 by ugerkens          #+#    #+#             */
/*   Updated: 2024/03/27 15:02:38 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	@brief 
	@details Take the PATH environment variable or default path
	and split it into an array of strings
	@details If PATH is modified the cache is cleared and
	this function called again at next execution
	@note Default paths is not describe by bash manual
	but it is a common behavior
*/
char	**build_paths_array(t_shell *sh)
{
	char	**paths_array;
	char	*paths;

	paths_array = NULL;
	paths = value(env_var("PATH", sh->env));
	if (!paths)
		paths = strdup_s(":", PROMPT, sh);
	paths_array = split_s(paths, ':', PROMPT, sh);
	if (contain_current_dir_symbol(paths))
		add_arg_to_array(&paths_array, strdup_s(".", PROMPT, sh), sh);
	return (paths_array);
}

/**
	@brief Add a path to the paths array
	@note Bash manual: A null directory name (= CURENT DIR) may appear 
	as two adjacent colons or as an initial or trailing colon. 
*/
bool	contain_current_dir_symbol(char *paths)
{
	if (!paths)
		return (false);
	if (paths[0] == ':' || paths[ft_strlen(paths) - 1] == ':')
		return (true);
	if (ft_strnstr(paths, "::", ft_strlen(paths)) != NULL)
		return (true);
	return (false);
}

/**
	@brief Join a directory path and a binary name
	@result full path ready to be used in execve
*/
char	*join_path(char *path_to, char *file, t_shell *sh)
{
	char	*slash_path;
	char	*full_path;

	slash_path = strjoin_s(path_to, "/", PROMPT, sh);
	full_path = strjoin_s(slash_path, file, PROMPT, sh);
	return (full_path);
}
