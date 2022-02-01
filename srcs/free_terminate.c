/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_terminate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfranke <dfranke@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 13:46:23 by dfranke           #+#    #+#             */
/*   Updated: 2022/02/01 15:45:34 by dfranke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

void	free_env(t_env *env)
{
	t_cmd	*tmp;

	free(env->shell);
	free(env->lmt);
	while (env->fir_cmd)
	{
		tmp = env->fir_cmd->next;
		free_array(env->fir_cmd->array);
		free(env->fir_cmd->pth);
		free(env->fir_cmd);
		env->fir_cmd = tmp;
	}
	free(env);
}

int	terminate(t_env *env, int error, char *str)
{
	write(2, env->shell, ft_strlen(env->shell));
	if (error == ERR_PTH)
	{
		write(2, ": memory allocation failed", 26);
		exit (0);
	}
	if (error == ERR_PTH)
	{
		write(2, ": no such file or directory: ", 29);
		ft_putendl_fd(str, 2);
		exit(1);
	}
	if (error == ERR_PER)
	{
		write(2, ": permission denied: ", 21);
		ft_putendl_fd(str, 2);
		exit(126);
	}
	if (error == ERR_CMD)
	{
		write(2, ": command not found: ", 21);
		ft_putendl_fd(str, 2);
		exit(127);
	}
	return (ERR);
}

/*
Error messages for bad permissions, file/cmd nonexistend or malloc fail
*/

void	wrong_input(int argc)
{
	char	a;

	a = (5 - argc) + '0';
	if ('1' < a)
	{
		ft_putstr_fd("Error: Pipex needs at least ", 2);
		ft_putchar_fd(a, 2);
		ft_putendl_fd(" additional arguments", 2);
	}
	else
		ft_putendl_fd("Error: Pipex needs at least 1 additional argument", 2);
}

/*
Error messages for wrong input
*/