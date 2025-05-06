#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"

#define COLOR_BLUE    "\x1b[34m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_RESET   "\x1b[0m"

// -----------------------------------------------------
// Logic
// -----------------------------------------------------

void readline(char *buf, const size_t size) {
    if (fgets(buf, size, stdin)) {
        buf[strcspn(buf, "\n")] = '\0';
    }
}

void add_resume(app_data_t *data) {
    printf(COLOR_YELLOW "Welcome to CV creation!\n" COLOR_RESET);

    resume_t resume;
    resume.first_position = NULL;
    resume.last_position = NULL;

    getchar();
    printf(COLOR_CYAN "Enter your name: " COLOR_RESET);
    readline(resume.name, sizeof(resume.name));
    printf(COLOR_CYAN "Enter your surname: " COLOR_RESET);
    readline(resume.surname, sizeof(resume.surname));
    printf(COLOR_CYAN "Enter your phone number: " COLOR_RESET);
    readline(resume.phone, sizeof(resume.phone));

    char pos = 'n';
    do {
        printf(COLOR_CYAN "Want to add a previous position? [Y/n]: " COLOR_RESET);
        char line[16];
        readline(line, sizeof(line));
        pos = line[0];

        if (pos != 'y' && pos != 'Y')
            break;

        position_t *new_position = malloc(sizeof(position_t));
        new_position->next = NULL;

        printf(COLOR_CYAN "  Company: " COLOR_RESET);
        readline(new_position->company, sizeof(new_position->company));
        printf(COLOR_CYAN "  Title: " COLOR_RESET);
        readline(new_position->title, sizeof(new_position->title));
        printf(COLOR_CYAN "  Description: " COLOR_RESET);
        readline(new_position->description, sizeof(new_position->description));

        if (resume.last_position == NULL)
            resume.last_position = resume.first_position = new_position;
        else {
            resume.last_position->next = new_position;
            resume.last_position = new_position;
        }
    } while (pos == 'y' || pos == 'Y');

    data->resumes[data->resume_count++] = resume;
    save_app_data(data);

    printf(COLOR_GREEN "Resume added successfully!\n" COLOR_RESET);
}

void view_users(const app_data_t *data) {
    if (data->resume_count == 0) {
        printf(COLOR_RED "No resumes have been added!\n" COLOR_RESET);
        return;
    }
    for (size_t i = 0; i < data->resume_count; i++) {
        const resume_t *resume = &data->resumes[i];
        printf(COLOR_BLUE "%zu." COLOR_RESET " %s %s\n",
               (i + 1), resume->name, resume->surname);
    }
}

void print_cv(const resume_t *resume) {
    printf("Name: %s %s\n", resume->name, resume->surname);
    printf("Phone: %s\n", resume->phone);
    printf("Previous positions:\n");

    const position_t *position = resume->first_position;
    size_t i = 0;
    while (position != NULL) {
        printf(COLOR_BLUE "  %llu. " COLOR_RESET "%s at %s\n",
               (unsigned long long) (++i), position->title, position->company);
        printf("    %s\n", position->description);
        position = position->next;
    }
    printf("\n");
}

void get_cv(const app_data_t *data) {
    size_t user_id = 0;
    printf(COLOR_CYAN "Enter user ID: " COLOR_RESET);
    scanf("%zu", &user_id);

    if (user_id < 1 || (user_id - 1) >= data->resume_count) {
        printf(COLOR_RED "Invalid user ID!\n" COLOR_RESET);
        return;
    }
    print_cv(&data->resumes[user_id - 1]);
}

void search_cvs(const app_data_t *data) {
    char keyword[50];
    printf(COLOR_CYAN "Enter search keyword: " COLOR_RESET);
    scanf("%49s", keyword);

    for (size_t i = 0; i < data->resume_count; i++) {
        const resume_t *r = &data->resumes[i];
        int found = strstr(r->name, keyword) || strstr(r->surname, keyword) ||
                    strstr(r->phone, keyword);
        const position_t *p = r->first_position;
        while (p && !found) {
            if (strstr(p->company, keyword) ||
                strstr(p->title, keyword) ||
                strstr(p->description, keyword)) {
                found = 1;
            }
            p = p->next;
        }
        if (found) {
            print_cv(r);
        }
    }
}

void exit_app() {
    printf(COLOR_YELLOW "Goodbye!\n" COLOR_RESET);
}

void invalid_option() {
    printf(COLOR_RED "Option does not exist!\n" COLOR_RESET);
}

// -----------------------------------------------------
// User interaction
// -----------------------------------------------------

void clear_console() {
#if TARGET_OS_WIN32
    system("cls");
#else
    system("clear");
#endif
}

void before_continue() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    printf(COLOR_CYAN "Press Enter to continue..." COLOR_RESET "\n");
    getchar();
}

void header() {
    clear_console();
    printf(COLOR_BLUE "-------------------------\n" COLOR_RESET);
    printf("  " COLOR_BLUE "1)" COLOR_RESET " Create a resume\n");
    printf("  " COLOR_BLUE "2)" COLOR_RESET " View user list\n");
    printf("  " COLOR_BLUE "3)" COLOR_RESET " Get CV\n");
    printf("  " COLOR_BLUE "4)" COLOR_RESET " Search CVs\n");
    printf("  " COLOR_BLUE "0)" COLOR_RESET " Exit\n");
    printf(COLOR_BLUE "-------------------------\n" COLOR_RESET);
}

void menu(const int op, app_data_t *data) {
    switch (op) {
        case 0:
            exit_app();
            break;
        case 1:
            clear_console();
            add_resume(data);
            before_continue();
            break;
        case 2:
            clear_console();
            view_users(data);
            before_continue();
            break;
        case 3:
            clear_console();
            get_cv(data);
            before_continue();
            break;
        case 4:
            clear_console();
            search_cvs(data);
            before_continue();
            break;
        default: invalid_option();
    }
}

int main() {
    app_data_t data;
    load_app_data(&data);

    int option;
    do {
        header();
        printf(COLOR_CYAN "Select an option: " COLOR_RESET);
        scanf("%d", &option);
        menu(option, &data);
    } while (option != 0);

    return 0;
}
