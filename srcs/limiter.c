/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   limiter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfranke <dfranke@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/30 19:59:48 by dfranke           #+#    #+#             */
/*   Updated: 2022/01/30 20:00:36 by dfranke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_ends_lmt(t_env *env)
{
	int	j;

	j = 0;
	while (j < env->pipe_no)
	{
		if (close(env->pipes[j][0]))
			perror("close lmt [X][0]");
		if (j != 0)
		{
			if (close(env->pipes[j][1]))
				perror("close lmt [0][X]");
		}
		j++;
	}
}

void	get_input(t_env *env)
{
	char	*line;

	line = NULL;
	close_ends_lmt(env);
	while (line || !line)
	{
		line = get_next_line(0);
		if (!ft_strncmp(line, env->lmt->lmtr, ft_strlen(env->lmt->lmtr)))
			return ;
		if (write(env->pipes[0][1], line, ft_strlen(line)) < 0)
			perror("write");
	}
}

void	fork_lmt(t_env *env)
{
	if (env->lmt->islmt)
	{
		env->lmt->pid = fork();
		if (env->lmt->pid < 0)
			perror("limiter fork: ");
		else if (!env->lmt->pid)
			get_input(env);
	}
}
