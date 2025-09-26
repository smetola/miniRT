/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jllamas- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 15:07:49 by jllamas-          #+#    #+#             */
/*   Updated: 2024/03/20 19:08:28 by jllamas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*result;

	if (!lst)
		return (NULL);
	result = ft_lstnew(lst->content);
	if (!result)
		return (NULL);
	result->content = f(lst->content);
	lst = lst->next;
	while (lst)
	{
		new = ft_lstnew(lst->content);
		if (!new)
		{
			ft_lstclear(&result, del);
			return (NULL);
		}
		new->content = f(lst->content);
		ft_lstadd_back(&result, new);
		lst = lst->next;
	}
	return (result);
}
