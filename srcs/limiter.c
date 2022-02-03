/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   limiter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfranke <dfranke@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/30 19:59:48 by dfranke           #+#    #+#             */
/*   Updated: 2022/02/03 11:16:25 by dfranke          ###   ########.fr       */
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

/*
closes all unused fd
*/

void	get_input(t_env *env)
{
	char	*line;

	line = NULL;
	close_ends_lmt(env);
	while (line || !line)
	{
		line = get_next_line(0);
		if (!ft_strncmp(line, env->lmt->lmtr, ft_strlen(env->lmt->lmtr)))
		{
			free(line);
			return ;
		}
		if (write(env->pipes[0][1], line, ft_strlen(line)) < 0)
			perror("write");
		free(line);
	}
}

/*
get_input closes at first all unused fd's of the pipes fd array and gets the 
user input via get_next_line and sends it line by line to the pipes arrays 
first input fd. If the limiter is matched. User input is terminated.
*/

void	fork_lmt(t_env *env)
{
	int	status;
	
	env->lmt->pid = fork();
	if (env->lmt->pid < 0)
		perror("limiter fork: ");
	else if (!env->lmt->pid)
		get_input(env);
	waitpid(-1, &status, 0);
}

/*
If the here_doc flag is set, fork_lmt creates a child process and sends the child
to execute get_input.
*/