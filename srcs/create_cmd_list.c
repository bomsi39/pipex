/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd_list.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfranke <dfranke@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 13:29:45 by dfranke           #+#    #+#             */
/*   Updated: 2022/01/28 14:06:23 by dfranke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	link_cmds(t_env *env, t_cmd *cmd)
{
	t_cmd	*tmp;
	
	if (!env->fir_cmd)
		env->fir_cmd = cmd;
	else
	{
		tmp = cmdlast(env->fir_cmd);
		tmp->next = cmd;
	}
	env->lst_cmd = cmd;
}

void	create_cmd(t_env *env, char **cmd_a, char *argv)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	cmd->child = 0;
	cmd->array = cmd_a;
	cmd->av = argv;
	cmd->s_in = 0;
	cmd->s_out = 0;
	cmd->next = NULL;
	link_cmds(env, cmd);
}
