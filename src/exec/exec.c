/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkot <dkot@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:14:05 by dkot              #+#    #+#             */
/*   Updated: 2025/07/09 21:03:47 by dkot             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../init/header.h"

int	execute_commands(t_list *groups, t_info *info)
{
	t_exec_ctx	ctx;
	t_group		*grp;
	int			result;

	init_exec_ctx(&ctx, info);
	if (!groups)
	{
		close(ctx.stdin_backup);
		close(ctx.stdout_backup);
		return (info->last_exit_status);
	}
	result = handle_heredoc_processing(groups, &ctx);
	if (result != 0)
		return (result);
	if (count_groups(groups) > 1)
		ctx.last_exit_status = execute_pipeline(groups, &ctx);
	else
	{
		grp = (t_group *)groups->content;
		ctx.last_exit_status = execute_single_command(grp, &ctx);
	}
	cleanup_and_restore(&ctx, info);
	return (ctx.last_exit_status);
}
