/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd_list.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfranke <dfranke@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 13:29:45 by dfranke           #+#    #+#             */
/*   Updated: 2022/01/30 19:56:29 by dfranke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

t_lmt	*create_lmt(void)
{
	t_lmt	*lmt;

	lmt = malloc(sizeof(t_lmt));
	if (!lmt)
		exit(1);
	lmt->islmt = false;
	lmt->lmtr = NULL;
	lmt->pid = -1;
	return (lmt);
}

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

void	create_cmd(t_env *env, char **cmd_a)
{
	t_cmd		*cmd;
	static int	i;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		exit(1);
	cmd->pid = 0;
	cmd->array = cmd_a;
	cmd->s_in = 0;
	cmd->s_out = 0;
	cmd->pth = NULL;
	cmd->idx = 1 + i++;
	cmd->next = NULL;
	link_cmds(env, cmd);
}

t_env	*init_env(int argc, char **envp)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	if (!env)
		exit(1);
	env->fir_cmd = NULL;
	env->lst_cmd = NULL;
	env->lmt = create_lmt();
	env->fdin = 0;
	env->fdout = 0;
	env->envp_c = envp;
	env->ac = argc;
	env->cmd_no = argc - 3;
	env->pipe_no = env->cmd_no - 1;
	env->error = 0;
	return (env);
}
