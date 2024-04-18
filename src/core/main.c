/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:36:38 by ugerkens          #+#    #+#             */
/*   Updated: 2024/02/05 13:30:42 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	sh;
	int		status;

	(void) argv;
	if (argc != 1)
		exit(EXIT_FAILURE);
	set_signal_main_process();
	init_shell(&sh, envp);
	status = interactive_shell(&sh);
	quit_shell(status, &sh);
}
