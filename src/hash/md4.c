/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md4.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acazuc <acazuc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/13 14:30:53 by acazuc            #+#    #+#             */
/*   Updated: 2018/08/13 17:03:13 by acazuc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hash/md4.h"
#include "ft_ssl.h"

t_hash		g_hash_md4 = {(t_hash_init)&md4_init
		, (t_hash_update)&md4_update, (t_hash_final)&md4_final
		, 16, 64, sizeof(t_md4_ctx), "MD4"};

static uint32_t	g_md4_s[48] = {3, 7, 11, 19, 3, 7, 11, 19, 3, 7, 11, 19, 3, 7, 11, 19
			, 3, 5, 9, 13, 3, 5, 9, 13, 3, 5, 9, 13, 3, 5, 9, 13
			, 3, 9, 11, 15, 3, 9, 11, 15, 3, 9, 11, 15, 3, 9, 11, 15};

static uint32_t	g_md4_g[48] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
			, 0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15
			, 0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};

static uint32_t	g_md4_k[48] = {0, 0x5a827999, 0x6ed9eba1};

static void	md4_loop(t_md4_ctx *ctx, int i, uint32_t *tmp)
{
	uint32_t	f;

	if (i <= 15)
		f = (tmp[1] & tmp[2]) | ((~tmp[1]) & tmp[3]);
	else if (i <= 31)
		f = (tmp[1] & tmp[2]) | (tmp[1] & tmp[3]) | (tmp[2] & tmp[3]);
	else
		f = tmp[1] ^ tmp[2] ^ tmp[3];
	f += tmp[0] + g_md4_k[i / 16] + ctx->data[g_md4_g[i]];
	tmp[0] = tmp[3];
	tmp[3] = tmp[2];
	tmp[2] = tmp[1];
	tmp[1] = rotate_left32(f, g_md4_s[i]);
}

static void	md4_chunk(t_md4_ctx *ctx)
{
	uint32_t	tmp[4];
	int		i;

	ft_memcpy(tmp, ctx->h, 16);
	i = -1;
	i = -1;
	while (++i < 48)
		md4_loop(ctx, i, tmp);
	i = -1;
	while (++i < 4)
		ctx->h[i] += tmp[i];
}

int		md4_init(t_md4_ctx *ctx)
{
	ctx->total_len = 0;
	ctx->data_len = 0;
	ctx->h[0] = 0x67452301;
	ctx->h[1] = 0xefcdab89;
	ctx->h[2] = 0x98badcfe;
	ctx->h[3] = 0x10325476;
	return (1);
}

int		md4_update(t_md4_ctx *ctx, const uint8_t *data, size_t len)
{
	while (1)
	{
		if (ctx->data_len + len < 64)
		{
			ft_memcpy((char*)ctx->data + ctx->data_len, data, len);
			ctx->data_len += len;
			ctx->total_len += len;
			return (1);
		}
		ft_memcpy((char*)ctx->data + ctx->data_len, data, 64 - ctx->data_len);
		len -= 64 - ctx->data_len;
		data += 64 - ctx->data_len;
		ctx->total_len += 64 - ctx->data_len;
		ctx->data_len = 0;
		md4_chunk(ctx);
	}
	return (1);
}

int		md4_final(uint8_t *md, t_md4_ctx *ctx)
{
	((char*)ctx->data)[ctx->data_len] = 0x80;
	ctx->data_len++;
	if (ctx->data_len > 56)
	{
		while (ctx->data_len < 64)
			((char*)ctx->data)[ctx->data_len++] = 0;
		md4_update(ctx, NULL, 0);
	}
	while (ctx->data_len < 56)
		((char*)ctx->data)[ctx->data_len++] = 0;
	ctx->total_len *= 8;
	ft_memcpy(ctx->data + 14, &ctx->total_len, 8);
	ctx->data_len = 64;
	md4_update(ctx, NULL, 0);
	ft_memcpy(md + 0, &ctx->h[0], 4);
	ft_memcpy(md + 4, &ctx->h[1], 4);
	ft_memcpy(md + 8, &ctx->h[2], 4);
	ft_memcpy(md + 12, &ctx->h[3], 4);
	return (1);
}
