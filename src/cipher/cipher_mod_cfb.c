/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cipher_mod_cfb.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acazuc <acazuc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/30 18:53:09 by acazuc            #+#    #+#             */
/*   Updated: 2018/08/13 21:31:31 by acazuc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cipher/cipher.h"
#include "libft.h"

t_cipher_mod	g_cipher_mod_cfb = {cipher_cfb_premod, cipher_cfb_postmod, 1};

void	cipher_cfb_premod(t_cipher_ctx *ctx, uint8_t *data)
{
	ft_memcpy(ctx->mod2, data, ctx->cipher->block_size);
	ft_memcpy(data, ctx->mod1, ctx->cipher->block_size);
	if (ctx->mode)
	{
		ctx->mode = 0;
		ctx->mod3 = 1;
	}
	else
	{
		ctx->mod3 = 0;
	}
}

void	cipher_cfb_postmod(t_cipher_ctx *ctx, uint8_t *data)
{
	ft_memxor(data, data, ctx->mod2, ctx->cipher->block_size);
	if (ctx->mod3)
	{
		ctx->mode = 1;
		ft_memcpy(ctx->mod1, ctx->mod2, ctx->cipher->block_size);
	}
	else
		ft_memcpy(ctx->mod1, data, ctx->cipher->block_size);
}
