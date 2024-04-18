/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvastena <fvastena@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:58:14 by fvastena          #+#    #+#             */
/*   Updated: 2024/02/01 16:58:15 by fvastena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	@brief Lexical analysis
	@param input input string from the user
	@result a list of tokens (type + content)
	@return EXIT_SUCCESS or EXIT_FAILURE
 **/
int	lexer(char *input, t_list **token_list, t_shell *sh)
{
	t_token			*token;
	t_token_type	type;
	size_t			i;
	size_t			len;

	i = 0;
	*token_list = NULL;
	while (input[i])
	{
		len = 0;
		if (!is_space(input[i]))
		{
			type = scan_next_token(input + i, &len, sh);
			if (type == TK_ERROR)
				return (report_syntax_error(sh));
			token = create_token(type, input + i, len, sh);
			lst_add_back_s(token, token_list, PROMPT, sh);
		}
		else
			len++;
		i += len;
	}
	return (EXIT_SUCCESS);
}

t_token	*create_token(t_token_type type, char *value, size_t len, t_shell *sh)
{
	t_token	*token;

	token = calloc_s(1, sizeof(t_token), PROMPT, sh);
	token->type = type;
	token->value = s_alloc(ft_substr(value, 0, len), PROMPT, sh);
	return (token);
}
