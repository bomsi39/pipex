/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfranke <dfranke@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 13:32:51 by dfranke           #+#    #+#             */
/*   Updated: 2022/01/29 14:12:29 by dfranke          ###   ########.fr       */
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
		if (node == env->fir_cmd)
		{
			node->s_in = env->fdin;
			node->s_out = env->pipes[0][1];
		}
		if (node != env->fir_cmd && node != env->lst_cmd)
		{
			node->s_in = env->pipes[i][0];
			node->s_out = env->pipes[i + 1][1];
			i++;
		}
		if (node == env->lst_cmd)
		{
			node->s_in = env->pipes[env->pipe_no - 1][0];
			node->s_out = env->fdout;
		}
		node = node->next;
	}
}
