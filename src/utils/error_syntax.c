/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:38:36 by ugerkens          #+#    #+#             */
/*   Updated: 2024/02/01 16:38:37 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	@brief	Set the syntax error message only if there is no previous
		error as only the first error will be reported.
*/
void	*set_syntax_error(char *unexpected_token, t_shell *sh)
{
	if (!sh->parsing_error)
		sh->parsing_error = unexpected_token;
	return (NULL);
}

int	report_syntax_error(t_shell *sh)
{
	report_error("syntax error near unexpected token `",
		sh->parsing_error, "'", sh);
	sh->parsing_error = NULL;
	return (2);
}
