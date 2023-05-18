/* Name: Ethan Hunt  UID: 117171051  DirID: ehunt124 */
#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include <stdlib.h>
#include <ctype.h>
#include "document.h"

/* Project 3 Methods */
int load_file(Document *doc, const char *filename) {
    FILE *input;
    /* The number of possible lines containing data is 
     * (MAX_PARAGRAPH_LINES * MAX_PARAGRAPHS)
     * The number of possible lines containing "" is MAX_PARAGRAPHS
     * So, total possible lines = (MAX_PARAGRAPH_LINES + 1) * MAX_PARAGRAPHS */
    char data[(MAX_PARAGRAPH_LINES + 1) * MAX_PARAGRAPHS][MAX_STR_SIZE + 1];
    int i = 0, j = 0, length = 0;
    if(filename == NULL || doc == NULL) {
        return FAILURE;
    }
    input = fopen(filename, "r"); /* Read mode */

    if(input == NULL) {
        return FAILURE;
    }

    fgets(data[0], MAX_STR_SIZE + 1, input); 
    while(!feof(input)) {
        length = strlen(data[i]);
        if(data[i][length - 1] == '\n') {
            data[i][length - 1] = '\0'; /* Removing \n */
        }
        /* If the line is only space it is truncated to "" */
        while(isspace(data[i][j])) {
            j++;
        }
        if(j + 1 == length) {
            data[i][0] = '\0';
        }

        fgets(data[i + 1], MAX_STR_SIZE + 1, input);
        i++; 
    }
    
    fclose(input);
    return load_document(doc, data, i);
}

int save_document(Document *doc, const char *filename) {
    FILE *output;
    int i, j;
    char p_num_string[] = "  ";
    if(filename == NULL || doc == NULL) {
        return FAILURE;
    }
    output = fopen(filename, "w+"); /* Write mode, clears file */
    if(output == NULL) {
        return FAILURE;
    }
    /* Could redirect stout from print_document into file somehow probably
     * but rewriting the code is easier */
    fputs("Document name: \"", output);
    fputs(doc->name, output);
    fputs("\"\nNumber of Paragraphs: ", output);
    sprintf(p_num_string, "%d", doc->number_of_paragraphs);
    fputs(p_num_string, output);
    fputs("\n", output);
    for(i = 0; i < doc->number_of_paragraphs; i++) {
        if(i != 0) {
            fputs("\n", output);
        }
        for(j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
            fputs(doc->paragraphs[i].lines[j], output);
            fputs("\n", output);
        }
    }
    fclose(output);
    return SUCCESS; 
}

/* Project 2 Methods */

int clear_doc(Document *doc) {
    int i,  j;
    doc->number_of_paragraphs = 0;
    for (i = 0; i < MAX_PARAGRAPHS; i++) {
        doc->paragraphs[i].number_of_lines = 0;
        for(j = 0; j < MAX_PARAGRAPH_LINES; j++) {
            strcpy(doc->paragraphs[i].lines[j], "");
        }
    }
    return SUCCESS;
}

int init_document(Document *doc, const char *name) {
    int length = 0;
    if(doc == NULL || name == NULL) { /* Checking for null */
        return FAILURE;
    }
    length = strlen(name); /* Faster */
    if ((length < 1) || (length > MAX_STR_SIZE)) {
        return FAILURE;
    }

    strcpy(doc->name, name);
    return clear_doc(doc);
}

int reset_document(Document *doc) {
    if(doc == NULL) {
        return FAILURE;
    }
    return clear_doc(doc);
}

int print_document(Document *doc) {
    int i, j;
    if(doc == NULL) {
        return FAILURE;
    }

    printf("Document name: \"%s\"\nNumber of Paragraphs: %d\n", doc->name, 
        doc->number_of_paragraphs);
    for(i = 0; i < doc->number_of_paragraphs; i++) {
        if(i != 0) {
            /* If there is more than one paragraph \n is printed between */
            printf("\n");
        }
        for(j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
            printf("%s\n", doc->paragraphs[i].lines[j]);
        }
    }
    return SUCCESS;
}

int add_paragraph_after(Document *doc, int paragraph_number) {
    int i;
    Paragraph new_p;
    new_p.number_of_lines = 0;
    
    if(doc == NULL) {
        return FAILURE;
    } else if(doc->number_of_paragraphs >= MAX_PARAGRAPHS ||
        paragraph_number < 0 || paragraph_number > doc->number_of_paragraphs) {
        return FAILURE;
    }

    /* Initial paragraph add */
    if(doc->number_of_paragraphs == 0 
        && (paragraph_number == 1 || paragraph_number == 0)) {
        doc->paragraphs[0] = new_p;
        doc->number_of_paragraphs++;
        return SUCCESS;
    }
    /* Moves the values after the replace forward to
     * give room for a new paragraph */
    for(i = doc->number_of_paragraphs - 1; i >= paragraph_number; i--) {
        doc->paragraphs[i + 1] = doc->paragraphs[i];
    }
    /* Add the new paragraph in the empty spot and increment the number */
    doc->paragraphs[paragraph_number] = new_p; 
    doc->number_of_paragraphs++;
    return SUCCESS;
}

