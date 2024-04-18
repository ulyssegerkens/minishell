/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_general.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:38:52 by ugerkens          #+#    #+#             */
/*   Updated: 2024/03/27 15:12:54 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	@file src/core/utils.c
	@brief General utils functions used across the project
*/

/**
	@brief Add a string to the end of an array of strings
	@warning to use only for PROMPT operations (allocation scope)
	@warning this function dont free the old array
*/
void	add_arg_to_array(char ***array, char *new_arg, t_shell *sh)
{
	char	**new_array;
	int		i;

	new_array = calloc_s(count_strs(*array) + 2, sizeof(char *), PROMPT, sh);
	i = 0;
	while (array && (*array)[i])
	{
		new_array[i] = (*array)[i];
		i++;
	}
	new_array[i] = new_arg;
	new_array[i + 1] = NULL;
	*array = new_array;
}

int	count_strs(char **argv)
{
	int	i;

	i = 0;
	while (argv && argv[i])
		i++;
	return (i);
}
