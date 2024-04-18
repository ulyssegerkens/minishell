/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secure_allocation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:38:39 by ugerkens          #+#    #+#             */
/*   Updated: 2024/02/01 16:38:40 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*s_alloc(void *pointer, t_tracking_scope scope, t_shell *sh)
{
	if (!pointer)
		error("Memory allocation", strerror(errno), EXIT_FAILURE, sh);
	if (scope != ONLY_CHECK)
		track_alloc(pointer, scope, sh);
	return (pointer);
}

void	*calloc_s(size_t count, size_t size, t_tracking_scope scope,
		t_shell *sh)
{
	return (s_alloc(ft_calloc(count, size), scope, sh));
}

void	track_alloc(void *pointer, t_tracking_scope scope, t_shell *sh)
{
	t_list	*new_node;

	new_node = ft_lstnew(pointer);
	check_node_alloc(new_node, pointer, sh);
	ft_lstadd_front(&sh->allocated_pointers[scope], new_node);
}
