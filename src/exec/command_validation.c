/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_validation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkot <dkot@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 19:21:31 by dkot              #+#    #+#             */
/*   Updated: 2025/07/18 19:23:37 by dkot             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../init/header.h"

int	validate_command(char *cmd_path, char *cmd_name)
{
	struct stat	st;

	if (stat(cmd_path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		write_error_with_arg("bash: ", cmd_name, ": Is a directory\n");
		return (126);
	}
	if (access(cmd_path, X_OK) != 0 && access(cmd_path, F_OK) == 0)
	{
		write_error_with_arg("bash: ", cmd_name, ": Permission denied\n");
		return (126);
	}
	return (0);
}

void	cleanup_memory(char **args, char **env)
{
	int	i;

	if (env != NULL)
	{
		i = 0;
		while (env[i] != NULL)
		{
			free(env[i]);
			++i;
		}
		free(env);
	}
	if (args != NULL)
	{
		i = 0;
		while (args[i] != NULL)
		{
			free(args[i]);
			++i;
		}
		free(args);
	}
}
