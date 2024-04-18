/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:37:02 by ugerkens          #+#    #+#             */
/*   Updated: 2024/03/27 14:49:59 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	@brief Prints the environment variables list in STDOUT
	@result "NAME=value\n" or "export NAME="value"\n"
	@param format options to print as env or export
*/
int	print_env(bool export_format, t_list *env, t_shell *sh)
{
	while (env)
	{
		if (!export_format && !value(env))
		{
			env = env->next;
			continue ;
		}
		if (export_format)
			write_s("declare -x ", STDOUT_FILENO, sh);
		write_s(name(env), STDOUT_FILENO, sh);
		if (value(env))
			write_s("=", STDOUT_FILENO, sh);
		if (export_format && value(env))
			write_s("\"", STDOUT_FILENO, sh);
		if (value(env))
			write_s(value(env), STDOUT_FILENO, sh);
		if (export_format && value(env))
			write_s("\"", STDOUT_FILENO, sh);
		write_s("\n", STDOUT_FILENO, sh);
		env = env->next;
	}
	return (EXIT_SUCCESS);
}

/**
	@brief Convert the environment variables list to a char **
	@result char ** array of the form {"NAME=value", NULL}
	@note this function is called for each command execution, it could be 
	optimized by storing the array and reallocating it only when needed
*/
char	**env_to_char_array(t_shell *sh)
{
	char	**env_array;
	char	*name_and_equal;
	int		i;
	t_list	*env;

	env = sh->env;
	env_array = calloc_s(ft_lstsize(env) + 1, sizeof(char *), PROMPT, sh);
	i = 0;
	while (env)
	{
		if (value(env))
		{
			name_and_equal = strjoin_s(name(env), "=", PROMPT, sh);
			env_array[i] = strjoin_s(name_and_equal, value(env), PROMPT, sh);
			i++;
		}
		env = env->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
