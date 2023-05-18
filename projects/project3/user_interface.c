/* Name: Ethan Hunt  UID: 117171051  DirID: ehunt124 */
#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include <stdlib.h>
#include <ctype.h>
#include "document.h"

#define MAX_LINE_SIZE 1024

/* Checks if the string contains only %d and then converts if applicable */
int atoi_checker(char* input); 
/* Moves the pointer to after the space */
void skip_space(char **input);
/* Copies the string between the quotes and skips leading and trailing space */
void string_between_quote(char **input, char **output);
/* Copies the next string and skips leading and trailing space */
void string_between_space(char **input, char **output);
/* Checks if command is valid */
int is_valid_command(Document *doc, char *line);
/* Checks if line is valid (command, comment, empty) */
int valid_line(Document *doc, char *line);
/* Runs the command line, printing > when needed and exiting on special value */
int command_line(Document *doc, char *line, int show_prompt);


int atoi_checker(char* input) {
    int i = 0;
    while(input[i] != '\0') {
        if(!isdigit(input[i])) {
            return -1; /* Non-digit in input */
        }
        i++;
    }
    return atoi(input); /* Returns int from String */
}

void skip_space(char **input) {
    while(isspace(**input)) {
        (*input)++;
    }
}

void string_between_quote(char **input, char **output) {
    int i = 0;
    char return_string[MAX_LINE_SIZE + 1] = {0};
    skip_space(input); /* Skips spaces before if there is any */
    if(**input == '\"') {
        (*input)++;
    }
    while(**input != '\"') {
        return_string[i] = **input;
        (*input)++;
        i++;
    }
    if(**input == '\"') {
        (*input)++;
    }
    if(i == 0) {
        *output = NULL;
    } else {
        strcpy(*output, return_string);
    }
    skip_space(input); /* Skips spaces after if there is any */
}

void string_between_space(char **input, char **output) {
    int i = 0;
    skip_space(input); /* Skips spaces before if there is any */
    sscanf(*input, "%s", *output);
    while(!isspace(**input)) {
        (*input)++;
        i++;
    }
    if(i == 0) {
        *output = NULL;
    }
    skip_space(input); /* Skips spaces after if there is any */
}

