/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoumni <smoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 15:30:20 by ymabsout          #+#    #+#             */
/*   Updated: 2024/03/08 01:47:46 by smoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

//ls && (ls) ls handle cmd after subshell
// ls && >>f
//>_:hello"test"yy'tt' world

void f ()
{
    system("leaks minishell");
}

void *parsing(char *input)
{
    char *cmd;
    t_list *saved_list;
    t_list *cleared_list;
    t_btree *rootoftree;

    if (!input[0])
        return (NULL);
    cmd = ft_strdup(input);
    saved_list = tokenize_lex(cmd);
    if (!saved_list)
        return (NULL);
    cleared_list = repair_list(saved_list);
    if (!cleared_list)
        return (lst_clear(&saved_list), NULL);
    lst_clear(&saved_list);
    saved_list = cleared_list;
    rootoftree = parse_ampersand_or(&cleared_list);
    if (!rootoftree)
        return (lst_clear(&cleared_list), NULL);
    // print_tree(rootoftree);
    lst_clear (&saved_list);
    return (rootoftree);
}
 // syntax error should be exit_status 258


void dl(void *content)
{
    free((char *)content);
}

void lol()
{
    system("leaks minishell");
}

int main (int ac, char *av[], char **env)
{
    char *input;
    char *keep;
    t_btree *exec_tree;
    t_listt *root_env;
    s_lol s;
    (void)av;
    // testing leaks
    // atexit(lol);

    if (ac != 1)
        return (printf("error arguments\n"), 0);
    root_env = create_envs(env);
    s.status_code = 0;
    input = NULL;   
    while (1)
    {
        handle_signal();
        input = readline(">_: ");
        if (received_signal == 2)
            (received_signal = 0, s.status_code = 1);
        if (!input)
            return (printf("exit\n"), ft_lstclear(&root_env, dl), s.status_code);
        keep = ft_strtrim(input, " ");
        if (keep && keep[0])
        {
            exec_tree = (t_btree *)parsing(keep);
            if (!exec_tree && input[0] != '\0')
                (printf("Parsing Error\n"), s.status_code = 258);
            if (exec_tree)
            {
                return_def();
                executing(exec_tree, root_env, &s);
                free_tree(exec_tree);
                sig_def();
                if (waitpid(s.pids, &s.status_code, 0) != -1)
                {
                    if (WIFSIGNALED(s.status_code))
                    {
                        s.status_code = WTERMSIG(s.status_code) + 128;
                        if (s.status_code == 131)
                            printf("Quit: %d\n", WTERMSIG(s.status_code));
                        else if (s.status_code == 130)
                            printf("\n");
                    }
                    else
                        s.status_code = WEXITSTATUS(s.status_code);
                }
                while (waitpid(-1, 0, 0) != -1);
                printf("StatusCode: [%d]\n", s.status_code);
            }
            // Handle cat | cat | ls last child waiting on him!
        }
        add_history(input);
        free(input);
        free(keep);
    }
}
/// $(pwd)
