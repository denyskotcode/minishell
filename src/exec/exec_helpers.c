/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkot <dkot@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 00:00:00 by dkot              #+#    #+#             */
/*   Updated: 2025/07/23 18:54:19 by dkot             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../init/header.h"

int	count_groups(t_list *groups)
{
	int		count;
	t_list	*tmp;

	count = 0;
	tmp = groups;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

void	init_exec_ctx(t_exec_ctx *ctx, t_info *info)
{
	ctx->stdin_backup = dup(STDIN_FILENO);
	ctx->stdout_backup = dup(STDOUT_FILENO);
	ctx->last_exit_status = info->last_exit_status;
	ctx->info = info;
}

void	update_exit_status(t_exec_ctx *ctx, t_info *info)
{
	char	*exit_str;

	info->last_exit_status = ctx->last_exit_status;
	exit_str = ft_itoa(ctx->last_exit_status);
	if (exit_str)
	{
		info->env = set_env_var(info->env, "?", exit_str, 1);
		free(exit_str);
	}
}

void	cleanup_and_restore(t_exec_ctx *ctx, t_info *info)
{
	dup2(ctx->stdin_backup, STDIN_FILENO);
	dup2(ctx->stdout_backup, STDOUT_FILENO);
	close(ctx->stdin_backup);
	close(ctx->stdout_backup);
	update_exit_status(ctx, info);
}

int	handle_heredoc_processing(t_list *groups, t_exec_ctx *ctx)
{
	int	heredoc_result;

	heredoc_result = preprocess_heredocs(groups);
	if (heredoc_result < 0)
	{
		close(ctx->stdin_backup);
		close(ctx->stdout_backup);
		if (heredoc_result == -2)
			return (130);
		return (1);
	}
	return (0);
}
