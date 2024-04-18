# Subject for ChatGPT
Abstract version of the assignment to prompt LLM.
---
Here is my next project for the 42 school. You will help me to understand the theoretical concepts to achieve this project, in the most pedagogical way. You will also implement some parts of the project with a clean and robust code style.

# Minishell Project
- **Program name:** minishell
- **Turn in files:** Makefile, *.h, *.c
- **External functions:** readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history, printf, malloc, free, write, access, open, read, close, fork, wait, waitpid, wait3, wait4, signal, sigaction, sigemptyset, sigaddset, kill, exit, getcwd, chdir, stat, lstat, fstat, unlink, execve, dup, dup2, pipe, opendir, readdir, closedir, strerror, perror, isatty, ttyname, ttyslot, ioctl, getenv, tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
- **Libft functions:** atoi, itoa, tolower, toupper, lstadd_back, lstadd_front, lstclear, lstdelone, lstiter, lstlast, lstmap, lstnew, lstsize, bzero, calloc, memchr, memcmp, memcpy, memmove, memset, putchar_fd, putendl_fd, putnbr_fd, putstr_fd, write_fd, isalnum, isalpha, isascii, isdigit, isprint, split, strchr, strcmp, strdup, striteri, strjoin, strlcat, strlcpy, strlen, strmapi, strncmp, strnstr, strrchr, strtrim, substr

**Description:**
The shell should:
- Display a prompt when waiting for a new command.
- Search and launch the right executable (based on the PATH variable or using a relative or an absolute path).
- Not interpret unclosed quotes or special characters which are not required by the subject such as `;` (semicolon).
- Handle single quote and double quote.
- Implement the following redirections: `<`, `>`, `<<`, `>>`.
- Implement pipes: `|`.
- `&&` and `||` with parenthesis for priorities.
- Handle environment variables (`$` followed by a sequence of characters) which should expand to their values.
- Handle `$?` which should expand to the exit status of the most recently executed foreground pipeline.
- Wildcards `*` should work for the current working directory.
- Handle `ctrl-C`, `ctrl-D` and `ctrl-\` which should behave like in bash. In interactive mode: `ctrl-C` displays a new prompt on a new line; `ctrl-D` exits the shell; `ctrl-\` does nothing.
- Implement the following builtins: `echo` with option `-n`; `cd` with only a relative or absolute path; `pwd`; `export`; `unset`; `env`; `exit`.

You should limit yourself to the subject description.  Anything that is not asked is not required. If you have any doubt about a requirement, take bash as a reference.
