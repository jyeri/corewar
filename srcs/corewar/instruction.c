/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   instruction.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrummuka <jrummuka@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:53:47 by qnguyen           #+#    #+#             */
/*   Updated: 2022/11/05 15:31:54 by jrummuka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/corewar.h"

void	live(t_process *process, t_arg *arg)
{
	return ;
}

void	ld(t_process *process, t_arg *arg)
{
	//write number in argument 1 (if dir) to registry in arg2
	//or write 4bytes in argument 1 memory location (arg1 % IDX_MOD) to registry in arg2
	//if value of arg1 is 0 set carry to 1
	//if non zero set carry to 0
	uint32_t	value;

	value = get_arg_value(process, &arg[0]);
	ft_printf("value is in ld: %u\n", value);
	if (arg[0].type == IND_CODE)
		value = get_n_byte(4, &g_arena[value]);
	process->reg[arg[1].value] = value;
	process->carry = (process->reg[arg[1].value] == 0);
	ft_printf("in ld reg: %u\n", process->reg[arg[1].value]);
}

void	st(t_process *process, t_arg *arg)
{
	//write 4 bytes from reg to arg2
	//if arg2 is regisrty, you just write them to other regisrty
	//if arg2 is ind, you take currpos + arg2 % IDX_MOD, and write value to that memory address
	uint32_t	value;

	value = get_arg_value(process, &arg[0]);
	if (arg[1].type == IND_CODE)
		write_4byte(process, value, get_arg_value(process, &arg[1]));
	else
		g_arena[arg[1].value] = value;
}

void	add(t_process *process, t_arg *arg)
{
	//write 4bytes of REG in arg1 + REG in arg2 to REG in arg3
	//if value to write is 0 set carry to 1, else to 0
	arg[2].value = get_arg_value(process, &arg[0]) + get_arg_value(process, &arg[1]);
	process->carry = (arg[2].value == 0);
}

void	sub(t_process *process, t_arg *arg)
{
	//write 4bytes of REG in arg1 - REG in arg2 to REG in arg3
	//if value to write is 0 set carry to 1, else to 0
	arg[2].value = get_arg_value(process, &arg[0]) - get_arg_value(process, &arg[1]);
	process->carry = (arg[2].value == 0);
}

void	and(t_process *process, t_arg *arg)
{
	//bitwise AND
	//bitwise AND of arg1, arg2, then write result to REG in arg3
	//if arg1/2 is REG, read value from there
	//if arg1/2 is DIR, read numerical value from argument as given
	//if arg1/2 is IND, take current position + <ARGUMENT> % IDX_MOD, read 4bytes from there
	uint32_t	arg0;
	uint32_t	arg1;

	arg0 = get_arg_value(process, &arg[0]);
	arg1 = get_arg_value(process, &arg[1]);
	if(arg[0].type == IND_CODE)
		arg0 = get_n_byte(4, &g_arena[process->pc + arg0]);
	if(arg[1].type == IND_CODE)
		arg1 = get_n_byte(4, &g_arena[process->pc + arg1]);
	arg[2].value = arg0 & arg1;
	process->carry = (arg[2].value == 0);
}

void	or(t_process *process, t_arg *arg)
{
	//bitwise OR
	//bitwise OR of arg1, arg2, then write result to REG in arg3
	//if arg1/2 is REG, read value from there
	//if arg1/2 is DIR, read numerical value from argument as given
	//if arg1/2 is IND, take current position + <ARGUMENT> % IDX_MOD, read 4bytes from there
	uint32_t	arg0;
	uint32_t	arg1;

	arg0 = get_arg_value(process, &arg[0]);
	arg1 = get_arg_value(process, &arg[1]);
	if(arg[0].type == IND_CODE)
		arg0 = get_n_byte(4, &g_arena[process->pc + arg0]);
	if(arg[1].type == IND_CODE)
		arg1 = get_n_byte(4, &g_arena[process->pc + arg1]);
	arg[2].value = arg0 | arg1;
	process->carry = (arg[2].value == 0);
}

void	xor(t_process *process, t_arg *arg)
{
	//bitwise XOR
	//bitwise XOR of arg1, arg2, then write result to REG in arg3
	//if arg1/2 is REG, read value from there
	//if arg1/2 is DIR, read numerical value from argument as given
	//if arg1/2 is IND, take current position + <ARGUMENT> % IDX_MOD, read 4bytes from there
	uint32_t	arg0;
	uint32_t	arg1;

	arg0 = get_arg_value(process, &arg[0]);
	arg1 = get_arg_value(process, &arg[1]);
	if(arg[0].type == IND_CODE)
		arg0 = get_n_byte(4, &g_arena[process->pc + arg0]);
	if(arg[1].type == IND_CODE)
		arg1 = get_n_byte(4, &g_arena[process->pc + arg1]);
	arg[2].value = arg0 ^ arg1;
	process->carry = (arg[2].value == 0);
}

//TEST
void	zjmp(t_process *process, t_arg *arg)
{
	//check if carry flag is 1
	//if it is the take current position + ARG1 % IDX_MOD, and update process pos to it.
	uint16_t	position;

	arg[0].value = get_n_byte(DIR_SIZE / 2, &g_arena[process->pc + 1]);
	//ft_printf("Jump from: %u\n", process->pc);
	position = process->pc + (int16_t)get_arg_value(process, &arg[0]) % IDX_MOD;
	if (process->carry == 1)
	{
		process->pc = position;
	}
	//ft_printf("Jump to: %d\n", (int16_t)position);
}

void	ldi(t_process *process, t_arg *arg)
{
	uint16_t	position;

	position = process->pc + (get_arg_value(process, &arg[0]) + get_arg_value(process, &arg[1])) % IDX_MOD;
	process->reg[arg[2].value] = get_n_byte(4, &g_arena[position]);
	ft_printf("pos: %u\n", position);
	ft_printf("reg ldi value: %d\n", process->reg[arg[2].value]);
}

void	sti(t_process *process, t_arg *arg)
{
	uint16_t	position;

	position = process->pc + (get_arg_value(process, &arg[1]) + get_arg_value(process, &arg[2])) % IDX_MOD;
	write_4byte(process, get_arg_value(process, &arg[0]), position);
}

void	foork(t_process *process, t_arg *arg)
{

}

void	lld(t_process *process, t_arg *arg)
{
	//same ld (writes value from 1st arg to reg in arg2)
	//but in indirect without truncating by IDX_MOD!! line 143 check
	uint32_t	value;

	if (arg[0].type == IND_CODE)
	{
		value = process->pc + (uint16_t)(arg[0].value);
		value = get_position(value);
		ft_printf("value is in lld: %u\n", value);
		value = get_n_byte(4, &g_arena[value]);
	}
	else
	{
		value = get_arg_value(process, &arg[0]);
	}
	process->reg[arg[1].value] = value;
	process->carry = (process->reg[arg[1].value] == 0);
//	ft_printf("in value reg: %u\n", value);
//	ft_printf("in lld reg: %u\n", process->reg[arg[1].value]);
}

void	lldi(t_process *process, t_arg *arg)
{
	//similar to the ldi statement.
	//It writes the value to the registry, which was passed to it as the third parameter. The value that this statement writes is 4 bytes that it read.
	//The bytes are read at the address, which is formed according to the following principle: current position + (<FIRST_ARGUMENT_VALUE> + <SECOND_ARGUMENT_VALUE>).
	//Unlike the ldi statement, in this case, when forming the address, you shouldn't truncate by modulo IDX_MOD.
	uint16_t	position;
	uint32_t	value;

	if (arg[0].type == IND_CODE)
	{
		value = process->pc + (uint16_t)(arg[0].value);
		value = get_position(value);
		ft_printf("value is in lldi: %u\n", value);
		value = get_n_byte(4, &g_arena[value]);
	}
	else
		value = get_arg_value(process, &arg[0]);
	position = process->pc + (value + get_arg_value(process, &arg[1]));
	position = get_position(position);
	process->reg[arg[2].value] = get_n_byte(4, &g_arena[position]);
//	ft_printf("pos: %u\n", position);
	// ft_printf("reg value: %u\n", process->reg[arg[2].value]);
	ft_printf("in lldi reg: %u\n", process->reg[arg[2].value]);
}

void	lfork(t_process *process, t_arg *arg)
{

}

void	aff(t_process *process, t_arg *arg)
{

}
