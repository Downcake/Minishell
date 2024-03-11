
#include "../mini_shell.h"

static void setting_pipe(t_btree *exec_tree, t_btree *wanted)
{
    if (exec_tree->pipe_write_end && exec_tree->pipe_read_end)
    {
        wanted->pipe_write_end = exec_tree->pipe_write_end;
        wanted->pipe_read_end = exec_tree->pipe_read_end;
    }    
}

void execute_and_op(t_btree *exec_tree, t_listt *env, s_lol *s)
{
    int in;
    int out;

    setting_pipe(exec_tree, exec_tree->left);
    (in = 0, out = 1);
    if (exec_tree->stdin != 0)
        in = dup(exec_tree->stdin);
    exec_tree->left->stdin = exec_tree->stdin;
    if (exec_tree->stdout != 1)
        out = dup(exec_tree->stdout);
    exec_tree->left->stdout = exec_tree->stdout;
    executing(exec_tree->left, env, s);
    if (waitpid(s->pids, &s->status_code, 0) != -1)
        s->status_code = WEXITSTATUS(s->status_code);
    while (waitpid(-1, 0, 0) != -1);
    if (s->status_code)
        return ;
    setting_pipe(exec_tree, exec_tree->right);
    exec_tree->right->stdin = in;
    exec_tree->right->stdout = out;
    executing(exec_tree->right, env, s);
    if (in != 0)
        close(in);
    if (out != 1)
        close(out);
}
