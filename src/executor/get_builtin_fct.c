/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_builtin_fct.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:37:32 by ugerkens          #+#    #+#             */
/*   Updated: 2024/02/01 16:37:33 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_builtin_func	get_builtin_function(char *cmd_name)
{
	int				i;
	const t_builtin	builtins[] = {{"echo", echo_builtin}, {"cd", cd_builtin},
	{"pwd", pwd_builtin}, {"export", export_builtin}, {"unset",
		unset_builtin}, {"env", env_builtin}, {"exit", exit_builtin}, {NULL,
		NULL}};

	i = 0;
	while (builtins[i].name)
	{
		if (ft_strcmp(cmd_name, builtins[i].name) == 0)
			return (builtins[i].func);
		i++;
	}
	return (NULL);
}
