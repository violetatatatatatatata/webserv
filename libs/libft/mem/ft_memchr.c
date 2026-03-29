/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:51:23 by avelandr          #+#    #+#             */
/*   Updated: 2025/12/18 16:52:04 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

void	*ft_memchr(const void *s, int c, unsigned long n)
{
	unsigned long	iter;
	void			*ns;

	ns = (void *)s;
	iter = 0;
	while (iter < n)
	{
		if (((unsigned char *)ns)[iter] == (unsigned char)c)
		{
			return (ns + iter);
		}
		iter++;
	}
	return (NULL);
}
