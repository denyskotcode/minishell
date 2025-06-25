/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marksylaiev <marksylaiev@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:14:05 by dkot              #+#    #+#             */
/*   Updated: 2025/07/14 17:39:47 by marksylaiev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../init/header.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

int	is_not_empty(const char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (!ft_isspace((unsigned char)str[i]))
			return (1);
		i++;
	}
	return (0);
}

char	*ft_readline(const char *prompt)
{
	char	*line;

	line = readline(prompt);
	if (line && *line && is_not_empty(line))
	{
		add_history(line);
	}
	return (line);
}

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
