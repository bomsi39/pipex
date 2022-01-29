/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfranke <dfranke@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 13:37:20 by dfranke           #+#    #+#             */
/*   Updated: 2022/01/29 14:10:59 by dfranke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_ends(t_env *env, t_cmd *node)
{
	int	j;

	j = 0;
	while (j < env->pipe_no)
	{
		if (j != node->idx - 1 || node == env->fir_cmd)
		{
			if (close(env->pipes[j][0]) != 0)
				perror("close [X][0]");
		}
		if (j != node->idx || node == env->lst_cmd)
		{
			if (close(env->pipes[j][1]) != 0)
				perror("close [X][1]");
		}
		j++;
	}
}

void	exec_c(t_env *env, t_cmd *node)
{
	int		i;
	char	*cmd;

	if (dup2(node->s_in, STDIN_FILENO) < 0)
		perror("1st dup");
	if (dup2(node->s_out, STDOUT_FILENO) < 0)
		perror("2nd dup");
	close_ends(env, node);
	i = 0;
	while (env->pths[i])
	{
		cmd = ft_strinsjoin(env->pths[i], node->array[0], '/');
		if (!access(cmd, X_OK))
		{
			execve(cmd, node->array, env->envp_c);
			perror("Execve: ");
		}
		//write(2, node->array[0], 100);
		free(cmd);
		i++;
	}
	write(2, env->shell, ft_strlen(env->shell));
	write(2, ": command not found: ", 21);
	ft_putendl_fd(node->array[0], 2);
}

void	fork_loop(t_env *env)
{
	t_cmd	*node;

	node = env->fir_cmd;
	while (node)
	{
		node->child = fork();
		if (node->child < 0)
			perror("Error: ");
		else if (!node->child)
			break ;
		node = node->next;
	}
	if (node)
		exec_c(env, node);
}

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
