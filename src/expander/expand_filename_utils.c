/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_filename_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 14:22:35 by ugerkens          #+#    #+#             */
/*   Updated: 2024/03/27 14:22:39 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	@brief This function is call before the filename_expansion to save the
	position of the wildcards to expend that have to be differentiated
	from literal wildcards (in quotes as `echo "*"`).
*/
void	saved_wildcards_position(char *to_check, t_expander *exp, t_shell *sh)
{
	int	i;
	int	*position;

	i = 0;
	if (exp->context != NO_QUOTE)
		return ;
	while (to_check[i])
	{
		if (to_check[i] == '*')
		{
			position = calloc_s(1, sizeof(int), PROMPT, sh);
			*((int *)position) = exp->buf_i + i;
			lst_add_back_s(position, &exp->wildcards_position, PROMPT, sh);
		}
		i++;
	}
}

/**
	@brief Little sister of saved_wildcards_position, this function is used
	to check if the current character is a wildcard that has to be expanded.
*/
bool	is_active_wildcard(int i, t_expander *exp)
{
	t_list	*current;

	current = exp->wildcards_position;
	while (current)
	{
		if (*((int *)(current->content)) == i)
			return (true);
		current = current->next;
	}
	return (false);
}

/**
	@brief Check if the end of a pattern is only composed of active wildcards.
*/
bool	only_active_wildcard_left(const char *str, t_expander *exp)
{
	if (!str || !*str)
		return (false);
	while (*str)
	{
		if (*str != '*' || !is_active_wildcard(exp->buf_i, exp))
			return (false);
		str++;
	}
	return (true);
}

/**
	@brief A pattern could use path, so we need to extract
	the root path to get the list of files in it.
*/
char	*extract_root_path(t_expander *exp, t_shell *sh)
{
	char	*path;
	int		i;

	i = exp->buf_i - 1;
	while (i >= 0 && exp->buf[i] != '/')
		i--;
	if (i == -1)
		return (strdup_s(".", PROMPT, sh));
	path = calloc_s(i + 2, sizeof(char), PROMPT, sh);
	ft_strlcpy(path, exp->buf, i + 2);
	return (path);
}

void	list_of_file_to_token_list(t_list *lst, t_expander *exp, t_shell *sh)
{
	while (lst)
	{
		lst_add_back_s(lst->content, exp->tokens, PROMPT, sh);
		lst = lst->next;
	}
}
