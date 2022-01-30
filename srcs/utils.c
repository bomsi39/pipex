/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfranke <dfranke@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 13:32:51 by dfranke           #+#    #+#             */
/*   Updated: 2022/01/30 18:31:18 by dfranke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

t_cmd	*cmdlast(t_cmd *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

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
