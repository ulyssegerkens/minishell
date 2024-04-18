
# Minishell Grammar
Here is a custom grammar for Minishell. It has been build on the [POSIX Shell Grammar](https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html#tag_02_10) and has been adapted for the subject. Unused rules and tokens have been removed. A custom notation is used to represent optional arguments on a single line (`(`, `)` and `|` ), only for single alternatives a null option is supposed.

A second grammar much different from the original rename rules and tokens to match our Minishell implementation. 

If I had to do it again, I would rather adapt [this specific grammar to bash](https://cmdse.github.io/pages/appendix/bash-grammar.html).

## Trunked POSIX Shell Grammar
```
/* Grammar symbols */
%token WORD
%token  AND_IF    OR_IF
/*      '&&'      '||' */
%token  DLESS  DGREAT
/*      '<<'   '>>'  */

/* Grammar */
%start complete_command

complete_command : (complete_command) and_or
                 ;

and_or           : pipeline
                 | and_or (AND_IF | OR_IF) pipeline
                 ;

pipeline         : (pipeline '|') command
                 ;

command          : simple_command io_redirect
                 ;

simple_command   : (cmd_prefix) cmd_name (cmd_suffix)
                 | (cmd_prefix)
                 ;

subshell         : '(' complete_command ')'

cmd_name         : WORD /* Apply rule for command name */

cmd_prefix       : (cmd_prefix) io_redirect
                 ;

cmd_suffix       : (cmd_suffix) WORD /* for arguments */
                 | (cmd_suffix) io_redirect
                 ;

io_redirect      : '<' filename
                 | '>' filename
                 | DGREAT filename /* append */
                 | DLESS  here_end /* heredoc */
                 ;

here_end         : WORD  /* Delimiter for end of heredoc */
                 ;

filename         : WORD  /* Apply rule for filename */
                 ;
```

## Custom Minishell Grammar
https://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_form#Table_of_symbols
repetition  { ... }  none or more

```ebnf
sequence         = pipeline, {"&&" | "|", pipeline}
                 ;

pipeline         = executable, {"|" executable}
                 ;

executable       = {redirect}, command, {redirect}
                 | {redirect}, subshell, {redirect}
				 | redirect, {redirect}
                 ;

subshell         = "(", sequence, ")"
                 ;

command          = WORD, {redirect}, {arguments}
                 ;

arguments        = WORD
				 ;

redirect      	= "<" | ">" | ">>", filename
                 | "<<", here_end
                 ;

here_end         = WORD
                 ;

filename         = WORD
                 ;
```