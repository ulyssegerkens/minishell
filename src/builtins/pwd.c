/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvastena <fvastena@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:59:42 by fvastena          #+#    #+#             */
/*   Updated: 2024/02/01 16:59:43 by fvastena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd_builtin(t_ast_command *cmd, t_shell *sh)
{
	char	*current_directory;
	char	*getcwd_ret;

	(void)cmd;
	current_directory = value(env_var("PWD", sh->env));
	if (!current_directory)
	{
		getcwd_ret = getcwd(NULL, 0);
		if (!getcwd_ret)
			return (syscall_error("getcwd", errno, sh), 1);
		s_alloc(getcwd_ret, PROMPT, sh);
		write_s(getcwd_ret, STDOUT_FILENO, sh);
		write_s("\n", STDOUT_FILENO, sh);
		return (0);
	}
	write_s(current_directory, STDOUT_FILENO, sh);
	write_s("\n", STDOUT_FILENO, sh);
	return (0);
}
