/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:36:29 by ugerkens          #+#    #+#             */
/*   Updated: 2024/03/27 15:09:31 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	@brief export builtin without options
	@result modify env list
		or print env list + export but unset variables
*/
int	export_builtin(t_ast_command *cmd, t_shell *sh)
{
	bool	name_error;

	cmd->args++;
	if (!*(cmd->args))
		return (print_env(true, sh->env, sh));
	name_error = false;
	while (*(cmd->args))
	{
		export_one(*(cmd->args), &name_error, sh);
		cmd->args++;
	}
	if (name_error)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/**
	@brief Export one variable
	@result modify env variable
		or add env variable to the end of env list
	@param assignment string of the form "name=value"
	@param name_error to set true if there is a name error
*/
void	export_one(char *assignment, bool *name_error, t_shell *sh)
{
	char	*equal_sign;
	bool	add;
	char	*name;
	t_list	*in_env;

	equal_sign = ft_strchr(assignment, '=');
	add = false;
	if (ft_strnstr(assignment, "+=", equal_sign - assignment + 1))
		add = true;
	name = s_alloc(ft_substr(assignment, 0, equal_sign - add - assignment),
			PROMPT, sh);
	if (!is_valid_name(name))
		return (report_name_error(assignment, name_error, sh));
	in_env = env_var(name, sh->env);
	if (in_env && equal_sign)
		edit_env_value(in_env, equal_sign + 1, add, sh);
	else if (!in_env && !equal_sign)
		add_new_env_var(name, NULL, &(sh->env), sh);
	else if (!in_env && equal_sign)
		add_new_env_var(name, equal_sign + 1, &(sh->env), sh);
}

void	report_name_error(char *assignment, bool *name_error, t_shell *sh)
{
	report_error("export: `", assignment, "': not a valid identifier", sh);
	*name_error = true;
}

bool	is_valid_name(char *name)
{
	if (!ft_isalpha(*name) && *name != '_')
		return (false);
	name++;
	while (*name)
	{
		if (!ft_isalnum(*name) && *name != '_')
			return (false);
		name++;
	}
	return (true);
}
