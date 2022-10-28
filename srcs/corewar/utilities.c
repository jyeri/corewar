/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qnguyen <qnguyen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 13:00:11 by qnguyen           #+#    #+#             */
/*   Updated: 2022/10/28 14:55:16 by qnguyen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/corewar.h"

int	get_4byte(unsigned char *size_byte)
{
	int	value;
	int	i;

	value = 0;
	i = 0;
	while (i < 4)
		value = (value * 0x100) + size_byte[i++];
	return (value);
}

//test functions

void	print_mem(int size, unsigned char *mem)
{
	int	i;

	i = 0;
	while (i < size)
	{
		ft_printf("%02x ", mem[i++]);
		if (i % 64 == 0)
			ft_printf("\n");
	}
	ft_printf("\n");
}
