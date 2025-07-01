/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkot <dkot@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:30:00 by marksylaiev       #+#    #+#             */
/*   Updated: 2025/07/10 18:05:56 by dkot             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../init/header.h"

int	is_command_token(t_e_type type)
{
	return (type == WORD || type == FIELD || type == EXP_FIELD);
}

static int	validate_pipe_token(int *has_command_before, int *expecting_command)
{
	if (!*has_command_before)
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		return (0);
	}
	*expecting_command = 1;
	*has_command_before = 0;
	return (1);
}

int	process_token_for_validation(t_token *token, int *has_command_before,
		int *expecting_command)
{
	if (token->type == SEP)
		return (1);
	if (token->type == PIPE)
	{
		if (!validate_pipe_token(has_command_before, expecting_command))
			return (0);
	}
	else if (is_command_token(token->type))
	{
		*has_command_before = 1;
		*expecting_command = 0;
	}
	return (1);
}

int	validate_pipe_syntax(t_list *tokens)
{
	t_list	*current;
	t_token	*token;
	int		has_command_before;
	int		expecting_command;

	if (!tokens)
		return (1);
	current = tokens;
	has_command_before = 0;
	expecting_command = 0;
	while (current)
	{
		token = (t_token *)current->content;
		if (!process_token_for_validation(token, &has_command_before,
				&expecting_command))
			return (0);
		current = current->next;
	}
	if (expecting_command)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (0);
	}
	return (1);
}
