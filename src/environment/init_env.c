/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:37:04 by ugerkens          #+#    #+#             */
/*   Updated: 2024/03/27 15:16:21 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*init_env(char **env, t_shell *sh)
{
	char	*equal_sign;
	char	*name;
	t_list	*env_list;
	char	*saved_home;

	env_list = NULL;
	while (*env)
	{
		equal_sign = ft_strchr(*env, '=');
		name = s_alloc(ft_substr(*env, 0, equal_sign - *env), PROMPT, sh);
		add_new_env_var(name, equal_sign + 1, &(env_list), sh);
		env++;
	}
	if (!env_var("PATH", env_list))
		add_new_env_var("PATH", DEFAULT_PATH, &(env_list), sh);
	saved_home = value(env_var("HOME", env_list));
	if (saved_home)
		sh->home = strdup_s(saved_home, SH, sh);
	else
		sh->home = NULL;
	return (env_list);
}

/**
	@brief Add new env variable to the end of env list
	@warning name and value are duplicated
*/
void	add_new_env_var(char *name, char *value, t_list **list, t_shell *sh)
{
	t_var	*var;

	var = calloc_s(1, sizeof(t_var), ONLY_CHECK, sh);
	var->name = strdup_s(name, ONLY_CHECK, sh);
	var->value = NULL;
	if (value)
		var->value = strdup_s(value, ONLY_CHECK, sh);
	lst_add_back_s(var, list, ONLY_CHECK, sh);
}

void	edit_env_value(t_list *in_env, char *new_value, bool add, t_shell *sh)
{
	t_var	*var;
	char	*tmp;

	var = (t_var *)(in_env->content);
	tmp = var->value;
	if (!new_value)
		var->value = NULL;
	else if (add)
		var->value = strjoin_s(var->value, new_value, ONLY_CHECK, sh);
	else
		var->value = strdup_s(new_value, ONLY_CHECK, sh);
	free(tmp);
}
