/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:38:13 by ugerkens          #+#    #+#             */
/*   Updated: 2024/02/01 16:38:14 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_type	tk_type(t_list *token)
{
	return (((t_token *)token->content)->type);
}

char	*tk_value(t_list *token)
{
	if (token == NULL)
		return (NULL);
	return (((t_token *)token->content)->value);
}

bool	is_tk_type(t_list **token_list, int num_args, ...)
{
	va_list			args;
	t_token_type	current_type;

	if (token_list == NULL || *token_list == NULL)
		return (false);
	current_type = tk_type(*token_list);
	va_start(args, num_args);
	while (num_args--)
	{
		if (current_type == va_arg(args, t_token_type))
		{
			va_end(args);
			return (true);
		}
	}
	va_end(args);
	return (false);
}

char	*tk_type_to_string(t_token_type type)
{
	if (type == TK_PIPE)
		return ("|");
	else if (type == TK_AND)
		return ("&&");
	else if (type == TK_OR)
		return ("||");
	else if (type == TK_SUBSHELL_OPEN)
		return ("(");
	else if (type == TK_SUBSHELL_CLOSE)
		return (")");
	else if (type == TK_WORD)
		return ("word");
	else if (type == TK_REDIRECT_IN)
		return ("<");
	else if (type == TK_REDIRECT_OUT)
		return (">");
	else if (type == TK_APPEND_OUT)
		return (">>");
	else if (type == TK_HEREDOC)
		return ("<<");
	else
		return ("[unknown token]");
}
