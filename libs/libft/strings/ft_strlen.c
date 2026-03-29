/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:54:32 by avelandr          #+#    #+#             */
/*   Updated: 2025/12/18 16:55:49 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

unsigned long	ft_strlen(const char *str)
{
	char	*s;

	s = (char *)str;
	while (*s)
		s++;
	return (s - str);
}
