/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugerkens <ugerkens@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:58:32 by ugerkens          #+#    #+#             */
/*   Updated: 2024/03/27 15:12:57 by ugerkens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "structures.h"

// ----- CORE ----- //
int				main(int argc, char **argv, char **envp);
// Init shell and env
void			init_shell(t_shell *sh, char **env);
void			increment_shlvl(t_shell *sh);
// Prompt
int				interactive_shell(t_shell *sh);
void			clear_prompt(t_shell *sh);
char			*prompt_listener(t_prompt_mode mode);

// Prompt Execution
int				prompt_execution(char *input, t_shell *sh);
// Quit shell
void			quit_shell(int exit_status, t_shell *sh);
// General utils
void			add_arg_to_array(char ***char_array, char *new_arg,
					t_shell *sh);
int				count_strs(char **argv);

// ----- LEXER ----- //
int				lexer(char *input, t_list **token_list, t_shell *sh);
t_token			*create_token(t_token_type type, char *value, size_t len,
					t_shell *sh);
// Lexer: Scan next token
t_token_type	scan_next_token(char *word, size_t *len, t_shell *sh);
t_token_type	scan_tk_subshell(char *word, size_t *len);
t_token_type	scan_tk_logic_and_pipe(char *word, char c, size_t *len,
					t_shell *sh);
t_token_type	scan_tk_redirect(char *word, char c, size_t *len, t_shell *sh);
t_token_type	scan_tk_word(char *word, size_t *len, t_shell *sh);
// Lexer: Utils
int				is_space(int c);
int				is_token(int c);

// ----- PARSER ----- //
int				parser(t_list *token_list, t_ast **ast, t_shell *sh);
// Parse Links (Logical, Pipeline)
t_ast			*parse_logical(t_list **token_list, t_shell *sh);
t_ast			*parse_pipeline(t_list **token_list, t_shell *sh);
// Parse Exe (Command, Subshell)
t_ast			*parse_subshell(t_list **token_list, t_shell *sh);
t_ast			*parse_command(t_list **token_list, t_shell *sh);
// Parse Redirection
t_ast			*parse_redirection(t_list **token_list, t_shell *sh);
t_ast			*parse_redirection_list(t_list **token_list, t_ast *command,
					t_shell *sh);
// Parse Redirection utils
t_ast			*build_redirected_command(t_ast *prefix, t_ast *suffix,
					t_ast *child);
t_ast			*get_last_redirection_list(t_ast *node);
// Init AST Nodes
void			init_ast_node(t_ast **node, t_ast_type type, t_shell *sh);
// Build AST Nodes
t_ast			*create_ast_command(char **argv, t_shell *sh);
t_ast			*create_ast_subshell(t_ast *child, t_shell *sh);
t_ast			*create_ast_redirection(t_token_type direction,
					t_list *tk_filename, t_ast *child, t_shell *sh);
t_ast			*create_ast_pipeline(t_ast *left, t_ast *right, t_shell *sh);
t_ast			*create_ast_logical(t_ast *left, t_token_type operator,
					t_ast *right, t_shell *sh);
// Token utils
t_token_type	tk_type(t_list *token);
char			*tk_value(t_list *token);
bool			is_tk_type(t_list **token_list, int num_args, ...);
char			*tk_type_to_string(t_token_type type);

// Expander Node
t_ast			*expand_node(t_ast *node, t_shell *sh);
void			expand_command(t_ast *node, t_shell *sh);
void			expand_redirection(t_ast *node, t_shell *sh);
// Expand one arg
void			expand_one_arg(char *str, t_list **expanded_args, t_shell *sh);
void			no_quote_state(char *str, t_expander *exp, t_shell *sh);
void			single_quote_state(char *str, t_expander *exp);
void			double_quote_state(char *str, t_expander *exp, t_shell *sh);
// Expand Variables
void			expand_var(char *str, t_expander *exp, t_shell *sh);
void			expand_last_status(t_expander *exp, t_shell *sh);
void			expand_tilde(char *str, t_expander *exp, t_shell *sh);
// Expand Variables : from env
char			*expand_env_var(char *str, t_expander *exp, t_shell *sh);
char			*get_valid_name(char *str, t_expander *exp, t_shell *sh);
// Expander Utils
void			init_expander(t_expander *exp, char *str,
					t_list **expanded_args, t_shell *sh);
void			*add_token_to_list(t_expander *exp, t_shell *sh);
void			add_variable_to_buffer(char *value, t_expander *exp,
					t_shell *sh);
char			**convert_list_to_array(t_list **lst, t_shell *sh);
char			*word_splitting(t_expander *exp, char *value, t_shell *sh);
// Expand Filename (Wildcards)
void			filename_expansion(t_expander *exp, t_shell *sh);
t_list			*get_list_of_files(t_expander *exp, t_shell *sh);
void			lst_insert_ordered(char *to_add, t_list **files, t_shell *sh);
// Expand Filename Pattern (Wildcards)
t_list			*pattern_filter(t_list *files, t_expander *exp);
bool			pattern_match(const char *filename, const char *pattern,
					int pattern_index, t_expander *exp);
bool			not_explicit_hidden_file(const char *full_filename,
					t_expander *exp);
// Expand Filename Utils (Wildcards)
void			saved_wildcards_position(char *str, t_expander *exp,
					t_shell *sh);
bool			is_active_wildcard(int i, t_expander *exp);
bool			only_active_wildcard_left(const char *str, t_expander *exp);
char			*extract_root_path(t_expander *exp, t_shell *sh);
void			list_of_file_to_token_list(t_list *lst, t_expander *exp,
					t_shell *sh);

// ----- EXECUTION ----- //
int				execute(t_ast *node, t_execute_end end, t_shell *sh);
int				check_process_child_exit(int status, bool *new_line,
					t_shell *sh);
// Execute Subshell
int				execute_subshell(t_ast_subshell *subshell, t_shell *sh);
// Execute Logical
int				execute_logical(t_ast_logical *logical, t_shell *sh);
// Execute Redirection
int				execute_redirection(t_ast_redirection *redirection,
					t_shell *sh);
int				redirect_input(t_ast_redirection *redir, t_shell *sh);
int				redirect_output(t_ast_redirection *redir, t_shell *sh);
int				append_output(t_ast_redirection *redir, t_shell *sh);
// Execute Redirection: Heredoc
int				execute_heredocs(t_ast *node, t_shell *sh);
bool			is_heredoc(t_ast *node);
int				search_for_heredocs(t_ast *node, t_shell *sh);
char			*trim_delimiter(char *delimiter, t_shell *sh);
int				heredoc_listener(int tmp_file, char *delimiter, t_shell *sh);
// Execute Pipeline
int				execute_pipeline(t_ast *node, t_shell *sh);
t_list			*build_cmd_list(t_ast *node, t_shell *sh);
int				execute_pipeline_list(t_list *cmd_list, t_shell *sh);
pid_t			execute_one_pipeline(t_list *cmd_list, int prev_read_end,
					int p[2], t_shell *sh);
// Execute Pipeline_utils
void			setup_for_next_command(int *prev_read_end, int p[2],
					t_shell *sh);
int				wait_for_children(pid_t last_pid, int n_pipeline, t_shell *sh);

// Execute Command
int				execute_command(t_ast_command *cmd, t_execute_end end,
					t_shell *sh);
int				fork_command(t_ast_command *cmd, t_execute_end end,
					t_shell *sh);
// Execute Command: Select the builtin function
t_builtin_func	get_builtin_function(char *cmd_name);
// Execute Command: Seach binary path for execution
char			*get_bin_path(char *bin, t_shell *sh);
char			*check_preset_bin_path(char *bin_path, t_shell *sh);
char			*find_bin_path(char *bin, t_shell *sh);
char			*search_in_saved_bin_paths(char *bin, t_list *saved_bin_paths,
					t_shell *sh);
char			*find_with_paths_array(char *bin_name, char **paths,
					t_shell *sh);
// Execute Command: Seach binary path: utils
char			**build_paths_array(t_shell *sh);
bool			contain_current_dir_symbol(char *paths);
char			*join_path(char *dir, char *file, t_shell *sh);

// ----- BUILTINS ----- //
// Cd
int				cd_builtin(t_ast_command *cmd, t_shell *sh);
// Pwd
int				pwd_builtin(t_ast_command *cmd, t_shell *sh);
// Echo
int				echo_builtin(t_ast_command *cmd, t_shell *sh);
bool			option_check(char **args, int *i, char c);
// Export
int				export_builtin(t_ast_command *cmd, t_shell *sh);
void			export_one(char *assignment, bool *name_error, t_shell *sh);
void			report_name_error(char *assignment, bool *name_error,
					t_shell *sh);
bool			is_valid_name(char *name);
// Unset
int				unset_builtin(t_ast_command *cmd, t_shell *sh);
void			remove_env_var(t_list *env, t_shell *sh);
void			free_env_var(void *env);
// Env
int				env_builtin(t_ast_command *cmd, t_shell *sh);
// Exit
int				exit_builtin(t_ast_command *cmd, t_shell *sh);
bool			is_valid_64bit_signed_integer(const char *str);
long long		ft_atoi_long_long(const char *str);

// ----- ENVIRONMENT ----- //
// Init Environment
void			add_new_env_var(char *name, char *value, t_list **list,
					t_shell *sh);
t_list			*init_env(char **env, t_shell *sh);
void			edit_env_value(t_list *env_var, char *new_value, bool add,
					t_shell *sh);
// Distribute Environment
int				print_env(bool export_format, t_list *env, t_shell *sh);
char			**env_to_char_array(t_shell *sh);
// Environment Utilities
t_list			*env_var(char *name, t_list *env);
char			*name(t_list *env);
char			*value(t_list *env);

// ----- SIGNAL ----- //
// Set signal
void			set_signal_prompt(void);
void			set_signal_main_process(void);
void			set_signal_child_process(void);
void			set_signal_heredoc(void);
void			set_signal_handler(int signum, void (*handler)(int));
// Handle signal
void			sigint_prompt(int signum);
void			sigint_heredoc(int signum);

// ----- UTILS ----- //
// Error
void			error(const char *context, char *description, int exit_status,
					t_shell *sh);
void			syscall_error(const char *context, int errnum, t_shell *sh);
int				report_errno(char *context, t_shell *sh);
int				report_error(char *context, char *element, char *description,
					t_shell *sh);
// Syntax error
void			*set_syntax_error(char *unexpected_token, t_shell *sh);
int				report_syntax_error(t_shell *sh);
// Secure Facade: Memory allocation
void			*s_alloc(void *pointer, t_tracking_scope scope, t_shell *sh);
void			*calloc_s(size_t count, size_t size, t_tracking_scope scope,
					t_shell *sh);
void			track_alloc(void *pointer, t_tracking_scope scope, t_shell *sh);
// Secure Facade: List
void			lst_add_front_s(void *content, t_list **lst,
					t_tracking_scope scope, t_shell *sh);
void			lst_add_back_s(void *content, t_list **lst,
					t_tracking_scope scope, t_shell *sh);
void			check_node_alloc(t_list *new_node, void *pointer, t_shell *sh);
void			remove_list_node(t_list **node, t_list **head,
					void (*free_function)(void *), bool free_node);
// Secure Facade: Libft
char			*strjoin_s(char const *s1, char const *s2,
					t_tracking_scope scope, t_shell *sh);
char			**split_s(char const *s, char c, t_tracking_scope scope,
					t_shell *sh);
char			*strdup_s(char const *s1, t_tracking_scope scope, t_shell *sh);
ssize_t			write_s(char const *s, int fd, t_shell *sh);
// Secure Facade: Syscall File descriptor
int				open_s(const char *pathname, int flags, mode_t mode,
					t_shell *sh);
int				close_s(int fd, t_shell *sh);
int				dup2_s(int oldfd, int newfd, t_shell *sh);
int				stat_s(const char *restrict path, struct stat *restrict buf,
					t_shell *sh);
char			*getcwd_s(char *buf, size_t size, t_shell *sh);
// Secure Facade: Syscall Process
int				execve_s(const char *pathname, char *const argv[],
					char *const envp[], t_shell *sh);
pid_t			fork_s(t_shell *sh);
int				pipe_s(int pipefd[2], t_shell *sh);
pid_t			wait_s(int *wstatus, t_shell *sh);
// Secure Facade: Syscall Directory
DIR				*opendir_s(const char *name, t_shell *sh);
struct dirent	*readdir_s(DIR *dirp, t_shell *sh);
int				closedir_s(DIR *dirp, t_shell *sh);

#endif
