/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_next_token.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:58:19 by fvastena          #+#    #+#             */
/*   Updated: 2024/02/02 18:01:51 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_type	scan_next_token(char *input, size_t *len, t_shell *sh)
{
	if (*input == '&' || *input == '|')
		return (scan_tk_logic_and_pipe(input, *input, len, sh));
	else if (*input == '<' || *input == '>')
		return (scan_tk_redirect(input, *input, len, sh));
	else if (*input == '(' || *input == ')')
		return (scan_tk_subshell(input, len));
	return (scan_tk_word(input, len, sh));
}

t_token_type	scan_tk_subshell(char *input, size_t *len)
{
	*len += 1;
	if (*input == '(')
		return (TK_SUBSHELL_OPEN);
	return (TK_SUBSHELL_CLOSE);
}

t_token_type	scan_tk_logic_and_pipe(char *input, char c, size_t *len,
		t_shell *sh)
{
	while (input[*len] == c)
		*len += 1;
	if (c == '&')
	{
		if (*len == 2)
			return (TK_AND);
		else if (*len == 1 || *len == 3)
			set_syntax_error("&", sh);
		else
			set_syntax_error("&&", sh);
	}
	else if (c == '|')
	{
		if (*len == 1)
			return (TK_PIPE);
		else if (*len == 2)
			return (TK_OR);
		else
			set_syntax_error("||", sh);
	}
	return (TK_ERROR);
}

t_token_type	scan_tk_redirect(char *input, char c, size_t *len, t_shell *sh)
{
	while (input[*len] == c)
		*len += 1;
	if (c == '<' && *len == 1)
		return (TK_REDIRECT_IN);
	else if (c == '<' && *len == 2)
		return (TK_HEREDOC);
	else if (c == '>' && *len == 1)
		return (TK_REDIRECT_OUT);
	else if (c == '>' && *len == 2)
		return (TK_APPEND_OUT);
	set_syntax_error(input, sh);
	return (TK_ERROR);
}

t_token_type	scan_tk_word(char *input, size_t *len, t_shell *sh)
{
	char	c;
	bool	in_quote;

	in_quote = false;
	while (input[*len])
	{
		if (input[*len + 1] && input[*len] == '\\')
			*len += 1;
		else if (input[*len] == '\"' || input[*len] == '\'')
		{
			if (!in_quote)
			{
				c = input[*len];
				in_quote = true;
			}
			else if (in_quote && input[*len] == c)
				in_quote = false;
		}
		else if ((is_token(input[*len]) || is_space(input[*len])) && !in_quote)
			break ;
		*len += 1;
	}
	if (in_quote)
		return (set_syntax_error("unclosed quote", sh), TK_ERROR);
	return (TK_WORD);
}
