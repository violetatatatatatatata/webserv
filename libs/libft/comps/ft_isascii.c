/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:41:31 by avelandr          #+#    #+#             */
/*   Updated: 2025/12/18 16:41:48 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

int	ft_isascii(int c)
{
	if ((sizeof(c) == sizeof(unsigned char)) || (c >= 0 && c <= 127))
	{
		return (1);
	}
	return (0);
}
