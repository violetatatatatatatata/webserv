/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:51:34 by avelandr          #+#    #+#             */
/*   Updated: 2025/12/18 16:52:03 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			iter;
	unsigned char	*s;
	unsigned char	*z;

	iter = 0;
	s = (unsigned char *)s1;
	z = (unsigned char *)s2;
	while (iter < n)
	{
		if (*s != *z)
		{
			return (*s - *z);
		}
		iter++;
		s++;
		z++;
	}
	return (0);
}
