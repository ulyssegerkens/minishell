/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:37:32 by fvastena          #+#    #+#             */
/*   Updated: 2024/02/06 01:10:50 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_getcwd(size_t size, char **new_pwd, t_shell *sh)
{
	char	*ret;
	char	*buf;

	buf = calloc_s(PATH_MAX, 1, PROMPT, sh);
	ret = getcwd(buf, size);
	if (!ret)
	{
		if (errno == ENOENT)
		{
			report_error("cd: error retrieving current directory: ",
				"getcwd: cannot access parent directories: ", strerror(errno),
				sh);
			edit_env_value(env_var("PWD", sh->env), "/.", true, sh);
			return (EXIT_FAILURE);
		}
		else
			syscall_error("getcwd: ", errno, sh);
	}
	*new_pwd = ret;
	return (EXIT_SUCCESS);
}

static int	set_new_pwd(char *new_old_pwd, t_shell *sh)
{
	char	*new_pwd;

	if (env_var("OLDPWD", sh->env))
		edit_env_value(env_var("OLDPWD", sh->env), new_old_pwd, false, sh);
	else
		add_new_env_var("OLDPWD", new_old_pwd, &(sh->env), sh);
	new_pwd = NULL;
	if (execute_getcwd(PATH_MAX, &new_pwd, sh))
		return (EXIT_FAILURE);
	if (new_pwd && env_var("PWD", sh->env))
		edit_env_value(env_var("PWD", sh->env), new_pwd, false, sh);
	return (EXIT_SUCCESS);
}

static int	execute_cd(t_ast_command *cmd, char *pwd,
		char *new_old_pwd, t_shell *sh)
{
	if (pwd[0] && chdir(pwd) == -1)
		return (report_error("cd: ", pwd, strjoin_s(": ", strerror(errno),
					PROMPT, sh), sh));
	if (set_new_pwd(new_old_pwd, sh))
		return (EXIT_FAILURE);
	if (cmd->args[1] && ft_strcmp(cmd->args[1], "-") == 0)
	{
		write_s(pwd, STDOUT_FILENO, sh);
		write_s("\n", STDOUT_FILENO, sh);
	}
	return (EXIT_SUCCESS);
}

/**
	@brief	Executes the cd builtin command.
*/
static bool	arg_to_new_pwd(t_ast_command *cmd, char **pwd, t_shell *sh)
{
	if (!cmd->args[1])
	{
		if (!env_var("HOME", sh->env))
			return (report_error("cd :", NULL, "HOME not set", sh));
		*pwd = value(env_var("HOME", sh->env));
	}
	else if (ft_strcmp(cmd->args[1], "-") == 0)
	{
		if (!env_var("OLDPWD", sh->env))
			return (report_error("cd :", NULL, "OLDPWD not set", sh));
		*pwd = strdup_s(value(env_var("OLDPWD", sh->env)), PROMPT, sh);
	}
	else
		*pwd = cmd->args[1];
	return (EXIT_SUCCESS);
}

int	cd_builtin(t_ast_command *cmd, t_shell *sh)
{
	char	*new_old_pwd;
	char	*pwd;
	int		status;

	status = EXIT_FAILURE;
	if (!cmd->args[0])
		return (EXIT_FAILURE);
	if (count_strs(cmd->args) > 2)
		return (report_error("cd: ", NULL, "too many arguments", sh));
	pwd = NULL;
	new_old_pwd = value(env_var("PWD", sh->env));
	status = arg_to_new_pwd(cmd, &pwd, sh);
	if (status != EXIT_SUCCESS)
		return (status);
	status = execute_cd(cmd, pwd, new_old_pwd, sh);
	return (status);
}
