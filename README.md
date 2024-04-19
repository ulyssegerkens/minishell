
<h1 align="center">
	An Advanced Minishell
</h1>
<p align="center">
	This project helped you? Give it a 🌟!
</p>

## 🐚 General information
Version: 6. Bonus included. <br>
Compliant with the new rules in version 6.1. about using global variables only for signals.<br>
Built with the latest bash version: [GNU bash, version 5.2.21(1)-release (x86_64-apple-darwin19.6.0)](https://www.gnu.org/software/bash/).<br>
Made by [Florent](https://github.com/SarKaZm19) & [Ulysse](https://github.com/ulyssegerkens)<br>

## 🚀 Set-up
To keep the GitHub repository tidy and to avoid displaying library code, the lib directory is archived. Follow the steps below to set up and run the project:
1. Decompress the Libraries: `tar xzf lib.tar.gz && rm -rf lib.tar.gz`<br>
    If needed, to re-archive: `tar czf lib.tar.gz lib/ && rm -rf lib/`
2. Install readline library via [homebrew](https://github.com/kube/42homebrew): `brew install readline`
3. Compile the Project: `make`
4. Run the Project: `./minishell`

## ✨ What makes this minishell special?
- *Fidelity*: built directly on the bash manual and some POSIX standards.
- *Robustness*: the [Recursive descent parser](https://en.wikipedia.org/wiki/Recursive_descent_parser) allows to handle complex commands.
- *Extensibility*: thanks to the Abstract Syntax Tree execution, it is easy to add new features.
- *Performance*: to avoid unnecessary search, binaries paths are cached in memory (see `/src/get_bin_path.c`). We also use recursion for parsing and execution.
- *Security*: robust design, all system calls checked.
- *Documentation*: parts of the code is documented with doxygen style. The `/docs/` directory include EBNF-like grammar, adapted bash manual and some other resources.
- *Debugging*: tokens from the lexer and ast nodes from the parser can be printed in `json` format (see `/src/debug.c`) and displayed [here](https://vanya.jp.net/vtree/).
- *Bonuses*: 
  - Parenthesis are implemented as subshells for easier testing.
  - Wildcard are working for simple subdirectories (e.g. `ls src/*`). 

## ⚠️ Limitations
- Newline and semicolon separators are not implemented; therefore, only one command per input line is supported. New lines in user input can lead to undefined behavior.
- Known bugs:
  - `(>out)`: return a syntax error but shouldn't.
  - `ls random_dir/*`: display well the error but also exit the minishell.

## 📚 Documentation & Resources
- `resources.md`
- `bash_manual_for_minishell.md`
- `grammar.md`: EBNF-like grammar
- `notes_for_evaluation.md`: Implementation Choices + KO tests and reasons
- `subject.md` + `subject_prompt_for_llm.md`

## 🚴‍♂️ Advices to succeed Minishell project
- Learn what bash, a POSIX shell, and a terminal are, and understand the differences between these concepts.
- Read the entire bash manual (it's shorter than it looks) or our truncated version in `/docs/bash_manual_for_minishell.md`.
- "bash manual > bash testing". Of course I highly recommend to test bash to understand it, but often taking a test instead of using the manual as a reference can be misleading.
- Use AST, it's a bit more complicated at first glance, but it will make your life much easier later on.
- USE TESTERS !!!

## 📋 Testing
We use the two testers below. The `docs/Notes_for_evaluation.md` details each KO test and reasons (most of them are considered out of scope).
- [**`/mpanic`**](https://github.com/ChewyToast/mpanic) : [549 OK]
- [**`/42_minishell_tester`**](https://github.com/zstenger93/42_minishell_tester) : [2935 OK]
