

#ifndef MINI_SHELL_H
# define MINI_SHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <termios.h>

typedef struct s_list
{
    char *content;
    int typeofcontent;
    char **joined_content;
    struct s_list *next;
    struct s_list *previous;
    struct s_list *up;
    struct s_list *down;
}   t_list;

typedef struct s_tree {
    struct s_tree *left;
    struct s_tree *right;
    int typeofcontent;
    char **string;
    int flag_subshell;
    struct s_tree *next;
    struct s_tree *down;
    char *item;
} t_btree;

typedef struct s_listt
{
    struct s_listt *next;
    void *content;
} t_listt;



enum token_type
{
    token_ampersand = 1 << 0,
    token_or = 1 << 1,
    token_red_out_trunc = 1 << 2,
    token_red_out_append = 1 << 3,
    token_red_here_doc = 1 << 4,
    token_red_input = 1 << 5,
    token_pipe = 1 << 6,
    token_word = 1 << 7,
    token_double_q = 1 << 8,
    token_single_q = 1 << 9,
    token_space = 1 << 10,
    token_par_in = 1 << 11,
    token_par_out = 1 << 12,
    token_heredoc_append = (token_red_out_append | token_red_here_doc),
    token_red = (token_red_out_trunc | token_heredoc_append | token_red_input),
    token_quote = (token_double_q | token_single_q),
    token_and_or = (token_ampersand | token_or),
    token_af_pipe = (token_pipe | token_and_or),
    token_meta = (token_pipe | token_red | token_and_or),
    token_pth = (token_par_in | token_par_out)
};

char	**ft_split(char const *s, char c);
void	fill(char *arr, char **s, char c);
char	**freearr(int index, char **arr);
int     count_words(char *s, char c);
int     len_world(char *s, char c);
char	*ft_strdup(char *s);
int     ft_strlen(const char *s);
char	*ft_substr(char *s, unsigned int start, size_t len);
char	*ft_strchr(char *s, int c);
char	*ft_strtrim(char *s1, char *set);
char	*ft_strrchr(char *s, int c);
int     ft_strncmp(char *s1, char *s2, size_t n);
void	lst_addback(t_list **lst, t_list *new);
t_list  *lst_new(char *content);
char	*ft_strjoin(char *s1, char *s2);
void    *parsing(char *input);
void    *tokenize_lex(char *cmd);
t_list  *duplicate_node(t_list *root);
void    deletenode(t_list *holder, t_list *node_delete);
t_list *lst_last(t_list *root);
void	lst_add_down(t_list **lst, t_list *new);
void	*ft_memset(void *s, int c, size_t n);
void    print_tree(t_btree *root);
t_btree *parse_cmd(t_list **root);
t_btree *duplicate_for_tree(t_list *root);
t_btree *parse_pipe(t_list **root);
t_btree *parse_heredoc_append(t_list **root);
t_btree *parse_ampersand_or(t_list **root);



void	ft_putstr_fd(char *s, int fd);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
char	*ft_itoa(int n);
int	ft_isalnum(int c);
void	ft_lstadd_back(t_listt **lst, t_listt *new);
void	ft_lstadd_front(t_listt **lst, t_listt *new);
void	ft_lstclear(t_listt **lst, void (*del)(void *));
void	ft_lstdelone(t_listt *lst, void (*del)(void *));
void	ft_lstiter(t_listt *lst, void (*f)(void *));
t_listt	*ft_lstlast(t_listt *lst);
t_listt	*ft_lstmap(t_listt *lst, void *(*f)(void *), void (*del)(void *));
t_listt	*ft_lstnew(void *content);
int	ft_lstsize(t_listt *lst);
char	**ft_split(char const *s, char c);
int	ft_isalpha(int c);
int	ft_atoi(const char *str);

void    syntax_error(void);
void    err_handler(int num);

char    *expand_env_vars(char *str, int state_code);
void handle_unclosed_quotes(char *line, int *state);
int find_num_char(char *str, int c);
void free_double(char **arr);
void valid_id_error(char *cmd, char *id);

t_listt *create_envs(char **env);

// BUILT-INS
int export_built_in(char **cmd, t_listt **head_env);
int env_built_in(char **cmd, t_listt *head_env);
int unset_built_in(char **cmd, t_listt **head_env);
int exit_built_in(char **cmd, int status_code);
int pwd_built_in(char **cmd);
int echo_built_in(char **cmd);
int cd_built_in(char **cmd, t_listt **head_env);

#endif 