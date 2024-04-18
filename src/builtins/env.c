/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:36:22 by ugerkens          #+#    #+#             */
/*   Updated: 2024/02/06 00:51:41 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	@brief env builtin without options and arguments
	@result prints the environment variables list in STDOUT
*/
int	env_builtin(t_ast_command *cmd, t_shell *sh)
{
	if (count_strs(cmd->args) > 1)
	{
		report_error("env: ", NULL, "too many arguments", sh);
		return (EXIT_FAILURE);
	}
	print_env(false, sh->env, sh);
	return (EXIT_SUCCESS);
}
