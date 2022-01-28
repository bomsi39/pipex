/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfranke <dfranke@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 13:46:23 by dfranke           #+#    #+#             */
/*   Updated: 2022/01/28 13:46:34 by dfranke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_env(t_env *env)
{
	t_cmd	*tmp;
	int		i;

	i = 0;
	while (env->pths[i])
		free(env->pths[i++]);
	free(env->pths);
	while (env->fir_cmd)
	{
		tmp = env->fir_cmd->next;
		i = 0;
		while (env->fir_cmd->array[i])
			free(env->fir_cmd->array[i++]);
		free(env->fir_cmd->array);
		free(env->fir_cmd);
		env->fir_cmd = tmp;
	}
	free(env);
}