/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymabsout <ymabsout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 09:43:08 by ymabsout          #+#    #+#             */
/*   Updated: 2024/02/05 17:28:37 by ymabsout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "m1.h"

t_list *lst_new(char *content)
{
	t_list *node;

	node = malloc(sizeof (t_list));
	if (!node)
		return (NULL);
	node->content = ft_strdup((char *)content);
	// free(content);
	node->next = NULL;
	return (node);
}

void	lst_addback(t_list **lst, t_list *new)
{
	t_list	*head;

	if (!lst || !new)
		return ;
	if (!(*lst))
	{
		(*lst) = new;
		new->next = NULL;
		return ;
	}
	head = (*lst);
	while (head->next != NULL)
		head = head->next;
	head->next = new;
}
