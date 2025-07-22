/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkot <dkot@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 00:00:00 by dkot              #+#    #+#             */
/*   Updated: 2025/07/23 18:54:41 by dkot             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../init/header.h"

int	handle_input_redirect_case(char **argv, int *i, int *last_error)
{
	if (!argv[*i + 1])
	{
		write_error("syntax error: expected filename after '<'\n");
		return (-1);
	}
	if (handle_input_redirect(argv[*i + 1]) < 0)
		*last_error = -1;
	remove_argv_element(argv, *i);
	remove_argv_element(argv, *i);
	return (1);
}

int	handle_output_redirect_case(char **argv, int *i)
{
	if (!argv[*i + 1])
	{
		write_error("syntax error: expected filename after '>'\n");
		return (-1);
	}
	if (handle_output_redirect(argv[*i + 1]) < 0)
		return (-1);
	remove_argv_element(argv, *i);
	remove_argv_element(argv, *i);
	return (1);
}

int	handle_append_redirect_case(char **argv, int *i, int *last_error)
{
	if (!argv[*i + 1])
	{
		write_error("syntax error: expected filename after '>>'\n");
		return (-1);
	}
	if (handle_append_redirect(argv[*i + 1]) < 0)
		*last_error = -1;
	remove_argv_element(argv, *i);
	remove_argv_element(argv, *i);
	return (1);
}

int	find_and_use_heredoc(const char *delimiter, t_group *grp)
{
	t_list			*heredoc_node;
	t_heredoc_data	*hd;

	heredoc_node = grp->heredocs;
	while (heredoc_node)
	{
		hd = (t_heredoc_data *)heredoc_node->content;
		if (ft_strcmp(hd->delimiter, delimiter) == 0)
		{
			return (handle_heredoc_redirect(hd->fd));
		}
		heredoc_node = heredoc_node->next;
	}
	return (-1);
}
