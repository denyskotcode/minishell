<div align="center">

# minishell

**A minimal POSIX-compliant shell written in C**

*42 School project — Prague campus*

![Language](https://img.shields.io/badge/Language-C-blue?style=flat-square)
![Norm](https://img.shields.io/badge/Norminette-passing-brightgreen?style=flat-square)
![School](https://img.shields.io/badge/School-42_Prague-black?style=flat-square)

</div>

---

## About

**minishell** is a fully functional Unix shell implemented from scratch in C as part of the 42 School curriculum. It reproduces the core behavior of **bash**, including command execution, pipelines, I/O redirections, environment variable expansion, heredoc, signal handling, and a complete set of built-in commands.

The project was built with a strong emphasis on clean architecture, POSIX compliance, and robust error handling — with zero memory leaks and strict adherence to the 42 Norm coding standard.

> Built by **dkot** and **marksylaiev** — [42 Prague](https://www.42prague.com/)

---

## Features

### Shell Behavior
- Interactive prompt with **command history** (via GNU Readline)
- Non-interactive (script) mode
- Proper **exit status propagation** (`$?`)
- **Signal handling**: `Ctrl+C` redisplays prompt, `Ctrl+D` exits cleanly, `Ctrl+\` is ignored in the main process

### Parsing Pipeline
The input goes through a 3-stage pipeline before execution:

```
Input string
    │
    ▼
┌─────────┐    tokens     ┌─────────┐    groups     ┌───────────┐
│  Lexer  │ ────────────► │ Parser  │ ────────────► │ Executor  │
└─────────┘               └─────────┘               └───────────┘
```

| Stage    | Responsibility |
|----------|---------------|
| **Lexer**    | Tokenizes raw input, validates quote pairing and pipe syntax |
| **Parser**   | Resolves token types, expands environment variables, groups commands by pipe boundaries |
| **Executor** | Handles single commands, pipelines, redirections, and heredocs |

### Command Execution
- **Single commands** executed directly in the current process (for builtins) or via `fork/execve`
- **Pipelines** of arbitrary length using `pipe(2)`, with proper fd management and `waitpid`
- Full **PATH resolution** for external commands
- **I/O Redirections**:
  - `<`  — redirect stdin from file
  - `>`  — redirect stdout to file (truncate)
  - `>>` — redirect stdout to file (append)
  - `<<` — heredoc: read lines until a delimiter is matched

### Variable Expansion
- `$VARIABLE` — expands to the value of the environment variable
- `$?` — expands to the exit status of the last command
- Expansion is performed inside **double quotes** but suppressed in **single quotes**

### Built-in Commands

| Command | Description |
|---------|-------------|
| `echo [-n]` | Print arguments to stdout; `-n` suppresses the trailing newline |
| `cd [path]` | Change current directory; updates `PWD` and `OLDPWD` |
| `pwd` | Print the current working directory |
| `export [name=value]` | Set or display environment variables |
| `unset [name]` | Remove an environment variable |
| `env` | Print all environment variables |
| `exit [code]` | Exit the shell with an optional status code |

---

## Architecture

```
minishell/
├── Makefile
├── libs/
│   └── libft/               # Custom C standard library (ft_*)
└── src/
    ├── init/
    │   ├── main.c           # Entry point, main loop, input dispatch
    │   ├── reader.c         # Readline wrapper, signal handler
    │   ├── init_shell_env.c # Shell environment initialization (SHLVL, etc.)
    │   └── header.h         # Global types, structs, and prototypes
    ├── lexer/
    │   ├── lexer.c          # Tokenizer entry point
    │   ├── handle.c         # Token handlers for each character class
    │   ├── handle_utils.c   # Helpers for quote and word tokenization
    │   ├── syntax_validator.c      # Pipe syntax validation
    │   ├── syntax_validator_utils.c
    │   └── utils.c
    ├── parser/
    │   ├── parser.c         # Token-to-group conversion
    │   ├── expand.c         # Environment variable expansion
    │   ├── expand_utils.c
    │   ├── handlers.c       # Redirect and text token handlers
    │   ├── checker.c        # Type classification helpers
    │   ├── groups.c         # Command group construction
    │   └── utils.c
    ├── exec/
    │   ├── exec.c           # Top-level execution dispatcher
    │   ├── exec_utils.c     # Context init/cleanup
    │   ├── exec_helpers.c
    │   ├── external.c       # execve wrapper for external commands
    │   ├── path.c           # PATH resolution
    │   ├── redir.c          # Redirection setup
    │   ├── redir_utils.c
    │   ├── redir_helpers.c
    │   ├── heredoc.c        # Heredoc processing (fork + pipe)
    │   ├── heredoc_utils.c
    │   └── command_validation.c
    ├── pipe/
    │   ├── pipe.c           # Pipeline entry point
    │   ├── child.c          # Child process setup
    │   ├── exec.c           # Per-command execution inside pipeline
    │   ├── external.c       # External command in pipeline context
    │   └── setup.c          # Pipe fd setup and teardown
    ├── builtin/
    │   ├── builtin.c        # Builtin dispatch table
    │   ├── cd.c             # cd implementation
    │   ├── echo.c           # echo implementation
    │   ├── export.c         # export/unset implementation
    │   └── utils.c          # Shared builtin helpers
    ├── env/
    │   ├── env.c            # env builtin
    │   ├── init.c           # Environment array initialization
    │   └── utils.c          # get/set/unset env variable helpers
    └── utils/
        ├── utils.c          # General-purpose utilities
        └── str.c            # String manipulation helpers
```

---

## Getting Started

### Prerequisites

- GCC or Clang
- GNU Make
- GNU Readline library

**macOS (Homebrew):**
```bash
brew install readline
```

**Linux (Debian/Ubuntu):**
```bash
sudo apt-get install libreadline-dev
```

### Build

```bash
git clone git@github.com:denyskotcode/minishell.git
cd minishell
make
```

This produces the `minishell` executable in the project root.

```bash
make clean    # Remove object files
make fclean   # Remove object files and binary
make re       # Full rebuild
```

### Run

```bash
./minishell
```

You will be greeted with an interactive prompt:

```
minishell>
```

---

## Usage Examples

```bash
# Basic commands
minishell> ls -la
minishell> echo "Hello, World!"
minishell> pwd

# Environment variables
minishell> export MY_VAR=hello
minishell> echo $MY_VAR
hello
minishell> echo $?
0

# Pipelines
minishell> ls | grep ".c" | wc -l
minishell> cat /etc/passwd | grep root | cut -d: -f1

# Redirections
minishell> echo "log entry" >> output.log
minishell> sort < unsorted.txt > sorted.txt

# Heredoc
minishell> cat << EOF
> line one
> line two
> EOF

# Quotes
minishell> echo 'no $expansion here'
no $expansion here
minishell> echo "but $HOME is expanded"
but /home/denys is expanded

# Chained exit status
minishell> false; echo $?
1
minishell> true; echo $?
0
```

---

## Technical Details

### Data Structures

| Struct | Purpose |
|--------|---------|
| `t_token` | Single lexer token: type enum + string value |
| `t_group` | Command group: argv list + heredoc list |
| `t_exec_ctx` | Execution context: env, stdio backups, exit status |
| `t_pipe_data` | Pipeline state: pipe fds, pids, command index |
| `t_heredoc_data` | Heredoc: delimiter string + read fd |
| `t_info` | Global shell state: env array, exit flag, last status |

### Token Types

```c
typedef enum e_type {
    WORD,             // unquoted word
    FIELD,            // single-quoted string (no expansion)
    EXP_FIELD,        // double-quoted string (with expansion)
    SEP,              // whitespace separator
    PIPE,             // |
    REDIRECT_IN,      // <
    REDIRECT_OUT,     // >
    REDIRECT_APPEND,  // >>
    HEREDOC           // <<
} t_e_type;
```

### Memory Management

All dynamically allocated memory is tracked and freed:
- Token lists are freed after parsing (`ft_free_tokens`)
- Command groups are freed after execution (`free_groups`)
- Environment arrays are deep-copied and freed independently
- Heredoc file descriptors are closed after use

---

## Known Limitations

- No support for `&&`, `||`, or `;` as logical/sequential operators (treated as noise)
- No subshell execution `(...)` or command substitution `` $(...) ``
- No wildcard/glob expansion (`*`, `?`)
- Readline is initialized once; no multi-line commands

---

## Authors

| Name | GitHub | 42 Login |
|------|--------|----------|
| Denys Kot | [@denyskotcode](https://github.com/denyskotcode) | `dkot` |
| Mark Sylaiev | — | `marksylaiev` |

---

## License

This project is part of the 42 School curriculum. No license — educational use only.
