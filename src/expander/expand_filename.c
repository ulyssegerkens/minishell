/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_filename.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 14:07:38 by ugerkens          #+#    #+#             */
/*   Updated: 2024/03/27 14:34:07 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	@brief Expand the buffer with the filename expansion
	@details
	1. ensure the buffer (pattern) is closed;
	2. get the list of files;
	3. filter the list of files with the pattern;
	4. if there is still files,
		place them in the token list and clear the buffer,
		otherwise, the pattern will be kept in buffer and printed as is.
*/
void	filename_expansion(t_expander *exp, t_shell *sh)
{
	t_list	*files;

	exp->buf[exp->buf_i] = '\0';
	files = get_list_of_files(exp, sh);
	files = pattern_filter(files, exp);
	if (files)
	{
		list_of_file_to_token_list(files, exp, sh);
		exp->buf_i = 0;
		exp->buf[exp->buf_i] = '\0';
	}
}

/**
	@brief Get the list of files in the current directory
	Works also with simple directory path, without wildcard.
*/
t_list	*get_list_of_files(t_expander *exp, t_shell *sh)
{
	t_list			*files;
	char			*path;
	DIR				*dir;
	struct dirent	*entry;
	char			*full_name;

	files = NULL;
	full_name = NULL;
	path = extract_root_path(exp, sh);
	dir = opendir_s(path, sh);
	if (dir)
	{
		entry = readdir_s(dir, sh);
		while (entry)
		{
			full_name = strdup_s(entry->d_name, PROMPT, sh);
			if (ft_strcmp(path, "."))
				full_name = strjoin_s(path, full_name, PROMPT, sh);
			if (ft_strcmp(entry->d_name, ".") && ft_strcmp(entry->d_name, ".."))
				lst_insert_ordered(full_name, &files, sh);
			entry = readdir_s(dir, sh);
		}
		closedir_s(dir, sh);
	}
	return (files);
}

/**
	@brief Insert a new filename in the list of files, in alphabetical order
*/
void	lst_insert_ordered(char *to_add, t_list **files, t_shell *sh)
{
	t_list	*current;
	t_list	*new_node;

	new_node = ft_lstnew(strdup_s(to_add, PROMPT, sh));
	check_node_alloc(new_node, to_add, sh);
	track_alloc(new_node, PROMPT, sh);
	if (*files == NULL || ft_strcmp((*files)->content, to_add))
	{
		new_node->next = *files;
		if (*files)
			(*files)->prev = new_node;
		*files = new_node;
	}
	else
	{
		current = *files;
		while (current->next && ft_strcmp(current->next->content, to_add) < 0)
			current = current->next;
		new_node->next = current->next;
		if (current->next)
			current->next->prev = new_node;
		current->next = new_node;
		new_node->prev = current;
	}
}
