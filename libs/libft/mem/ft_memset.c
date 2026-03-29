/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:51:58 by avelandr          #+#    #+#             */
/*   Updated: 2025/12/18 16:51:59 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

void	*ft_memset(void *dest, int c, size_t count)
{
	size_t	iter;

	iter = 0;
	while (iter < count)
	{
		((char *)dest)[iter] = (char)c;
		iter++;
	}
	return (dest);
}
