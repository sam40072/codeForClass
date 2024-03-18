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

void my_string_destroy(My_string** phMy_string) {
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


int my_string_get_capacity(My_string* hMy_string) {
    if (hMy_string == NULL) {
        return -1; // Or handle the error as appropriate
    }
    return hMy_string->capacity;
}
int my_string_get_size(My_string* hMy_string) {
    if (hMy_string == NULL) {
        return -1; // Or handle the error as appropriate
    }
    return hMy_string->size;
}

int my_string_compare(My_string* hLeft_string, My_string* hRight_string) {
    if (hLeft_string == NULL || hRight_string == NULL) {
        return -2; // Or handle the error as appropriate
    }

    int min_size = hLeft_string->size < hRight_string->size ? hLeft_string->size : hRight_string->size;
    for (int i = 0; i < min_size; ++i) {
        if (hLeft_string->data[i] < hRight_string->data[i]) {
            return -1;
        }
        else if (hLeft_string->data[i] > hRight_string->data[i]) {
            return 1;
        }
    }

    // If all characters are the same up to min_size, the shorter string is considered smaller
    if (hLeft_string->size < hRight_string->size) {
        return -1;
    }
    else if (hLeft_string->size > hRight_string->size) {
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
    MY_STRING pMy_string = (MY_STRING)hMy_string;

    do {
        c = fgetc(fp);
        if (c == EOF) {
            return FAILURE;
        }
    } while (isspace(c));


    while (!isspace(c)) {
        if (my_string_push_back(pMy_string, c) == FAILURE) {
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

    MY_STRING pMy_string = (MY_STRING)hMy_string;
    int size = my_string_get_size(pMy_string);
    char* str = my_string_c_str(pMy_string);

    if (fwrite(str, sizeof(char), size, fp) != size) {
        return FAILURE;
    }

    return SUCCESS;


}



Status my_string_push_back(My_string* hMy_string, char character) {
    if (hMy_string == NULL) {
        return FAILURE; // Or handle the error as appropriate
    }

    // Ensure capacity
    if (hMy_string->size >= hMy_string->capacity) {
        int new_capacity = (hMy_string->capacity == 0) ? 1 : 2 * hMy_string->capacity; // Double the capacity
        char* new_data = realloc(hMy_string->data, new_capacity * sizeof(char));
        if (new_data == NULL) {
            return FAILURE; // Or handle the error as appropriate
        }
        hMy_string->data = new_data;
        hMy_string->capacity = new_capacity;
    }

    // Append the character
    hMy_string->data[hMy_string->size++] = character;

    return SUCCESS;
}

char* my_string_c_str(My_string* hMy_string) {
    if (hMy_string == NULL) {
        return NULL;
    }

    // Allocate memory for the null-terminated string
    char* c_str = malloc((hMy_string->size + 1) * sizeof(char));
    if (c_str == NULL) {
        return NULL; // Or handle the error as appropriate
    }

    // Copy the characters from the My_string data array to the C string
    for (int i = 0; i < hMy_string->size; ++i) {
        c_str[i] = hMy_string->data[i];
    }
    c_str[hMy_string->size] = '\0'; // Null-terminate the string

    return c_str;
}