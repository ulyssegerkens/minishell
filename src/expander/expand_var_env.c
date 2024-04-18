/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:37:46 by fvastena          #+#    #+#             */
/*   Updated: 2024/03/27 15:02:02 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	@brief Expand environment variable into its value
*/
char	*expand_env_var(char *str, t_expander *exp, t_shell *sh)
{
	char	*name;
	char	*env_value;

	name = get_valid_name(str, exp, sh);
	if (name == NULL)
	{
		exp->buf[exp->buf_i++] = str[exp->i];
		exp->buf[exp->buf_i] = '\0';
		return (NULL);
	}
	env_value = value(env_var(name, sh->env));
	exp->i += ft_strlen(name);
	return (env_value);
}

char	*get_valid_name(char *str, t_expander *exp, t_shell *sh)
{
	int		j;
	char	*name;

	j = exp->i + 1;
	if (!ft_isalpha(str[j]) && str[j] != '_')
		return (NULL);
	while (ft_isalnum(str[j]) || str[j] == '_')
		j++;
	name = s_alloc(ft_substr(str, exp->i + 1, j - exp->i - 1), PROMPT, sh);
	return (name);
}
