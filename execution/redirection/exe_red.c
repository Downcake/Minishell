#include "../../mini_shell.h"

static void setting_streams_out(t_btree *exec_tree, int above_st , int target_st, int fd)
{
    exec_tree->stdin = above_st;
    if (target_st != 1)
        exec_tree->stdout = target_st;
    else
        exec_tree->stdout = fd;
}
static void setting_streams_in(t_btree *exec_tree, int above_st , int target_st, int fd)
{
    exec_tree->stdout = above_st;
    if (target_st != 0)
        exec_tree->stdin = target_st;
    else
        exec_tree->stdin = fd;
}

static void dbl_setter(t_btree *exec_tree, char **string)
{
    int i;
    char **arr;

    i = -1;
    while (string[++i]) ;

    arr = malloc(sizeof(char *) * (i));
    if (!arr)
        return ;
    
    i = -1;
    while (string[(++i) + 1])
    {
        arr[i] = ft_strdup(string[i + 1]);
        if (!arr[i])
            return (free_half_double(arr, i));
    }
    arr[i] = NULL;
    exec_tree->string = arr;
    free_double(string);
}

static void cmd_on_right(t_btree *exec_tree, t_listt *env,  s_lol *s, int flag)
{
    int fd;
    char *file;

    fd = open_file(exec_tree->right, flag, s->status_code, env);
    if (fd < 0)
        return (fd_failure(exec_tree->right->string[0], fd, s));
    if (exec_tree->pipe_write_end && exec_tree->pipe_read_end)
    {
        exec_tree->right->pipe_write_end = exec_tree->pipe_write_end;
        exec_tree->right->pipe_read_end = exec_tree->pipe_read_end;
    }
    dbl_setter(exec_tree->right, exec_tree->right->string);
    if (flag >= 2)
        setting_streams_in(exec_tree->right, exec_tree->stdin, exec_tree->stdout, fd);
    else
        setting_streams_out(exec_tree->right, exec_tree->stdout, exec_tree->stdin, fd);
    executing(exec_tree->right, env, s);
    close(fd);
}

void execute_red(t_btree *exec_tree, t_listt *env, s_lol *s, int flag)
{
    int fd;
    char *file;

    if (exec_tree->left == NULL)
        return (cmd_on_right(exec_tree, env, s, flag));
    fd = open_file(exec_tree->right, flag, s->status_code, env);
    if (fd < 0)
        return (fd_failure(exec_tree->right->string[0], fd, s));
    if (exec_tree->pipe_write_end && exec_tree->pipe_read_end)
    {
        exec_tree->left->pipe_write_end = exec_tree->pipe_write_end;
        exec_tree->left->pipe_read_end = exec_tree->pipe_read_end;
    }
    file = exec_tree->right->string[0];
    free(file);
    exec_tree->left->string = ft_join_all_nexts(exec_tree->left, \
        s->status_code, env);
    create_string(exec_tree->left, exec_tree->right->string + 1);
    (free(exec_tree->right->string), exec_tree->right->string = NULL);
    if (flag >= 2)
        setting_streams_in(exec_tree->left, exec_tree->stdout, exec_tree->stdin, fd);
    else
        setting_streams_out(exec_tree->left, exec_tree->stdin, exec_tree->stdout, fd);
    (executing(exec_tree->left, env, s), close(fd));
}
