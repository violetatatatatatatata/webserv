/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avelandr <avelandr@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:48:42 by avelandr          #+#    #+#             */
/*   Updated: 2025/12/18 16:50:01 by avelandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new_lst)
{
	t_list	*tmp;

	if (!new_lst)
		return ;
	if (*lst == NULL)
		*lst = new_lst;
	else
	{
		tmp = ft_lstlast(*(lst));
		tmp->next = new_lst;
	}
}
