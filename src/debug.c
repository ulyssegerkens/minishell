#include "minishell.h"

// ----- MAKEFILE ALT ----- //
// # Makefile config 42 computer:
// # # Makefile config M1 computer:
// # READLINE_DIR	=	/opt/homebrew/opt/readline

// ----- TESTER PROMPT SETTING ----- //
	// Set up for 42_minishell_tester, should be disable for mpanic test
    // To include in prompt_listener function
	// if (!(isatty(fileno(stdin)))) {
	// 	char buffer[1024];
	// 	char *line = fgets(buffer, 1024, stdin);
	// 	if (line != NULL) {
	// 		user_input = ft_strtrim(line, "\n");
	// 		}
	// 	return user_input;
	// }

// ----- DEBUG HEADER ----- //
void			print_tab(char **str);
void			print_tokens(t_list *tokens);
void			print_ast(t_ast *ast);
void			write_ast_to_file(t_ast *root, const char *filename);
const char		*token_type_to_string(t_token_type type);

// ----- JSON PRINT FOR TOKEN AND AST ----- //
// The code bellow is written by ChatGPT
// JSON can be displayed with https://vanya.jp.net/vtree/

// ----------- EXPANDER -----------
void	print_tab(char **str)
{
	int i = 0;
	while (str[i])
	{
		printf("str[%d] = %s\n", i, str[i]);
		i++;
	}
}

// ----------- PRINT TOKEN LIST -----------
// display with https://vanya.jp.net/vtree/
const char *token_type_to_string(t_token_type type) {
    switch (type) {
        case TK_WORD: return "TK_WORD";
        case TK_PIPE: return "TK_PIPE";
        case TK_AND: return "TK_AND";
        case TK_OR: return "TK_OR";
        case TK_REDIRECT_IN: return "TK_REDIRECT_IN";
		// case TK_DUP_IN: return "TK_DUP_IN";
        case TK_REDIRECT_OUT: return "TK_REDIRECT_OUT";
		// case TK_DUP_OUT: return "TK_DUP_OUT";
        case TK_APPEND_OUT: return "TK_APPEND_OUT";
		// case TK_DUP_APPEND_OUT: return "TK_DUP_APPEND_OUT";
        case TK_HEREDOC: return "TK_HEREDOC";
		case TK_SUBSHELL_OPEN: return "TK_SUBSHELL_OPEN";
		case TK_SUBSHELL_CLOSE: return "TK_SUBSHELL_CLOSE";
        default: return "UNKNOWN";
    }
}

void escape_double_quotes(const char *input, char *output) {
    while (*input) {
        if (*input == '\"') {
            *output++ = '\\'; // Add backslash before double quote
        }
        *output++ = *input++; // Copy the original character
    }
    *output = '\0'; // Null-terminate the output string
}

void print_tokens(t_list *tokens) {
    FILE *fp = fopen("tokens.json", "w");
    if (!fp) {
        perror("Error opening file");
        return;
    }

    fprintf(fp, "[\n");
    for (t_list *current = tokens; current != NULL; current = current->next) {
        t_token *token = (t_token *)current->content;
        fprintf(fp, "  {\n");
        fprintf(fp, "    \"type\": \"%s\",\n", token_type_to_string(token->type));
        if (token->type == TK_WORD) {
            // Allocate enough space to escape all double quotes (worst case: every character is a double quote)
            char *escapedValue = malloc(strlen(token->value) * 2 + 1);
            escape_double_quotes(token->value, escapedValue);
            fprintf(fp, "    \"value\": \"%s\"\n", escapedValue);
            free(escapedValue);
        } else {
            fprintf(fp, "    \"value\": \"%s\"\n", token->value);
        }
        fprintf(fp, "  }%s\n", current->next ? "," : "");
    }
    fprintf(fp, "]\n");

    fclose(fp);
}


// ----------- PRINT AST.JSON -----------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ast_to_json(t_ast *node, char **json_str);