int is_valid_command(Document *doc, char *line) {
    int p_num, l_num;
    char c_string[MAX_LINE_SIZE + 1] = {0}, p1_string[MAX_LINE_SIZE + 1] = {0},
        p2_string[MAX_LINE_SIZE + 1] = {0};
    char *command = c_string, *param_1 = p1_string, *param_2 = p2_string;
    char **command_ptr = &command, **param_1_ptr = &param_1,
        **param_2_ptr = &param_2, **line_pos = &line;
    string_between_space(line_pos, command_ptr);
    if(strcmp(command, "add_paragraph_after") == 0) {
        string_between_space(line_pos, param_1_ptr);
        if(param_1 == NULL || strcmp(*line_pos, "") != 0) {
            /* No PARAGRAPH_NUMBER provided 
             * or data provided after PARAGRAPH_NUMBER */
            return 0;
        }
        p_num = atoi_checker(param_1);
        if(p_num < 0) {
            return 0; /* Includes -1 case from nondigit in input */
        }
        if(strcmp(*line_pos, "") != 0) {
            return 0; /* Data provided after */
        }
        if(add_paragraph_after(doc, p_num) == FAILURE) {
            printf("add_paragraph_after failed\n");
        }
        return 1;
    } else if(strcmp(command, "add_line_after") == 0) {
        string_between_space(line_pos, param_1_ptr);
        if(param_1 == NULL) {
            /* No PARAGRAPH_NUMBER provided */
            return 0;
        }
        p_num = atoi_checker(param_1);
        if(p_num < 1) {
            return 0; /* Includes -1 case from nondigit in input */
        }
        string_between_space(line_pos, param_2_ptr);
        if(param_2 == NULL) {
            /* No LINE_NUMBER provided */
            return 0;
        }
        l_num = atoi_checker(param_2);
        if(l_num < 0) {
            return 0; /* Includes -1 case from nondigit in input */
        }
        if(**line_pos != '*') {
            return 0;
        } else {
            (*line_pos)++; /* Moving past the * so we only get the input */
        }
        if(line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
            /* Removing the \n from the end of the line for simplicity */
        }
        if(add_line_after(doc, p_num, l_num, *line_pos) == FAILURE) {
            printf("add_line_after failed\n");
        }
        return 1;
    } else if(strcmp(command, "print_document") == 0) {
        if(strcmp(*line_pos, "") != 0) {
            return 0;
        }
        if(print_document(doc) == FAILURE) {
            printf("print_document failed\n");
        }
        return 1;
    } else if(strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0) {
        if(strcmp(*line_pos, "") != 0) {
            return 0;
        }
        return 2; /* Special value to see when quit is entered */
    } else if(strcmp(command, "append_line") == 0) {
        string_between_space(line_pos, param_1_ptr);
        if(param_1 == NULL) {
            /* No PARAGRAPH_NUMBER provided */
            return 0;
        }
        p_num = atoi_checker(param_1);
        if(p_num < 1) {
            return 0; /* Includes -1 case from nondigit in input */
        }
        if(**line_pos != '*') {
            return 0;
        } else {
            (*line_pos)++; /* Moving past the * so we only get the input */
        }
        if(line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
            /* Removing the \n from the end of the line for simplicity */
        }
        if(append_line(doc, p_num, *line_pos) == FAILURE) {
            printf("append_line failed\n");
        }
        return 1;
    } else if(strcmp(command, "remove_line") == 0) {
        string_between_space(line_pos, param_1_ptr);
        if(param_1 == NULL) {
            /* No PARAGRAPH_NUMBER provided */
            return 0;
        }
        p_num = atoi_checker(param_1);
        if(p_num < 1) {
            return 0; /* Includes -1 case from nondigit in input */
        }
        string_between_space(line_pos, param_2_ptr);
        if(param_2 == NULL) {
            /* No LINE_NUMBER provided */
            return 0;
        }
        l_num = atoi_checker(param_2);
        if(l_num < 0) {
            return 0; /* Includes -1 case from nondigit in input */
        }
        if(strcmp(*line_pos, "") != 0) {
            return 0;
        }
        if(remove_line(doc, p_num, l_num) == FAILURE) {
            printf("remove_line failed\n");
        }
        return 1;
    } else if(strcmp(command, "load_file") == 0) {
        string_between_space(line_pos, param_1_ptr);
        if(param_1 == NULL || strcmp(*line_pos, "") != 0) {
            /* No FILENAME provided or data provided after */
            return 0;
        }
        if(load_file(doc, param_1) == FAILURE) {
            printf("load_file failed\n");
        }
        return 1;
    } else if(strcmp(command, "replace_text") == 0) {
        string_between_quote(line_pos, param_1_ptr);
        if(param_1 == NULL || strcmp(*line_pos, "") == 0) {
            /* No "TARGET" provided */
            return 0;
        }
        string_between_quote(line_pos, param_2_ptr);
        if(param_2 == NULL) {
            /* No "REPLACEMENT" provided */
            return 0;
        }
        if(replace_text(doc, param_1, param_2) == FAILURE) {
            printf("replace_text failed\n");
        }
        return 1;
    } else if(strcmp(command, "highlight_text") == 0) {
        string_between_quote(line_pos, param_1_ptr);
        if(param_1 == NULL) {
            /* No "TARGET" provided */
            return 0;
        }
        if(highlight_text(doc, param_1) == FAILURE) {
            printf("highlight_text failed\n");
        }
        return 1;
    } else if(strcmp(command, "remove_text") == 0) {
        string_between_space(line_pos, param_1_ptr);
        if(param_1 == NULL) {
            /* No "TARGET" provided */
            return 0;
        }
        if(remove_text(doc, param_1) == FAILURE) {
            printf("remove_text failed\n");
        }
        return 1;
    } else if(strcmp(command, "save_document") == 0) {
        string_between_space(line_pos, param_1_ptr);
        if(param_1 == NULL || strcmp(*line_pos, "") != 0) {
            /* No FILENAME provided or data provided after */
            return 0;
        }
        if(save_document(doc, param_1) == FAILURE) {
            printf("save_document failed\n");
        }
        return 1;
    } else if(strcmp(command, "reset_document") == 0) {
        if(strcmp(*line_pos, "") != 0) {
            return 0;
        }
        reset_document(doc);
        return 1;
    }
    return 0;
}

int valid_line(Document *doc, char *line) {
    int length, i, space;
    if(line == NULL) {
        return 0;
    } 
    length = strlen(line);

    for(i = 0; i < length; i++) {
        if(isspace(line[i])) {
            space++;
        } else if(line[i] == '#') {
            return 1; /* Comment Line */
        } else {
            /* Starting where the whitespace ends */
            return is_valid_command(doc, line);
        }
    }

    if(space > 0) {
        return 1; /* Empty line */
    }
    return 0;
}

int command_line(Document *doc, char *line, int show_prompt) {
    int output = valid_line(doc, line);
    if(!output) {
        printf("Invalid Command\n");
    } else if(output == 2) {
        return 1; /* exit or quit called */
    }
    if(show_prompt) {
        printf("> ");
    }
    return 0;
}

int main(int argc, char *argv[]) {
    FILE *input;
    Document main_doc;
    char line[MAX_LINE_SIZE + 1] = {0};
    int show_prompt = 0;

    if(argc > 2) {
        fprintf(stderr, 
            "Usage: user_interface\nUsage: user_interface <file_name>");
        exit(EX_USAGE);
    } else if(argc == 1) {
        input = stdin;
        show_prompt = 1;
        printf("> ");
    } else if(argc == 2) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            fprintf(stderr, "%s cannot be opened.", argv[1]);
            exit(EX_OSERR);
        }
    }
    init_document(&main_doc, "main_document");
    
    fgets(line, MAX_LINE_SIZE, input);
    while (!feof(input)) {
        if(command_line(&main_doc, line, show_prompt)) {
            break; /* quit or exit called */
        }
        fgets(line, MAX_LINE_SIZE, input);
    }

    fclose(input);
    return 0;
}