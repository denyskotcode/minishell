/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkot <dkot@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 00:00:00 by dkot              #+#    #+#             */
/*   Updated: 2025/07/23 18:54:26 by dkot             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../init/header.h"

static int	handle_heredoc_parent_process(int *pipefd, pid_t pid)
{
	int	status;

	close(pipefd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipefd[0]);
		return (-1);
	}
	return (pipefd[0]);
}

int	create_heredoc_fd(const char *delimiter)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) < 0)
		return (-1);
	pid = fork();
	if (pid < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		heredoc_child_process(pipefd[1], delimiter);
		exit(0);
	}
	return (handle_heredoc_parent_process(pipefd, pid));
}

int	handle_heredoc_redirect(int heredoc_fd)
{
	if (dup2(heredoc_fd, STDIN_FILENO) < 0)
	{
		perror("dup2");
		return (-1);
	}
	close(heredoc_fd);
	return (0);
}
