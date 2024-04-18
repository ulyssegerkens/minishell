
# Note for Minishell Evaluation

We use the two testers below. The `docs/Notes_for_evaluation.md` details each KO test and reasons (most of them are considered out of scope).

## Implementation Choices

### Arithmetic Expansion & nested subshell
In bash:
- `(())` are used for arithmetic expansion
- `((ls))` raised an error 
- `((ls) > out )` not 
- `(((ls) > out))` raised an error because `(ls) > out` in arithmetic.
- 
As we don't implement a.e., we cannot properly raise the error.
We could be tempted to simply check for pairs of double pa `(( ))`, but `((ls) | (cat > out ))` would fall into this filter despite being a valid command (not sure if valid bur it works in zsh).
Solution: ignore the existence of this notation and do not interpret it.
So, not as bash, our minishell interpret `((ls))` as `( (ls) )`.

### Redirections
The order of redirections matters!
Ok, but is the pre/post-fix order important? 
No because all redirections are applied before the command is executed.<br>
cf. https://www.gnu.org/software/bash/manual/html_node/Redirections.html

### Expansion: Wildcards
We decide to not implement quoting effect on wildcard after expansion. <br>
Ex: `export var="\"*\""; echo $var` will print `*` and not the list of files in the current directory. Our minishell don't manage that and simply expand the wildcard.

### Heredoc
The variable expansion on heredoc is not implemented because we consider it off-subject.

---

## Testers

### MPANIC
[github/mpanic](https://github.com/ChewyToast/mpanic) <br>
`bash mpanic.sh -b` <br>
549 OK, with some false positives for redirection with fd  <br>
14 KO, all out of subject

- ENV
  - 3 first test are not working, but seems working in manual test
- PARSING
  - `echo $'\n'; echo $'\r';echo $'\t'` as `echo $"   t  hi   t    "; ...`: use specific separator not asked in the subject, limit the minishell to the ` ` separator seems reasonable.

- "PANIC MANDATORY"
  - `exit --`: subject say "exit with no options": our implementation consider `--` as an non-numeric argument and raise an error
  - `echo all &> file`: `&>` is not in the subject
  - `echo <> file`: `<>` is a redirection not in the subject
  - `cat < file 3< file2`: `3<`: redirection with fd are not in the subject
  - `echo ~ (without $HOME in env)` out of subject


### 42_MINISHELL_TESTER
[github/42_minishell_tester](https://github.com/zstenger93/42_minishell_tester) <br>
`bash tester.sh a` <br>
✅ 2935 <br>
❌ 47, cf bellow <br>

#### "COMPARE PARSING"
#### "PARSING HELL"
- 12, 14, 15: Heredocs expansion
- 62
	```bash
	bash$ ENV > out1
	bash$ ./minishell 
	minishell$ ENV > out2
	minishell$ diff out1 out2
		24c24
		< SHLVL=1
		---
		> SHLVL=2
		36c36
		< _=/usr/bin/ENV
		---
		> _=./minishell
	```
- 118: `>| echo sure`: `>|` is as special output redirection not in the subject, so we raise a parsing error. Bash create de output file (named `echo`) and try to execute the command named `sure`. 117: `<| echo ok` success to raise a parsing error because in both case it is not a valid redirection.
- 119: `cd --`: as we implement "cd with only a relative or absolute path" our minishell interpret `--` as a relative path and raise a "No such file or directory" error. For bash is a empty option.

#### "BUILTINS"
- 101: `cd '/////' 2>/dev/null`: fd redirection is not in the subject, so `2` interpreted as a `cd` arg and "too many arguments" error is raised. `cd '/////'` works as expected.
- 104, 106: same as 101
- 139: `env what`: we raise a "too many arguments" error. Bash react differently because there is a `/usr/bin/what` in my computer.
- 149: `export`: probably because we don't sort the export list, as it not a bash feature.
- 164, 169, 184: `export -TEST=100`: bash raise a option error (exit 2), we raise a "not a valid identifier" error (exit 1) as export option are not in the subject.
- 223: `unset TES;T`: bash raise an error because of the `;`, in our minishell this character means nothing and the command is executed as expected.
- 230: `unset -TEST`: bash raise an option error because of the `-`. unset option are not in the subject.

#### "PIPELINES"
- 1 & 2, looks to work, check without fsanitize that slow down the execution
- 22: I think it fail because of the parallel execution of the commands. Bash manage to execute the `> tmp_lol.c` before the parallel execution of the first pipeline append. Our minishell do also parallel execution but not on the same rhythm as bash.
	```bash
	cat tmp_lol.c | cat > tmp_lol.c
	rm -rf tmp_lol.c
	```
  NB: another test with proper config don't fail, I leave the comment if it fail again.
- 27, 28: Heredoc expansion not implemented
- 29: same as 25, 26: manual test works to raise syntax error

#### "REDIRECTIONS"
- 84, 86: Heredoc expansion not implemented

#### "SCMD"
- 7: don't work because we don't implement unclosed prompt line, but work by directly paste the command in the terminal.

#### "CORRECTION"
- 81: `env | grep $USER` fail sometime because launch in a different environment, for example with a OLDPWD set.

#### "WILD"
Dom't have the energy to understand this shit xp
- 10, 13, 14, 15:
- 16: work work with manual test

#### BONUS: "WILDCARDS"
- 3 works in manual test and when only bonus are tested
