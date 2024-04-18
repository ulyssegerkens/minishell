/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_logical.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:37:11 by ugerkens          #+#    #+#             */
/*   Updated: 2024/02/01 16:37:12 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_logical(t_ast_logical *logical, t_shell *sh)
{
	int	left_status;
	int	right_status;

	left_status = execute(logical->left, O_RETURN, sh);
	sh->last_status = left_status;
	if (logical->operator == TK_AND)
	{
		if (left_status == EXIT_SUCCESS)
		{
			right_status = execute(logical->right, O_RETURN, sh);
			return (right_status);
		}
		return (left_status);
	}
	else if (logical->operator == TK_OR)
	{
		if (left_status != EXIT_SUCCESS)
		{
			right_status = execute(logical->right, O_RETURN, sh);
			return (right_status);
		}
		return (left_status);
	}
	return (EXIT_FAILURE);
}
