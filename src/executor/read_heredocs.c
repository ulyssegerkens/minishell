/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredocs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:37:34 by ugerkens          #+#    #+#             */
/*   Updated: 2024/03/27 14:29:43 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	@brief Read heredoc all heredocs
	@note as it has to be done before any other execution,
		it parse the whole tree to find heredocs and execute them
*/
int	execute_heredocs(t_ast *node, t_shell *sh)
{
	char	*tmp_file_name;
	char	*file_id;
	int		tmp_file;
	int		status;

	if (!is_heredoc(node))
		return (search_for_heredocs(node, sh));
	file_id = s_alloc(ft_itoa(ft_lstsize(sh->temporary_files)), PROMPT, sh);
	tmp_file_name = strjoin_s("/tmp/minishell_heredoc_", file_id, PROMPT, sh);
	lst_add_front_s(tmp_file_name, &sh->temporary_files, PROMPT, sh);
	tmp_file = open_s(tmp_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0600, sh);
	status = heredoc_listener(tmp_file,
			trim_delimiter(node->u_data.redirection.file, sh), sh);
	close_s(tmp_file, sh);
	node->u_data.redirection.file = tmp_file_name;
	if (node->u_data.redirection.child)
		return (search_for_heredocs(node->u_data.redirection.child, sh));
	return (status);
}

bool	is_heredoc(t_ast *node)
{
	return (node && node->type == AST_REDIRECTION
		&& node->u_data.redirection.direction == TK_HEREDOC);
}

int	search_for_heredocs(t_ast *node, t_shell *sh)
{
	if (!node)
		return (EXIT_SUCCESS);
	else if (is_heredoc(node))
		return (execute_heredocs(node, sh));
	else if (node->type == AST_REDIRECTION)
		return (search_for_heredocs(node->u_data.redirection.child, sh));
	else if (node->type == AST_LOGICAL)
		return (search_for_heredocs(node->u_data.logical.left, sh)
			|| search_for_heredocs(node->u_data.logical.right, sh));
	else if (node->type == AST_PIPELINE)
		return (search_for_heredocs(node->u_data.pipeline.left, sh)
			|| search_for_heredocs(node->u_data.pipeline.right, sh));
	else if (node->type == AST_SUBSHELL)
		return (search_for_heredocs(node->u_data.subshell.child, sh));
	return (EXIT_SUCCESS);
}

char	*trim_delimiter(char *delimiter, t_shell *sh)
{
	char	first_char;

	first_char = *delimiter;
	if (first_char == '\'' || first_char == '\"')
		delimiter = s_alloc(ft_strtrim(delimiter, &first_char), PROMPT, sh);
	return (delimiter);
}

int	heredoc_listener(int tmp_file, char *delimiter, t_shell *sh)
{
	char	*user_input;

	user_input = NULL;
	while (true)
	{
		user_input = prompt_listener(HEREDOC_PROMPT);
		if (g_signal_value == SIGINT)
			return (130);
		if (!user_input)
		{
			report_error("warning: here-document delimited by end-of-file \
					(wanted `", delimiter, "')", sh);
			return (EXIT_SUCCESS);
		}
		if (ft_strcmp(user_input, delimiter) == 0)
		{
			free(user_input);
			return (EXIT_SUCCESS);
		}
		write_s(user_input, tmp_file, sh);
		write_s("\n", tmp_file, sh);
		free(user_input);
	}
	return (EXIT_SUCCESS);
}
