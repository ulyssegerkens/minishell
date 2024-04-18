/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_node_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:38:00 by ugerkens          #+#    #+#             */
/*   Updated: 2024/03/27 14:45:39 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_ast_node(t_ast **node, t_ast_type type, t_shell *sh)
{
	*node = calloc_s(1, sizeof(t_ast), PROMPT, sh);
	(*node)->type = type;
}
