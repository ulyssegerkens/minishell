/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:38:10 by ugerkens          #+#    #+#             */
/*   Updated: 2024/03/26 13:00:43 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	@brief	Syntax analysis (cf abstract_syntax_tree.md).
			Converts the tokens list into an abstract syntax tree.
	@return	The abstract syntax tree. NULL if there is a syntax error.
*/
int	parser(t_list *token_list, t_ast **ast, t_shell *sh)
{
	*ast = parse_logical(&token_list, sh);
	if (token_list)
		set_syntax_error(tk_value(token_list), sh);
	if (sh->parsing_error)
		return (report_syntax_error(sh));
	return (EXIT_SUCCESS);
}
