/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfranke <dfranke@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 13:29:45 by dfranke           #+#    #+#             */
/*   Updated: 2022/02/01 15:04:21 by dfranke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

t_lmt	*create_lmt(void)
{
	t_lmt	*lmt;

	lmt = malloc(sizeof(t_lmt));
	if (!lmt)
		return (0);
	lmt->islmt = false;
	lmt->lmtr = NULL;
	lmt->pid = -1;
	return (lmt);
}

/*
limits struct holds information about the limiter.
*/

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
		return ;
	cmd->pid = 0;
	cmd->array = cmd_a;
	cmd->s_in = 0;
	cmd->s_out = 0;
	cmd->pth = NULL;
	cmd->idx = 1 + i++;
	cmd->next = NULL;
	link_cmds(env, cmd);
}

/*
creates a new element for the cmd struct. IDX is the index of the
command, starting by 1 for the first cmd.
Link_cmds links the new element to the end of the cmd list.
*/

t_env	*init_env(int argc, char **envp)
{
	t_env	*env;

	env = malloc(sizeof(t_env) + (argc - 4) * sizeof(*(env->pipes)));
	if (!env)
		return (0);
	env->fir_cmd = NULL;
	env->lst_cmd = NULL;
	env->lmt = create_lmt();
	if (!env->lmt)
		env->error = terminate(env, ERR_MALL, "ERROR");
	env->fdin = 0;
	env->fdout = 0;
	env->envp_c = envp;
	env->ac = argc;
	env->cmd_no = argc - 3;
	env->pipe_no = argc - 4;
	env->error = 0;
	return (env);
}

/* 
Initialises the env struct. i work with an flexible array for the pipe 
I/O. In order to allocate the correct amount of memory for that array, i 
need to know how many pipes i need (argc - 4).
I also have two pointers to the cmd struct, that are supposed to point at
the first and the last cmd.
*/