/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:36:19 by ugerkens          #+#    #+#             */
/*   Updated: 2024/02/05 19:56:47 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	@brief echo builtin with -n option
	@result print args to stdout
*/
int	echo_builtin(t_ast_command *cmd, t_shell *sh)
{
	bool	new_line;
	int		i;

	i = 1;
	new_line = !(option_check(cmd->args, &i, 'n'));
	while (cmd->args[i])
	{
		write_s(cmd->args[i], STDOUT_FILENO, sh);
		if (cmd->args[i + 1])
			write_s(" ", STDOUT_FILENO, sh);
		i++;
	}
	if (new_line)
		write_s("\n", STDOUT_FILENO, sh);
	return (EXIT_SUCCESS);
}

bool	option_check(char **args, int *i, char c)
{
	int		j;
	bool	detected;

	detected = false;
	while (args[*i] && args[*i][0] == '-')
	{
		j = 1;
		while (args[*i][j] == c)
			j++;
		if (args[*i][j - 1] == c && args[*i][j] == '\0')
			detected = true;
		else
			return (detected);
		(*i)++;
	}
	return (detected);
}
