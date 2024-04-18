/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:36:31 by ugerkens          #+#    #+#             */
/*   Updated: 2024/03/27 14:26:25 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	@brief unset builtin without options
	@result remove env variable from env list
*/
int	unset_builtin(t_ast_command *cmd, t_shell *sh)
{
	t_list	*env_var_node;

	cmd->args++;
	while (*(cmd->args))
	{
		env_var_node = env_var(*(cmd->args), sh->env);
		remove_list_node(&env_var_node, &(sh->env), free_env_var, true);
		cmd->args++;
	}
	return (EXIT_SUCCESS);
}

void	free_env_var(void *content)
{
	t_var	*var;

	var = (t_var *)content;
	free(var->name);
	free(var->value);
	free(var);
}
