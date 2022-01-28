/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfranke <dfranke@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 13:37:20 by dfranke           #+#    #+#             */
/*   Updated: 2022/01/28 14:07:02 by dfranke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	exec_c(t_env *env, t_cmd *node)
{
	int		i;
	char	*cmd;
	
	dup2(node->s_in, STDIN_FILENO);
	dup2(node->s_out, STDOUT_FILENO);
	if (node->close)
		close(node->close);
	i = 0;
	while (env->pths[i])
	{
		cmd = ft_strinsjoin(env->pths[i], node->array[0], '/');
		if(!access(cmd, X_OK))
			execve(cmd, node->array, env->envp_c);
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
			exec_c(env, node);
		node = node->next;
	}
}

void	pipex(t_env *env)
{
	int		status;
	
	pipe(env->end);
	set_io(env);
	fork_loop(env);
	close(env->end[0]);
	close(env->end[1]);
	while (waitpid(-1, &status, 0) > 0);
}
