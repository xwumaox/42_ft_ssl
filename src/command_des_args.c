/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_des_args.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acazuc <acazuc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/03 21:19:27 by acazuc            #+#    #+#             */
/*   Updated: 2018/07/05 15:55:42 by acazuc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include <fcntl.h>

static int	file_open(t_des_args *args, int type)
{
	args->i++;
	if (args->i >= args->ac)
		return (-1);
	if (type)
		return (open(args->av[args->i], O_WRONLY | O_CREAT | O_TRUNC, 0644));
	return (open(args->av[args->i], O_RDONLY));
}

static int	parse_args3(t_des_args *args)
{
	if (!ft_strcmp(args->av[args->i], "-s"))
	{
		if (++args->i >= args->ac)
		{
			ft_putendl_fd("ft_ssl: expected salt after -s", 2);
			return (0);
		}
		args->salt = args->av[args->i];
	}
	else if (!ft_strcmp(args->av[args->i], "-v"))
	{
		if (++args->i >= args->ac)
		{
			ft_putendl_fd("ft_ssl: expected init vector after -v", 2);
			return (0);
		}
		args->iv = args->av[args->i];
	}
	else
	{
		ft_putstr_fd("ft_ssl: invalid parameter: ", 2);
		ft_putendl_fd(args->av[args->i], 2);
		return (0);
	}
	return (1);
}

static int	parse_args2(t_des_args *args)
{
	if (!ft_strcmp(args->av[args->i], "-k"))
	{
		if (++args->i >= args->ac)
		{
			ft_putendl_fd("ft_ssl: expected key after -k", 2);
			return (0);
		}
		args->key = args->av[args->i];
	}
	else if (!ft_strcmp(args->av[args->i], "-p"))
	{
		if (++args->i >= args->ac)
		{
			ft_putendl_fd("ft_ssl: expected password after -p", 2);
			return (0);
		}
		args->password = args->av[args->i];
	}
	else
		return (parse_args3(args));
	return (1);
}

static int	parse_args1(t_des_data *data, t_des_args *args)
{
	if (!ft_strcmp(args->av[args->i], "-e"))
		data->cipher.mode = 0;
	else if (!ft_strcmp(args->av[args->i], "-d"))
		data->cipher.mode = 1;
	else if (!ft_strcmp(args->av[args->i], "-a"))
		data->base64 = 1;
	else if (!ft_strcmp(args->av[args->i], "-i"))
	{
		if (data->fdin != 0)
			close(data->fdin);
		if ((data->fdin = file_open(args, 0)) == -1)
			return (0);
	}
	else if (!ft_strcmp(args->av[args->i], "-o"))
	{
		if (data->fdout != 1)
			close(data->fdout);
		if ((data->fdout = file_open(args, 1)) == -1)
			return (0);
	}
	else
		return (parse_args2(args));
	return (1);
}

int		cmd_des_parse_args(t_des_data *data, t_des_args *args)
{
	args->i = 0;
	while (args->i < args->ac)
	{
		if (!parse_args1(data, args))
			return (0);
		++args->i;
	}
	return (1);
}