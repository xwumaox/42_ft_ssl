/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_genrsa.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acazuc <acazuc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/07 19:56:57 by acazuc            #+#    #+#             */
/*   Updated: 2018/08/04 22:08:38 by acazuc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include "rsa.h"
#include "pem.h"

static int	do_init(t_genrsa_data *data)
{
	data->fdout = 1;
	data->b64_ctx.fdout = 1;
	if (!base64_write_init(&data->b64_ctx))
	{
		ft_putendl_fd("ft_ssl: failed to init b64_write", 2);
		return (0);
	}
	data->key_len = 512;
	if (!bignum_rand_add_urandom())
	{
		ft_putendl_fd("ft_ssl: failed to init rng", 2);
		return (0);
	}
	return (1);
}

static int	parge_args(t_genrsa_data *data, int ac, char **av)
{
	int	i;

	while (i < ac)
	{
		if (!ft_strcmp(av[i], "-out"))
		{
			//
		}
		else if (!ft_strcmp(av[i], "-f4"))
		{
		}
		else if (!ft_strcmp(av[i], "-3"))
		{
		}
		else if (!ft_strcmp(av[i], "-des"))
		{
		}
		else if (!ft_strcmp(av[i], "-des2"))
		{
		}
		else if (!ft_strcmp(av[i], "-des3"))
		{
		}
		else if (ft_strisnum(av[i]))
		{
		}
		else
		{
			ft_putstr_fd("ft_ssl: invalid argument: ", 2);
			ft_putendl_fd(av[i], 2);
			return (0);
		}
		++i;
	}
	return (1);
}

int	command_genrsa(int ac, char **av)
{
	t_genrsa_data	data;

	if (!do_init(&data))
		return (EXIT_FAILURE);
	if (ac)
		data.key_len = ft_atol(av[0]);
	if (data.key_len < 16)
	{
		ft_putendl_fd("ft_ssl: invalid key len, minimum is 16", 2);
		return (EXIT_FAILURE);
	}
	if (!rsa_genkey(&data.rsa_ctx, data.key_len, 1))
	{
		ft_putendl_fd("ft_ssl: failed to generate key", 2);
		return (EXIT_FAILURE);
	}
	ft_putstr("e is ");
	bignum_print(data.rsa_ctx.e);
	ft_putstr(" (0x");
	bignum_printhex(data.rsa_ctx.e);
	ft_putendl(")");
	char *dataa;
	int len;
	if ((len = pem_write_rsa_priv(&dataa, &data.rsa_ctx)) == -1)
	{
		ft_putendl_fd("ft_ssl: failed to generate pem file", 2);
		return (EXIT_FAILURE);
	}
	ft_putendl_fd("====BEGIN RSA PRIVATE KEY=====", data.fdout);
	base64_write_update(&data.b64_ctx, (uint8_t*)dataa, len);
	base64_write_final(&data.b64_ctx);
	ft_putendl_fd("====END RSA PRIVATE KEY=====", data.fdout);
	//cmd_genrsa_write(&data);
	ft_putstr("n: ");
	bignum_print(data.rsa_ctx.n);
	ft_putstr(" (");
	ft_putul(data.rsa_ctx.n->len * 4);
	ft_putstr(")\n");
	ft_putstr("p: ");
	bignum_print(data.rsa_ctx.p);
	ft_putstr(" (");
	ft_putul(data.rsa_ctx.p->len * 4);
	ft_putstr(")\n");
	ft_putstr("q: ");
	bignum_print(data.rsa_ctx.q);
	ft_putstr(" (");
	ft_putul(data.rsa_ctx.q->len * 4);
	ft_putstr(")\n");
	t_bignum *a = bignum_new();
	bignum_dec2bignum(a, "112233445566778899112233445566778899887766554433211");
	bignum_mod_exp(a, a, data.rsa_ctx.e, data.rsa_ctx.n);
	ft_putstr("Crypted: ");
	bignum_print(a);
	ft_putchar('\n');
	t_bignum *m1 = bignum_new();
	t_bignum *m2 = bignum_new();
	bignum_mod_exp(m1, a, data.rsa_ctx.dmp, data.rsa_ctx.p);
	bignum_mod_exp(m2, a, data.rsa_ctx.dmq, data.rsa_ctx.q);
	bignum_sub(m1, m1, m2);
	bignum_mul(m1, m1, data.rsa_ctx.coef);
	bignum_mod(m1, m1, data.rsa_ctx.p);
	bignum_mul(m1, m1, data.rsa_ctx.q);
	bignum_add(m1, m1, m2);
	bignum_free(m2);
	ft_putstr("Decrypted1: ");
	bignum_print(m1);
	ft_putchar('\n');
	bignum_mod_exp(a, a, data.rsa_ctx.d, data.rsa_ctx.n);
	ft_putstr("Decrypted2: ");
	bignum_print(a);
	ft_putchar('\n');
	return (1);
}
