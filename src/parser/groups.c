/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   groups.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marksylaiev <marksylaiev@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 18:30:33 by dkot              #+#    #+#             */
/*   Updated: 2025/07/24 17:54:53 by marksylaiev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../init/header.h"

static void	free_argv_list(t_group *grp)
{
	t_list	*nexta;

	while (grp->argv)
	{
		nexta = grp->argv->next;
		free(((t_command *)grp->argv->content)->arg);
		free(grp->argv->content);
		free(grp->argv);
		grp->argv = nexta;
	}
}

static void	free_heredoc_list(t_group *grp)
{
	t_list			*nexth;
	t_heredoc_data	*hd;

	while (grp->heredocs)
	{
		nexth = grp->heredocs->next;
		hd = (t_heredoc_data *)grp->heredocs->content;
		free(hd->delimiter);
		close(hd->fd);
		free(hd);
		free(grp->heredocs);
		grp->heredocs = nexth;
	}
}

void	free_groups(t_list **groups)
{
	t_list	*nextg;
	t_group	*grp;

	while (*groups)
	{
		nextg = (*groups)->next;
		grp = (*groups)->content;
		free_argv_list(grp);
		free_heredoc_list(grp);
		free(grp);
		free(*groups);
		*groups = nextg;
	}
}

t_group	*new_group(void)
{
	t_group	*g;

	g = malloc(sizeof(*g));
	if (!g)
		return (NULL);
	g->argv = NULL;
	g->heredocs = NULL;
	return (g);
}
