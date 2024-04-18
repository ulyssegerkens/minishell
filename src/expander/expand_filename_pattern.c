/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_filename_pattern.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 14:22:27 by ugerkens          #+#    #+#             */
/*   Updated: 2024/03/27 14:22:31 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	@brief Filter a list of files on a pattern as `echo M*`
*/
t_list	*pattern_filter(t_list *files, t_expander *exp)
{
	t_list	*current;
	char	*full_filename;
	t_list	*next;

	current = files;
	while (current)
	{
		full_filename = (char *)current->content;
		if (!pattern_match(full_filename, exp->buf, 0, exp)
			|| not_explicit_hidden_file(full_filename, exp))
		{
			next = current->next;
			remove_list_node(&current, &files, NULL, false);
			current = next;
		}
		else
			current = current->next;
	}
	return (files);
}

/**
	@brief Check if a filename matches a pattern
	@note This function is recursive
*/
bool	pattern_match(const char *filename, const char *pattern,
		int pattern_index, t_expander *exp)
{
	while (*filename && *pattern)
	{
		if (*pattern == '*' && is_active_wildcard(pattern_index, exp))
		{
			pattern++;
			pattern_index++;
			if (!*pattern)
				return (true);
			while (*filename)
			{
				if (pattern_match(filename, pattern, pattern_index, exp))
					return (true);
				filename++;
			}
			return (false);
		}
		else if (*pattern != *filename)
			return (false);
		pattern++;
		filename++;
		pattern_index++;
	}
	if (only_active_wildcard_left(pattern, exp))
		return (true);
	return (*pattern == *filename);
}

/**
	@brief This function is used to filter hidden files if they are not
	explicitly stated in the pattern.
*/
bool	not_explicit_hidden_file(const char *full_filename, t_expander *exp)
{
	const char	*filename;
	const char	*last_part_pattern;

	filename = ft_strrchr(full_filename, '/');
	if (!filename)
		filename = full_filename;
	else
		filename++;
	last_part_pattern = ft_strrchr(exp->buf, '/');
	if (!last_part_pattern)
		last_part_pattern = exp->buf;
	else
		last_part_pattern++;
	if (filename[0] == '.' && last_part_pattern[0] != '.')
		return (true);
	return (false);
}
