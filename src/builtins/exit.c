/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:36:25 by ugerkens          #+#    #+#             */
/*   Updated: 2024/03/27 12:40:37 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
	@brief exit builtin without options
	@note implementation is mainly about error handling
	- if there is no argument, the shell will exit with the last status
	- if there is a first argument, it must be a valid 64bit signed integer
		if not it will print an error message and exit with status 2
		if it is, it will exit with the casted value into uint8_t
	- if there are more than 2 arguments, and a first valid one, it will print
	 an error message and return EXIT_FAILURE (so not exit the shell)
*/
int	exit_builtin(t_ast_command *cmd, t_shell *sh)
{
	if (cmd && cmd->args && count_strs(cmd->args) >= 2)
	{
		if (!is_valid_64bit_signed_integer(cmd->args[1]))
		{
			report_error("exit: ", cmd->args[1],
				": numeric argument required", sh);
			quit_shell(2, sh);
		}
		else if (count_strs(cmd->args) > 2)
		{
			report_error("exit: ", NULL, "too many arguments", sh);
			return (EXIT_FAILURE);
		}
		else
			quit_shell((uint8_t)ft_atoi_long_long(cmd->args[1]), sh);
	}
	quit_shell(sh->last_status, sh);
	return (EXIT_SUCCESS);
}

bool	is_number(const char *str)
{
	if (!str || *str == '\0')
		return (false);
	while (*str == ' ')
		str++;
	if (*str == '-' || *str == '+')
		str++;
	if (*str == '\0')
		return (false);
	while (*str)
	{
		if (!ft_isdigit(*str) && *str != ' ')
			return (false);
		str++;
	}
	while (*str == ' ')
		str++;
	if (*str != '\0')
		return (false);
	return (true);
}

bool	is_valid_64bit_signed_integer(const char *str)
{
	long long		max_div_10;
	long long		value;
	int				negative_flag;
	int				digit;

	max_div_10 = LLONG_MAX / 10;
	if (!is_number(str))
		return (false);
	negative_flag = 0;
	if (*str == '-' || *str == '+')
	{
		negative_flag = (*str == '-');
		str++;
	}
	value = 0;
	while (*str)
	{
		digit = *str - '0';
		if (value > max_div_10 || (value == max_div_10 && digit > (7
					+ negative_flag)))
			return (false);
		value = value * 10 + digit;
		str++;
	}
	return (true);
}

long long	ft_atoi_long_long(const char *str)
{
	long long	value;
	int			sign;

	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	sign = 1;
	if (*str == '-' || *str == '+')
		sign = 44 - *str++;
	value = 0;
	while (ft_isdigit(*str))
		value = value * 10 + (*str++ - '0');
	return (sign * value);
}
