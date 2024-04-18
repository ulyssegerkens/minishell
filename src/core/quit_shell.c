/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:36:56 by ugerkens          #+#    #+#             */
/*   Updated: 2024/02/01 16:36:57 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	@brief Free allocation and exit the current process
	@warning don't use any secure functions here as
		it could create infinite loop in case of error
*/
void	quit_shell(int exit_status, t_shell *sh)
{
	if (sh)
	{
		if (sh->in_main_process && isatty(STDIN_FILENO))
			ft_write_fd("exit\n", STDERR_FILENO);
		clear_prompt(sh);
		ft_lstclear(&sh->env, free_env_var);
		ft_lstclear(&sh->allocated_pointers[SH], free);
	}
	rl_clear_history();
	exit(exit_status);
}
