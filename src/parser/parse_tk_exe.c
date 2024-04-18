/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tk_exe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:38:02 by ugerkens          #+#    #+#             */
/*   Updated: 2024/03/26 13:01:13 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*parse_subshell(t_list **token, t_shell *sh)
{
	t_ast	*content;

	if (is_tk_type(token, 1, TK_SUBSHELL_OPEN))
	{
		*token = (*token)->next;
		content = parse_logical(token, sh);
		if (*token && tk_type(*token) == TK_SUBSHELL_CLOSE)
		{
			*token = (*token)->next;
			if ((*token) && is_tk_type(token, 1, TK_WORD))
				return (set_syntax_error(tk_value(*token), sh));
			return (create_ast_subshell(content, sh));
		}
		else if (*token == NULL)
			return (set_syntax_error("\\n", sh));
		else
			return (set_syntax_error(tk_value(*token), sh));
	}
	else if (is_tk_type(token, 1, TK_SUBSHELL_CLOSE))
		return (set_syntax_error(")", sh));
	return (parse_command(token, sh));
}

t_ast	*parse_command(t_list **token, t_shell *sh)
{
	t_list	*current;
	int		argc;
	char	**argv;
	int		i;

	current = *token;
	argc = 0;
	while (is_tk_type(&current, 1, TK_WORD))
	{
		argc++;
		current = current->next;
	}
	if (argc == 0)
		return (NULL);
	argv = calloc_s((argc + 1), sizeof(char *), PROMPT, sh);
	i = 0;
	while (i < argc)
	{
		argv[i++] = tk_value(*token);
		*token = (*token)->next;
	}
	argv[argc] = NULL;
	return (create_ast_command(argv, sh));
}
