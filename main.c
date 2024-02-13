/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymabsout <ymabsout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 15:30:20 by ymabsout          #+#    #+#             */
/*   Updated: 2024/02/13 21:09:56 by ymabsout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "m1.h"

int delimeter(int c)
{
    if (c == '|' || c == '>' || c == '<' || c == '\0')
        return (0);
    else
        return (1);
}

void *set_correct_type(t_list *root, int numb)
{
    if (ft_strchr(root->content, '<'))
    {
        if (numb == 2)
            root->typeofcontent = token_red_here_doc;
        else
            root->typeofcontent = token_red_input;
    }
    else if (ft_strchr(root->content, '>'))
    {
        if (numb == 2)
            root->typeofcontent = token_red_out_append;
        else
            root->typeofcontent = token_red_out_trunc;
    }
    else if (ft_strchr(root->content, '\"'))
        root->typeofcontent = token_double_q;
    else if (ft_strchr(root->content, '\''))
        root->typeofcontent = token_single_q;
    else if (ft_strchr(root->content, '|'))
        root->typeofcontent = token_pipe;
    else if (ft_strchr(root->content, ' '))
        root->typeofcontent = token_space;
    else
        root->typeofcontent = token_word;
    return (root);
}

void printdownlist(t_list *root)
{
    while (root)
    {
        printf("^^^^^down level ->{%s} --> type :{%d}^^^^\n", root->content, root->typeofcontent);
        root = root->down;
    }
}

void printlist(t_list *root, int a)
{
    t_list *holder;
    if (a == 1)
    {
        while (root)
        {
            printf("{%s} --> type :{%d}\n",  root->content, root->typeofcontent);
            holder = root;
            if (root->down)
                printdownlist(root->down);
            root = root->next;
        }
    }
    while (root)
    {
        printf("{%s} --> type :{%d}\n",  root->content, root->typeofcontent);
        root = root->next;
    }
}

int db_sl_quote(int c)
{
    if (c == '\'' || c == '\"')
        return (0);
    return (1);
}
void *get_quotes(t_list **root, char *cmd, int index)
{
    int dbl;
    int sgl;
    int saver;
    
    index = 0;
    sgl = 0;
    dbl = 0;
    (cmd[index] == '\'') && (sgl = 1);
    (cmd[index] == '\"') && (dbl = 1);
    saver = 0;
    index = 0;
    while (cmd[++index] && (ft_strchr(cmd, '\'') || ft_strchr(cmd, '\"')))
    {
            if (cmd[index] == '\'' && !dbl && sgl)
            {
                sgl = 0;
            lst_addback(root, lst_new(ft_substr(cmd, saver, index + 1)));
            lst_last(*root)->typeofcontent = token_single_q;
            cmd = ft_substr(cmd, index + 1, ft_strlen(cmd + index));
            break ;
        }
        else if (cmd[index] == '\"' && !sgl && dbl)
        {
            dbl = 0;
            lst_addback(root, lst_new(ft_substr(cmd, saver, index + 1)));
            lst_last(*root)->typeofcontent = token_double_q;
            cmd = ft_substr(cmd, index + 1, ft_strlen(cmd + index));
            break ;
        }
        else if (cmd[index] == '\"' && !dbl && !sgl)
            dbl = 1;
        else if (cmd[index] == '\'' && !sgl && !dbl)
            sgl = 1;
    }
    if (sgl || dbl)
        return(printf("Syntax Error\n"), NULL);
    return (cmd);
}

void *tokenize_lex(char *cmd)
{
    t_list *root;
    int index;
    int track;
    int savepos;

    track = 0;
    savepos = 0;
    root = NULL;
    index = -1;
    while (++index <= ft_strlen(cmd))
    {
        if (cmd[index] == ' ' || cmd[index] == '\t')
        {
            if (index != 0)
                lst_addback(&root, set_correct_type(lst_new(ft_substr(cmd, 0 , index--)), 1));
            lst_addback(&root, set_correct_type(lst_new(ft_strdup(" ")), 1));
            while (cmd[++index] && cmd[index] == ' ')
                ;
            cmd = ft_strdup(cmd + index);
            index = -1;
        }
        else if (!delimeter(cmd[index]))
        {
            if (index  != 0)
                lst_addback(&root, set_correct_type(lst_new(ft_substr(cmd, 0, index)), 1));
            if (cmd[index] != '\0')
            {
                if (!delimeter(cmd[index + 1]))
                {
                    if (cmd[index] != cmd[index + 1])
                        return(printf("Syntax Error near %c\n", cmd[index]), NULL);
                    lst_addback(&root, set_correct_type(lst_new(ft_substr(cmd, index, 2)), 2));
                    track = 1;
                }
                else
                    lst_addback(&root, set_correct_type(lst_new(ft_substr(cmd, index, 1)), 1));
            }
            if (index != ft_strlen(cmd))
            {
                cmd = ft_strdup(cmd + index + 1 + track);
                track = 0;
                index = -1;
            }
        }
        else if (!db_sl_quote(cmd[index]))
        {
            if (index != 0)
            {
                lst_addback(&root, set_correct_type(lst_new(ft_substr(cmd, 0, index)), 1));
                cmd = ft_substr(cmd ,index, ft_strlen(cmd + index));
            }
            cmd = get_quotes(&root, cmd, index);
            if (!cmd)
                return (printf("ghalat\n"), NULL);
            index = -1;
        }
    }
    return(root);
    }

t_list *repair_list(t_list *root)
{
    t_list *new_list;
    t_list *tmp;

    new_list = NULL;
    while (root)
    {
        if (root->typeofcontent & token_meta)
        {
            lst_addback(&new_list, duplicate_node(root));
            if ((!root->next || (root->next->next && root->next->next->typeofcontent & token_meta)))
                return(printf("syntax error near %s\n", root->content), NULL);
        }
        else if (root->typeofcontent & token_quote)
        {
            if (!(root->previous->typeofcontent & token_space))
            {
                lst_add_down(&new_list ,duplicate_node(root));
                puts("test");
            }
            else
                lst_addback(&new_list, duplicate_node(root));
        }
        else if (root->typeofcontent & token_word)
            lst_addback(&new_list, duplicate_node(root));
        else if (root->typeofcontent & token_space)
            ;
        else
            lst_addback(&new_list, duplicate_node(root));
        root = root->next;
    }
    puts("----------------------------------------");
    printlist(new_list, 1);
    return (new_list);
}

void *parsing(char *input)
{
    char *cmd;
    t_list *saved_list;

    cmd = ft_strtrim(ft_strdup(input), " ");
    if (!cmd)
        return (NULL);
    if (!cmd[0])
        return(cmd);
    saved_list = tokenize_lex(cmd);
    if (!saved_list)
        return (NULL);
    saved_list = repair_list(saved_list); 
    if (!saved_list)
        return (NULL);
    // printlist(saved_list);
    return (cmd);
}

int main (int ac, char *av[], char **env)
{
    char *input;
    (void)env;
    (void)av;
    if (ac != 1)
        return (printf("error arguments\n"), 0);
    while (1)
    {
        input = readline(">_:");
        if (!parsing(input))
            printf("Parsing Error\n");
        add_history(input);
    }
}