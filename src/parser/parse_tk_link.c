/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tk_link.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:38:04 by ugerkens          #+#    #+#             */
/*   Updated: 2024/02/01 16:38:05 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*parse_logical(t_list **token, t_shell *sh)
{
	t_ast			*left;
	t_token_type	logical_operator;
	t_ast			*right;

	left = parse_pipeline(token, sh);
	while (is_tk_type(token, 2, TK_AND, TK_OR))
	{
		logical_operator = tk_type(*token);
		*token = (*token)->next;
		right = parse_pipeline(token, sh);
		left = create_ast_logical(left, logical_operator, right, sh);
	}
	return (left);
}

t_ast	*parse_pipeline(t_list **token, t_shell *sh)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_redirection(token, sh);
	while (*token && tk_type(*token) == TK_PIPE)
	{
		*token = (*token)->next;
		right = parse_redirection(token, sh);
		left = create_ast_pipeline(left, right, sh);
	}
	return (left);
}
