/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_redirection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:37:17 by ugerkens          #+#    #+#             */
/*   Updated: 2024/02/01 16:37:18 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_redirection(t_ast_redirection *redir, t_shell *sh)
{
	int	status;

	status = EXIT_FAILURE;
	if (redir->direction == TK_REDIRECT_IN || redir->direction == TK_HEREDOC)
		status = redirect_input(redir, sh);
	else if (redir->direction == TK_REDIRECT_OUT)
		status = redirect_output(redir, sh);
	else if (redir->direction == TK_APPEND_OUT)
		status = append_output(redir, sh);
	return (status);
}

int	redirect_input(t_ast_redirection *redir, t_shell *sh)
{
	int	input_fd;
	int	original_stdin;
	int	status;

	input_fd = open(redir->file, O_RDONLY);
	if (input_fd == -1)
		return (report_errno(redir->file, sh));
	original_stdin = dup(STDIN_FILENO);
	dup2_s(input_fd, STDIN_FILENO, sh);
	close_s(input_fd, sh);
	status = execute(redir->child, O_RETURN, sh);
	dup2(original_stdin, STDIN_FILENO);
	close_s(original_stdin, sh);
	return (status);
}

int	redirect_output(t_ast_redirection *redir, t_shell *sh)
{
	int	output_fd;
	int	original_stdout;
	int	status;

	output_fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (output_fd == -1)
		return (report_errno(redir->file, sh));
	original_stdout = dup(STDOUT_FILENO);
	dup2_s(output_fd, STDOUT_FILENO, sh);
	close_s(output_fd, sh);
	status = execute(redir->child, O_RETURN, sh);
	dup2_s(original_stdout, STDOUT_FILENO, sh);
	close_s(original_stdout, sh);
	return (status);
}

int	append_output(t_ast_redirection *redir, t_shell *sh)
{
	int	output_fd;
	int	original_stdout;
	int	status;

	output_fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (output_fd == -1)
		return (report_errno(redir->file, sh));
	original_stdout = dup(STDOUT_FILENO);
	dup2_s(output_fd, STDOUT_FILENO, sh);
	close_s(output_fd, sh);
	status = execute(redir->child, O_RETURN, sh);
	dup2_s(original_stdout, STDOUT_FILENO, sh);
	close_s(original_stdout, sh);
	return (status);
}
