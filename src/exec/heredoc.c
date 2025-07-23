/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkot <dkot@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:14:05 by dkot              #+#    #+#             */
/*   Updated: 2025/07/23 18:55:26 by dkot             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../init/header.h"

void	heredoc_child_process(int write_fd, const char *delimiter)
{
	char	*line;
	size_t	delim_len;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	delim_len = ft_strlen(delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, delim_len) == 0
			&& line[delim_len] == '\0')
		{
			free(line);
			break ;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
}

static int	process_heredoc_command(t_command *cmd, t_group *grp)
{
	int				heredoc_fd;
	t_heredoc_data	*hd;
	t_list			*heredoc_node;

	heredoc_fd = create_heredoc_fd(cmd->arg);
	if (heredoc_fd < 0)
		return (-2);
	hd = malloc(sizeof(t_heredoc_data));
	if (!hd)
	{
		close(heredoc_fd);
		return (-1);
	}
	hd->delimiter = ft_strdup(cmd->arg);
	hd->fd = heredoc_fd;
	heredoc_node = ft_lstnew(hd);
	if (!heredoc_node)
	{
		free(hd->delimiter);
		free(hd);
		close(heredoc_fd);
		return (-1);
	}
	ft_lstadd_back(&grp->heredocs, heredoc_node);
	return (0);
}

static int	process_argv_heredocs(t_group *grp)
{
	t_list		*argv_node;
	t_command	*cmd;
	int			result;

	argv_node = grp->argv;
	while (argv_node)
	{
		cmd = (t_command *)argv_node->content;
		if (ft_strcmp(cmd->arg, "<<") == 0)
		{
			argv_node = argv_node->next;
			if (argv_node)
			{
				cmd = (t_command *)argv_node->content;
				result = process_heredoc_command(cmd, grp);
				if (result < 0)
					return (result);
			}
		}
		argv_node = argv_node->next;
	}
	return (0);
}

int	preprocess_heredocs(t_list *groups)
{
	t_list	*group_node;
	t_group	*grp;
	int		result;

	group_node = groups;
	while (group_node)
	{
		grp = (t_group *)group_node->content;
		result = process_argv_heredocs(grp);
		if (result < 0)
			return (result);
		group_node = group_node->next;
	}
	return (0);
}
