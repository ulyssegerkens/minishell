/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:38:38 by ugerkens          #+#    #+#             */
/*   Updated: 2024/03/27 12:46:21 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	@brief Print error message and quit shell
	@warning don't use any secure functions here as it could create infinite loop
*/
void	error(const char *context, char *description, int exit_status,
	t_shell *sh)
{
	ft_write_fd("minishell: ", STDERR_FILENO);
	ft_write_fd(description, STDERR_FILENO);
	ft_write_fd(": ", STDERR_FILENO);
	ft_write_fd(context, STDERR_FILENO);
	ft_write_fd("\n", STDERR_FILENO);
	quit_shell(exit_status, sh);
}

void	syscall_error(const char *context, int errnum, t_shell *sh)
{
	error(context, strerror(errnum), EXIT_FAILURE, sh);
}

int	report_errno(char *context, t_shell *sh)
{
	write_s("minishell: ", 2, sh);
	perror(context);
	return (EXIT_FAILURE);
}

int	report_error(char *context, char *element, char *description, t_shell *sh)
{
	write_s("minishell: ", STDERR_FILENO, sh);
	write_s(context, STDERR_FILENO, sh);
	if (element)
		write_s(element, STDERR_FILENO, sh);
	write_s(description, STDERR_FILENO, sh);
	write_s("\n", STDERR_FILENO, sh);
	return (EXIT_FAILURE);
}
