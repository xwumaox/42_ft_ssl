/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bignum_div_mod.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acazuc <acazuc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/06 21:34:44 by acazuc            #+#    #+#             */
/*   Updated: 2018/07/07 19:20:08 by acazuc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bignum.h"

static int	do_clear(t_bignum *tmp1, t_bignum *tmp2, t_bignum *tmp3, t_bignum *tmp4)
{
	bignum_free(tmp1);
	bignum_free(tmp2);
	bignum_free(tmp3);
	bignum_free(tmp4);
	return (0);
}

static int	do_init1(t_bignum *a, t_bignum *b)
{
	bignum_trunc(a);
	bignum_trunc(b);
	if (bignum_is_zero(b))
		return (0);
	return (1);
}

static int	do_init2(t_bignum **tmp1, t_bignum **tmp2, t_bignum **tmp3, t_bignum **tmp4)
{
	*tmp1 = NULL;
	*tmp2 = NULL;
	*tmp3 = NULL;
	*tmp4 = NULL;
	if (!(*tmp1 = bignum_new()))
		return (0);
	if (!(*tmp2 = bignum_new()))
		return (0);
	if (!(bignum_resize(*tmp2, 1)))
		return (0);
	if (!(*tmp3 = bignum_new()))
		return (0);
	if (!(*tmp4 = bignum_new()))
		return (0);
	return (1);
}

int	bignum_div_mod(t_bignum *dv, t_bignum *rm, t_bignum *a, t_bignum *b)
{
	t_bignum	*tmp1;
	t_bignum	*tmp2;
	t_bignum	*tmp3;
	t_bignum	*tmp4;
	uint64_t	tmp;
	uint64_t	i;
	uint64_t	x;
	uint64_t	l;
	uint64_t	s;

	if (!do_init1(a, b))
		return (0);
	if (bignum_is_zero(a))
	{
		if (dv)
			bignum_zero(dv);
		if (rm)
			bignum_zero(rm);
		return (1);
	}
	if (!do_init2(&tmp1, &tmp2, &tmp3, &tmp4))
		return (do_clear(tmp1, tmp2, tmp3, tmp4));
	i = a->len - 1;
	while (1)
	{
		if (!bignum_grow_front(tmp1, a->data[i]))
			return (do_clear(tmp1, tmp2, tmp3, tmp4));
		x = 0;
		l = 0;
		s = BIGNUM_BASE;
		while (l <= s)
		{
			tmp = (l + s) >> 1;
			tmp2->data[0] = tmp;
			if (!bignum_mul(tmp3, b, tmp2))
				return (do_clear(tmp1, tmp2, tmp3, tmp4));
			if (bignum_cmp(tmp3, tmp1) > 0)
			{
				x = tmp;
				s = tmp - 1;
			}
			else
				l = tmp + 1;
		}
		tmp2->data[0] = x - 1;
		if (!bignum_mul(tmp3, tmp2, b))
			return (do_clear(tmp1, tmp2, tmp3, tmp4));
		if (!bignum_sub(tmp1, tmp1, tmp3))
			return (do_clear(tmp1, tmp2, tmp3, tmp4));
		if (tmp4 && !(bignum_grow_front(tmp4, x - 1)))
			return (do_clear(tmp1, tmp2, tmp3, tmp4));
		if (!i)
			break;
		--i;
	}
	bignum_trunc(tmp1);
	if (rm && !bignum_copy(rm, tmp1))
		return (do_clear(tmp1, tmp2, tmp3, tmp4));
	bignum_trunc(tmp4);
	if (dv && !bignum_copy(dv, tmp4))
		return (do_clear(tmp1, tmp2, tmp3, tmp4));
	bignum_free(tmp1);
	bignum_free(tmp2);
	bignum_free(tmp3);
	bignum_free(tmp4);
	return (1);
}
