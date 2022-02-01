/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfranke <dfranke@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 13:32:51 by dfranke           #+#    #+#             */
/*   Updated: 2022/02/01 15:43:13 by dfranke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	access_io(t_env *env, char **argv)
{
	if (!env->lmt->islmt)
	{
		if (access(argv[1], F_OK))
			env->error = terminate(env, ERR_PTH, argv[1]);
		if (access(argv[1], R_OK))
			env->error = terminate(env, ERR_PER, argv[1]);
	}
	if (!access(argv[env->ac - 1], F_OK))
	{
		if (access(argv[env->ac - 1], W_OK))
			env->error = terminate(env, ERR_PER, argv[env->ac - 1]);
	}
}

/*
checks the access of input / output files.
If no limiter is set,
Existence of input file is checked, if nonexistend error is set.
Permission of input file is checked, if no read permission error is set.

In every case the existence of the output file is checked.
If the output file exists, permission is checked. If wrong permissions,
error is set.
*/

t_cmd	*cmdlast(t_cmd *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

/*
Returns the last element of cmd list.
*/

void	set_io(t_env *env)
{
	t_cmd	*node;
	int		i;

	i = 0;
	node = env->fir_cmd;
	while (node)
	{
		if (node->idx == 1 && !env->lmt->islmt)
		{
			node->s_in = env->fdin;
			node->s_out = env->pipes[0][1];
		}
		else if (node->idx < env->cmd_no)
		{
			node->s_in = env->pipes[i][0];
			node->s_out = env->pipes[i + 1][1];
			i++;
		}
		else if (node->idx == env->cmd_no)
		{
			node->s_in = env->pipes[env->pipe_no - 1][0];
			node->s_out = env->fdout;
		}
		node = node->next;
	}
}

/*
set_io defines the variables s_in & s_out for every command.
This variables will be used for dup2.
*/