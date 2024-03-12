/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smoumni <smoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 15:30:20 by ymabsout          #+#    #+#             */
/*   Updated: 2024/03/12 14:45:38 by smoumni          ###   ########.fr       */
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
        return (lst_clear(&saved_list), NULL);
    lst_clear(&saved_list);
    return (rootoftree);
}

void get_here_doc(t_btree *exec_tree, int status, t_listt *env)
{
    if (!exec_tree)
        return ;
    get_here_doc(exec_tree->left, status, env);
    if (exec_tree->typeofcontent & token_red_here_doc)
        read_stdin(exec_tree, status, env);
    get_here_doc(exec_tree->right, status , env);
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
    struct termios term;

    if (ac != 1)
        return (printf("error arguments\n"), 0);
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag = ~ECHOCTL; // removed every signal displayed on prompt 
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
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
        while (keep && keep[0])
        {
            exec_tree = (t_btree *)parsing(keep);
            if (!exec_tree && input[0] != '\0')
                (printf("Parsing Error\n"), s.status_code = 258);
            if (exec_tree)
            {
                received_signal = 0;
                get_here_doc(exec_tree, s.status_code, root_env);
                if (received_signal == -1)
                {
                    s.status_code = 1;
                    break ;
                }
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
                // printf("StatusCode: [%d]\n", s.status_code);
            }
            received_signal = 0;
            break ;
        }
        add_history(input);
        free(input);
        free(keep);
    }
}
/// $(pwd)
