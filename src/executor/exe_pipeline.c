/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_pipeline.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:37:15 by ugerkens          #+#    #+#             */
/*   Updated: 2024/02/01 16:37:16 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	@brief Execute a pipeline
	@details
		1. Build a linked list of the whole pipeline
		2. Execute the pipeline list
	@return the exit status of the last command in the pipeline
*/
int	execute_pipeline(t_ast *node, t_shell *sh)
{
	t_list	*pipeline;

	pipeline = build_cmd_list(node, sh);
	return (execute_pipeline_list(pipeline, sh));
}

/**
	@brief Build a linked list of the whole pipeline from the AST
*/
t_list	*build_cmd_list(t_ast *node, t_shell *sh)
{
	t_list	*pipeline;

	pipeline = NULL;
	while (node->type == AST_PIPELINE)
	{
		lst_add_front_s(node->u_data.pipeline.right, &pipeline, PROMPT, sh);
		if (node->u_data.pipeline.left->type == AST_PIPELINE)
			node = node->u_data.pipeline.left;
		else
		{
			lst_add_front_s(node->u_data.pipeline.left, &pipeline, PROMPT, sh);
			break ;
		}
	}
	return (pipeline);
}

/**
	@brief Execute a pipeline list
*/
int	execute_pipeline_list(t_list *pipeline, t_shell *sh)
{
	int		p[2];
	int		prev_read_end;
	pid_t	last_pid;
	int		last_cmd_status;
	int		n_pipeline;

	n_pipeline = ft_lstsize(pipeline);
	last_pid = 0;
	prev_read_end = -1;
	while (pipeline)
	{
		pipe_s(p, sh);
		last_pid = execute_one_pipeline(pipeline, prev_read_end, p, sh);
		setup_for_next_command(&prev_read_end, p, sh);
		pipeline = pipeline->next;
	}
	last_cmd_status = wait_for_children(last_pid, n_pipeline, sh);
	return (last_cmd_status);
}

pid_t	execute_one_pipeline(t_list *pipeline, int prev_read_end, int p[2],
		t_shell *sh)
{
	pid_t	pid;

	pid = fork_s(sh);
	if (pid != 0)
		return (pid);
	sh->in_main_process = false;
	set_signal_child_process();
	if (prev_read_end != -1)
	{
		dup2_s(prev_read_end, STDIN_FILENO, sh);
		close_s(prev_read_end, sh);
	}
	if (pipeline->next != NULL)
		dup2_s(p[WRITE_END], STDOUT_FILENO, sh);
	close_s(p[READ_END], sh);
	close_s(p[WRITE_END], sh);
	execute((t_ast *)pipeline->content, O_EXIT, sh);
	return (pid);
}
