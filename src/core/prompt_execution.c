/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_execution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:36:42 by ugerkens          #+#    #+#             */
/*   Updated: 2024/02/06 10:29:48 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	prompt_execution(char *user_input, t_shell *sh)
{
	t_list	*token_list;
	t_ast	*ast;
	int		status;

	status = lexer(user_input, &token_list, sh);
	if (status == EXIT_SUCCESS && token_list)
	{
		status = parser(token_list, &ast, sh);
		if (status == EXIT_SUCCESS && ast)
		{
			status = execute_heredocs(ast, sh);
			if (status == EXIT_SUCCESS)
				status = execute(ast, O_RETURN, sh);
		}
	}
	return (status);
}
