/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfranke <dfranke@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 13:37:20 by dfranke           #+#    #+#             */
/*   Updated: 2022/02/01 15:43:40 by dfranke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_ends(t_env *env, t_cmd *node)
{
	int	j;

	j = 0;
	if (!env->lmt->islmt)
		node->idx--;
	while (j < env->pipe_no)
	{
		if (node->idx == 0 || j != node->idx - 1)
		{
			if (close(env->pipes[j][0]))
				perror("close [X][0]");
		}
		if (j != node->idx || node == env->lst_cmd)
		{
			if (close(env->pipes[j][1]))
				perror("close [X][1]");
		}
		j++;
	}
}

/*
All unused file descriptors are being closed for every cmd/child.
If no limiter is set, IDX shifts -1 for every command in order
to close the correct pipes.

BASIC INUT:
1. CMD NEEDS					[0][1]	--> close [0][0]	[1][0] [1][1]
2. CMD NEEDS		[0][0]	&&	[1][1]	--> close [0][1]	[1][0]
3. CMD NEEDS		[1][0]				--> close [0][0]	[0][1] [1][1]

WITH LIMITER:
LIM WRITE  NEEDS				[0][1]	-->	close [0][0]	[1][0] [1][1]
1. CMD NEEDS		[0][0]	&&	[1][1]	--> close [0][1]	[1][0]
2. CMD NEEDS		[1][0]				--> close [0][0]	[0][1] [1][1]
*/

void	exec_c(t_env *env, t_cmd *node)
{
	if (dup2(node->s_in, STDIN_FILENO) < 0)
		perror("1st dup");
	if (dup2(node->s_out, STDOUT_FILENO) < 0)
		perror("2nd dup");
	close_ends(env, node);
	if (execve(node->pth, node->array, env->envp_c))
		perror("execve: ");
}

/*
In ecec_c the STDIN / STDOUT fd's of the commands are being replaced by the
fd's from the pipe array for evey child/cmd. Stored via set_io (utils.c) in 
s_in & s_out.
close_ends closes all unsused fd's from the pipe array.
Finally execve is executed with the matching file path.
*/

void	fork_loop(t_env *env)
{
	t_cmd	*node;

	node = env->fir_cmd;
	while (node)
	{
		node->pid = fork();
		if (node->pid < 0)
			perror("cmd fork: ");
		else if (!node->pid)
			break ;
		node = node->next;
	}
	if (node)
		exec_c(env, node);
}

/*
In the fork loop, fork is executed for every cmd once. The new child breaks
out of this loop and ecec_c is executed for the child (parent is !node 
after it reaches the last cmd.)
*/

void	pipex(t_env *env)
{
	int		status;
	int		i;

	i = 0;
	while (i < env->pipe_no)
	{
		if (pipe(env->pipes[i++]) < 0)
			perror("pipe: ");
	}
	set_io(env);
	if (env->lmt->islmt)
		fork_lmt(env);
	if (env->lmt->pid)
		fork_loop(env);
	i = 0;
	while (i < env->pipe_no)
	{
		close(env->pipes[i][0]);
		close(env->pipes[i++][1]);
	}
	i = -1;
	while (++i < env->cmd_no)
		waitpid(-1, &status, 0);
}

/*
In my approach, the parend process creates a child for every command and 
for the bonus as well the I/O is being passed from one child to the other.
The parent process does not execute any command, its waiting for the children
to be finished.

In pipex, at first the 2d array thats holding the pipes fd's is being filled
In set_io, stin and stout variable is set for every command.
If a here_doc is part of the program input, fork_lmt is executed.
If it is executed, its child process has env->lmt->pid = 0. This way its child
doesnt enter the fork loop. Just the parent does.
If fork_lmt is not entered, env->lmt->pid was orignially set as -1 so the 
parent will enter here as well.
All open fds are being closed for the parent and children and finally the
father waits for every child to finish.
*/