int add_line_after(Document *doc, int paragraph_number, int line_number,
	const char *new_line) {
    int i, length;

    if(doc == NULL || new_line == NULL) {
        return FAILURE;
    } else if(paragraph_number < 0 
        || paragraph_number > doc->number_of_paragraphs 
        || doc->paragraphs[paragraph_number - 1].number_of_lines >= 
        MAX_PARAGRAPH_LINES || line_number < 0 || 
        line_number > doc->paragraphs[paragraph_number - 1].number_of_lines) {
        /* Checking all FAILURE */
        return FAILURE;
    }

    length = strlen(new_line);
    if(length < 0 || length > MAX_STR_SIZE) {
        return FAILURE;
    }
    
    /* Initial line add */
    if(doc->paragraphs[paragraph_number - 1].number_of_lines 
        == 0 && (line_number == 1 || line_number == 0)) {
        strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number], 
            new_line);
        doc->paragraphs[paragraph_number - 1].number_of_lines++;
        return SUCCESS;
    }
    /* Moves the lines so the new line can be added */
    for(i = doc->paragraphs[paragraph_number - 1].number_of_lines - 1; 
        i >= line_number; i--) {
        strcpy(doc->paragraphs[paragraph_number - 1].lines[i + 1], 
            doc->paragraphs[paragraph_number - 1].lines[i]);
    }
    /* Adds a new line and increments the amount of lines */
    strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number], new_line);
    doc->paragraphs[paragraph_number - 1].number_of_lines++;
    return SUCCESS;
}

int get_number_lines_paragraph(Document *doc, int paragraph_number, 
    int *number_of_lines) {
    if(doc == NULL || number_of_lines == NULL) {
        return FAILURE;
    } else if(paragraph_number < 1 || 
        paragraph_number > doc->number_of_paragraphs) {
        return FAILURE;
    }
    *number_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;
    return SUCCESS;
}

int append_line(Document *doc, int paragraph_number, const char *new_line) {
    int num_lines = 0;
    /* Null check not needed as it is included in both 
     * get_number_lines_paragraph and add_line_after */
    if(get_number_lines_paragraph(doc, paragraph_number, &num_lines) 
        == SUCCESS) {
        return add_line_after(doc, paragraph_number, num_lines, new_line);
    }
    return FAILURE;
}

int remove_line(Document *doc, int paragraph_number, int line_number) {
    int i;
    if(doc == NULL) {
        return FAILURE;
    } else if(paragraph_number < 1 || 
        paragraph_number > doc->number_of_paragraphs || line_number < 1 || 
        line_number > doc->paragraphs[paragraph_number - 1].number_of_lines) {
        /* Checking all FAILURE */
        return FAILURE;
    }

    /* Moves all the lines back, deleting the removed line by replacing it */
    for(i = line_number - 1; 
        i < doc->paragraphs[paragraph_number - 1].number_of_lines - 1; i++) {
        strcpy(doc->paragraphs[paragraph_number - 1].lines[i], 
            doc->paragraphs[paragraph_number - 1].lines[i + 1]);
    }
    doc->paragraphs[paragraph_number - 1].number_of_lines--;
    return SUCCESS;
}

int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], 
    int data_lines) {
    Document fail_doc;
    int i = 0, j = 1, bool_return = SUCCESS;
    if(doc == NULL || data == NULL) {
        return FAILURE;
    } else if(data_lines < 1) {
        return FAILURE;
    }
    fail_doc = *doc; /* In case we fail, keep the original document */

    bool_return = add_paragraph_after(doc, 0);
    while(i < data_lines && bool_return == SUCCESS) {
        if(strcmp(data[i], "") == 0) {
            /* Adds a paragraph when line is empty, if max number of paragraphs 
             * has been reached the while loop ends and we return failure */
            bool_return = add_paragraph_after(doc, j);
            j++;
        } else {
            /* Adds a line when line isn't empty, if max number of lines 
             * has been reached the while loop ends and we return failure */
            bool_return = append_line(doc, j, data[i]);
        }
        i++;
    }
    if(bool_return == FAILURE) {
        /* We revert the changes to doc on failure */
        *doc = fail_doc;
    }
    return bool_return;
}

int replace_text(Document *doc, const char *target, const char *replacement) {
    int i, j, t_length = 0, r_length = 0;
    char replace_str[MAX_STR_SIZE + 1] = "";
    char *substring;
    if(doc == NULL || target == NULL || replacement == NULL) {
        return FAILURE;
    }
    t_length = strlen(target);
    r_length = strlen(replacement);

    for(i = 0; i < MAX_PARAGRAPHS; i++) {
        for(j = 0; j < MAX_PARAGRAPH_LINES; j++) {
            /* strstr finds the first occurance of the target in the line */
            substring = strstr(doc->paragraphs[i].lines[j], target);
            while(substring != NULL) {
                /* When the target is found we make a string, replace_str,  that
                 * is the replacement string, then the rest of the line without 
                 * the target string. */
                strcat(replace_str, replacement);
                strcat(replace_str, substring + t_length);
                /* We then replace the rest of the line with that replacement 
                 * string */
                strcpy(substring, replace_str);
                /* We then find the next place where the target is found after 
                 * the replace string (since the replace can include the target 
                 * string) and clear the replacement for use if there is more 
                 * target strings in the line */
                substring = strstr(substring + r_length, target);
                strcpy(replace_str, "");
            }
        }
    }
    return SUCCESS;
}

int highlight_text(Document *doc, const char *target) {
    /* This creates the highlighted string */
    char combine_string[MAX_STR_SIZE + 1] = HIGHLIGHT_START_STR;
    strcat(combine_string, target);
    strcat(combine_string, HIGHLIGHT_END_STR);
    /* Find the target and replace with the highlighted version */
    return replace_text(doc, target, combine_string);
}

int remove_text(Document *doc, const char *target) {
    /* Find the target and replace with nothing */
    return replace_text(doc, target, "");
}