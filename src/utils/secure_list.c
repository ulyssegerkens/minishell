/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secure_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:38:42 by ugerkens          #+#    #+#             */
/*   Updated: 2024/03/27 14:27:30 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	lst_add_front_s(void *content, t_list **lst, t_tracking_scope scope,
		t_shell *sh)
{
	t_list	*new_node;

	new_node = ft_lstnew(content);
	check_node_alloc(new_node, content, sh);
	ft_lstadd_front(lst, new_node);
	track_alloc(new_node, scope, sh);
}

void	lst_add_back_s(void *content, t_list **lst, t_tracking_scope scope,
		t_shell *sh)
{
	t_list	*new_node;

	new_node = ft_lstnew(content);
	check_node_alloc(new_node, content, sh);
	ft_lstadd_back(lst, new_node);
	track_alloc(new_node, scope, sh);
}

void	check_node_alloc(t_list *new_node, void *pointer, t_shell *sh)
{
	if (!new_node)
	{
		free(pointer);
		error("Memory allocation", strerror(errno), EXIT_FAILURE, sh);
	}
}

/**
	@brief remove a node from a list
 */
void	remove_list_node(t_list **node, t_list **head,
		void (*free_function)(void *), bool free_node)
{
	t_list	*to_remove;

	if (!node || !*node)
		return ;
	to_remove = *node;
	if (to_remove->prev)
		to_remove->prev->next = to_remove->next;
	else
		*head = to_remove->next;
	if (to_remove->next)
		to_remove->next->prev = to_remove->prev;
	if (free_node)
		ft_lstdelone(to_remove, free_function);
}
