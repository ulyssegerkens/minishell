/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   access_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:37:00 by ugerkens          #+#    #+#             */
/*   Updated: 2024/02/01 16:37:01 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*env_var(char *name, t_list *env)
{
	t_var	*var;

	while (env)
	{
		var = (t_var *)(env->content);
		if (ft_strcmp(name, var->name) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

char	*name(t_list *env)
{
	if (!env)
		return (NULL);
	return (((t_var *)(env->content))->name);
}

char	*value(t_list *env)
{
	if (!env)
		return (NULL);
	return (((t_var *)(env->content))->value);
}
