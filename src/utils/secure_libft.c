/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   secure_libft.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:38:41 by ugerkens          #+#    #+#             */
/*   Updated: 2024/02/01 16:38:42 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*strjoin_s(char const *s1, char const *s2, t_tracking_scope scope,
	t_shell *sh)
{
	char	*new_str;

	new_str = ft_strjoin(s1, s2);
	s_alloc(new_str, scope, sh);
	return (new_str);
}

char	**split_s(char const *s, char c, t_tracking_scope scope, t_shell *sh)
{
	char	**new_str;
	int		i;

	new_str = ft_split(s, c);
	s_alloc(new_str, scope, sh);
	i = 0;
	while (new_str && new_str[i])
		s_alloc(new_str[i++], scope, sh);
	return (new_str);
}

char	*strdup_s(char const *s1, t_tracking_scope scope, t_shell *sh)
{
	char	*new_str;

	new_str = ft_strdup(s1);
	s_alloc(new_str, scope, sh);
	return (new_str);
}

ssize_t	write_s(const char *s, int fd, t_shell *sh)
{
	ssize_t	ret;

	ret = ft_write_fd(s, fd);
	if (ret == -1)
		syscall_error("ft_write_fd", errno, sh);
	return (ret);
}
