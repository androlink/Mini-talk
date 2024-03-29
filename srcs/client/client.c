/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcros <gcros@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 00:28:40 by gcros             #+#    #+#             */
/*   Updated: 2024/01/31 00:32:52 by gcros            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _XOPEN_SOURCE 700
#include "client.h"
#include <limits.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int	ft_send_str(char *str, int pid);
int	ft_send_byte(t_byte byte, int pid);
int	ft_strisnumber(const char *s);
int	ft_atoi(const char *nptr);

int	main(int ac, char **av)
{
	int	pid;

	ft_client_init();
	if (ac != 3)
		return (1);
	if (!ft_strisnumber(av[1]) && av[1][0] == '-')
		return (2);
	pid = ft_atoi(av[1]);
	if (pid < 0)
		return (2);
	if (kill(pid, 0) == -1)
		exit(-2);
	return (ft_send_str(av[2], pid));
}

int	ft_send_str(char *str, int pid)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (ft_send_byte(str[i], pid) == -1)
			return (-1);
		i++;
	}
	if (ft_send_byte('\0', pid) == -1)
		return (-1);
	return (0);
}

int	ft_send_byte(t_byte byte, int pid)
{
	int	bit_shift;

	bit_shift = 7;
	while (bit_shift >= 0)
	{
		if (ft_send_bit((byte >> bit_shift) & 1, pid) == -1)
			return (-1);
		if (get_feedback() == SIGUSR2)
			return (-1);
		bit_shift--;
	}
	return (1);
}

int	ft_strisnumber(const char *s)
{
	if (*s == '-' || *s == '+')
		s++;
	while (*s)
	{
		if (*s < '0' && *s > '9')
			return (0);
		s++;
	}
	return (1);
}

int	ft_atoi(const char *nptr)
{
	int			s;
	int			res;
	const char	*str;

	str = nptr;
	res = 0;
	s = 1;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-')
		s = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		if ((res * 10 + (*str - '0') - (s == -1)) < res)
			return (-(s == 1));
		res *= 10;
		res += *str - '0';
		str++;
	}
	return (res * s);
}
