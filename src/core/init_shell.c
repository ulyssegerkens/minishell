/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:36:34 by ugerkens          #+#    #+#             */
/*   Updated: 2024/03/27 14:49:02 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_shell(t_shell *sh, char **env)
{
	errno = 0;
	sh->in_main_process = true;
	sh->last_status = EXIT_SUCCESS;
	sh->allocated_pointers[ONLY_CHECK] = NULL;
	sh->allocated_pointers[SH] = NULL;
	sh->allocated_pointers[PROMPT] = NULL;
	sh->temporary_files = NULL;
	sh->env = init_env(env, sh);
	increment_shlvl(sh);
	sh->parsing_error = NULL;
}

void	increment_shlvl(t_shell *sh)
{
	t_list	*in_env;
	int		new_value;
	char	*new_value_str;

	in_env = env_var("SHLVL", sh->env);
	if (!in_env)
		return (add_new_env_var("SHLVL", "1", &(sh->env), sh));
	new_value = ft_atoi(value(in_env)) + 1;
	new_value_str = s_alloc(ft_itoa(new_value), PROMPT, sh);
	if (new_value < 0)
		edit_env_value(in_env, "0", false, sh);
	else if (new_value > 1000)
	{
		report_error("warning: shell level (", new_value_str,
			") too high, resetting to 1", sh);
		edit_env_value(in_env, "1", false, sh);
	}
	else
		edit_env_value(in_env, new_value_str, false, sh);
}