void escape_and_append(char **json_str, const char *str) {
    while (*str) {
        if (*str == '\"') {
            strcat(*json_str, "\\\"");
        } else {
            size_t len = strlen(*json_str);
            (*json_str)[len] = *str;
            (*json_str)[len + 1] = '\0';
        }
        str++;
    }
}

void command_to_json(t_ast_command *command, char **json_str) {
    strcat(*json_str, "{\"type\": \"command\", \"args\": [");
    for (int i = 0; command->args[i] != NULL; i++) {
        if (i > 0) strcat(*json_str, ", ");
        strcat(*json_str, "\"");
        escape_and_append(json_str, command->args[i]);
        strcat(*json_str, "\"");
    }
    strcat(*json_str, "]}");
}

void pipeline_to_json(t_ast_pipeline *pipeline, char **json_str) {
    strcat(*json_str, "{\"type\": \"pipeline\", \"left\": ");
    ast_to_json(pipeline->left, json_str);
    strcat(*json_str, ", \"right\": ");
    ast_to_json(pipeline->right, json_str);
    strcat(*json_str, "}");
}

void subshell_to_json(t_ast_subshell *subshell, char **json_str) {
	strcat(*json_str, "{\"type\": \"subshell\", \"child\": ");
	ast_to_json(subshell->child, json_str);
	strcat(*json_str, "}");
}

void logical_to_json(t_ast_logical *logical, char **json_str) {
    strcat(*json_str, "{\"type\": \"logical\", \"operator\": \"");
    strcat(*json_str, (logical->operator == TK_AND) ? "&&" : "||");
    strcat(*json_str, "\", \"left\": ");
    ast_to_json(logical->left, json_str);
    strcat(*json_str, ", \"right\": ");
    ast_to_json(logical->right, json_str);
    strcat(*json_str, "}");
}

void redirection_to_json(t_ast_redirection *redirection, char **json_str) {
    strcat(*json_str, "{\"type\": \"redirection\", \"direction\": \"");
    switch (redirection->direction) {
        case TK_REDIRECT_IN:
            strcat(*json_str, "<");
            break;
        case TK_REDIRECT_OUT:
            strcat(*json_str, ">");
            break;
        case TK_APPEND_OUT:
            strcat(*json_str, ">>");
            break;
        case TK_HEREDOC:
            strcat(*json_str, "<<");
            break;
        default:
            strcat(*json_str, "unknown");
    }
    strcat(*json_str, "\", \"file\": \"");
    strcat(*json_str, redirection->file);
    strcat(*json_str, "\", \"child\": ");
    if (redirection->child) {
        ast_to_json(redirection->child, json_str);
    } else {
        strcat(*json_str, "\"NULL\"");
    }
    strcat(*json_str, "}");
}


void ast_to_json(t_ast *node, char **json_str) {
    if (!node)
		return;

    switch (node->type) {
        case AST_COMMAND:
            command_to_json(&node->u_data.command, json_str);
            break;
        case AST_PIPELINE:
            pipeline_to_json(&node->u_data.pipeline, json_str);
            break;
        case AST_LOGICAL:
            logical_to_json(&node->u_data.logical, json_str);
            break;
		case AST_SUBSHELL:
			subshell_to_json(&node->u_data.subshell, json_str);
			break;
		case AST_REDIRECTION:
            redirection_to_json(&node->u_data.redirection, json_str);
            break;
		default:
			break;
		// Add cases for other types if necessary
    }
}

void write_ast_to_file(t_ast *root, const char *filename) 
{
    char *json_str = malloc(10000); // Adjust size as necessary
    if (!json_str) {
        perror("Failed to allocate memory for JSON string");
        return;
    }
    json_str[0] = '\0'; // Initialize the string

    ast_to_json(root, &json_str);

    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        free(json_str);
        return;
    }

    fprintf(file, "%s", json_str);
    fclose(file);
    free(json_str);
}
