#include <stdio.h>
#include <string.h>
#include "text_manipulation.h"

int remove_spaces(const char *source, char *result, int *num_spaces_removed) {
    int i = 0, j, k = 0, length;

    if(source[0] == '\0' || source == NULL) {
        return FAILURE;
    } else if(strlen(source) == 0) {
        return FAILURE;
    }
    length = strlen(source);
    j = length - 1;

    while(source[j] == ' ') {
        j--;
    }
    while(source[i] == ' ') {
        i++;
    }
    if(num_spaces_removed != NULL) {
        *num_spaces_removed = length - (j - i) - 1;
    }
    
    for(i; i < j + 1; i++) {
        result[k] = source[i];
        k++;
    }
    result[k] = '\0';

    return SUCCESS;
}

int center(const char *source, int width, char *result) {
    int length, calc, i, j = 0;

    if(source[0] == '\0' || source == NULL) {
        return FAILURE;
    }
    length = strlen(source);
    calc = width - length;
    if(calc < 0 || length == 0) {
        return FAILURE;
    }

    if(calc % 2 != 0) {
        calc--;
    }
    calc /= 2;

    for(i = 0; i < calc; i++) {
        result[i] = ' ';
    }
    length += i;
    for (i; i < length; i++) {
        result[i] = source[j];
        j++;
    }
    calc += i;
    for(i; i < calc; i++) {
        result[i] = ' ';
    }
    result[i] = '\0';
    return SUCCESS;
}
