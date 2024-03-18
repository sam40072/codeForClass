#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include <string.h>

MY_STRING my_string_init_default(void) {
    My_string* hMy_string = (My_string*)malloc(sizeof(My_string));
    if (hMy_string == NULL) {
        return NULL;
    }
    
    // Initialize with default capacity
    hMy_string->size = 0;
    hMy_string->capacity = 7;
    hMy_string->data = malloc(hMy_string->capacity * sizeof(char));
    if (hMy_string->data == NULL) {
        free(hMy_string);
        return NULL;
    }

    return hMy_string;
}

void my_string_destroy(MY_STRING* phMy_string) {
    if (phMy_string == NULL || *phMy_string == NULL) {
        return;
    }

    free((*phMy_string)->data);
    free(*phMy_string);
    *phMy_string = NULL;
}

MY_STRING my_string_init_c_string(const char* c_string) {
    if (c_string == NULL) {
        return NULL;
    }

    My_string* hMy_string = my_string_init_default();
    if (hMy_string == NULL) {
        return NULL;
    }

    int length = strlen(c_string);
    if (length >= hMy_string->capacity) {
        hMy_string->capacity = length + 1;
        hMy_string->data = realloc(hMy_string->data, hMy_string->capacity * sizeof(char));
        if (hMy_string->data == NULL) {
            my_string_destroy(&hMy_string);
            return NULL;
        }
    }

    strcpy(hMy_string->data, c_string);
    hMy_string->size = length;

    return hMy_string;
}

int my_string_get_capacity(MY_STRING hMy_string) {
    if (hMy_string == NULL) {
        return -1; // Or handle the error as appropriate
    }
    return ((My_string*)hMy_string)->capacity;
}

int my_string_get_size(MY_STRING hMy_string) {
    if (hMy_string == NULL) {
        return -1; // Or handle the error as appropriate
    }
    return ((My_string*)hMy_string)->size;
}

int my_string_compare(MY_STRING hLeft_string, MY_STRING hRight_string) {
    if (hLeft_string == NULL || hRight_string == NULL) {
        return -2; // Or handle the error as appropriate
    }

    My_string* left = (My_string*)hLeft_string;
    My_string* right = (My_string*)hRight_string;

    int min_size = left->size < right->size ? left->size : right->size;
    for (int i = 0; i < min_size; ++i) {
        if (left->data[i] < right->data[i]) {
            return -1;
        }
        else if (left->data[i] > right->data[i]) {
            return 1;
        }
    }

    // If all characters are the same up to min_size, the shorter string is considered smaller
    if (left->size < right->size) {
        return -1;
    }
    else if (left->size > right->size) {
        return 1;
    }
    else {
        return 0; // Both strings are equal
    }
}

Status my_string_extraction(MY_STRING hMy_string, FILE* fp) {
    if (hMy_string == NULL || fp == NULL) {
        return FAILURE;
    }

    char c;
    My_string* pMy_string = (My_string*)hMy_string;

    do {
        c = fgetc(fp);
        if (c == EOF) {
            return FAILURE;
        }
    } while (isspace(c));


    while (!isspace(c)) {
        if (my_string_push_back(hMy_string, c) == FAILURE) {
            return FAILURE;
        }
        c = fgetc(fp);
        if (c == EOF) {
            break;
        }
    }

    ungetc(c, fp);

    return SUCCESS;
}

Status my_string_insertion(MY_STRING hMy_string, FILE* fp) {
    if (hMy_string == NULL || fp == NULL) {
        return FAILURE;
    }

    My_string* pMy_string = (My_string*)hMy_string;
    int size = my_string_get_size(hMy_string);
    char* str = my_string_c_str(hMy_string);

    if (fwrite(str, sizeof(char), size, fp) != size) {
        return FAILURE;
    }

    return SUCCESS;
}

Status my_string_push_back(MY_STRING hMy_string, char item) {
    My_string* pString = (My_string*)hMy_string;

    // Check if resizing is needed
    if (pString->size >= pString->capacity) {
        // Perform resizing
        // Assuming my_string_resize is implemented elsewhere
        // If not, you need to provide the implementation
        if (!my_string_resize(pString, pString->capacity * 2)) {
            return FAILURE; 
        }
    }

    // Append the character at the end of the string
    pString->data[pString->size++] = item;

    return SUCCESS;
}

char* my_string_c_str(MY_STRING hMy_string) {
    My_string* pMy_string = (My_string*)hMy_string;

    if (pMy_string == NULL) {
        return NULL;
    }

    // Allocate memory for the null-terminated string
    char* c_str = malloc((pMy_string->size + 1) * sizeof(char));
    if (c_str == NULL) {
        return
