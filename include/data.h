#pragma once
#include <stdlib.h>

typedef struct Position {
    char company[50];
    char title[50];
    char description[250];
    struct Position* next;
} position_t;

typedef struct {
    char name[50];
    char surname[50];
    char phone[11];
    position_t* first_position;
    position_t* last_position;
} resume_t;

typedef struct {
    size_t resume_count;
    resume_t resumes[50];
} app_data_t;

void save_app_data(const app_data_t* app_data);
void load_app_data(app_data_t* app_data);