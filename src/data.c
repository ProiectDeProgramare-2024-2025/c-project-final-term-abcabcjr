#include "data.h"

#include <stdio.h>
#include <string.h>

// Data saving/loading
// -----------------------------------------------------

void save_app_data(const app_data_t* app_data) {
    FILE* file = fopen("data.txt", "w");

    for (size_t i = 0; i < app_data->resume_count; i++) {
        const resume_t* resume = &app_data->resumes[i];

        fprintf(file, "%s\n", "NEW");

        fprintf(file, "%s\n", resume->name);
        fprintf(file, "%s\n", resume->surname);
        fprintf(file, "%s\n", resume->phone);

        for (const position_t* pos = resume->first_position; pos != NULL; pos = pos->next) {
            fprintf(file, "%s\n", "POS");
            fprintf(file, "%s\n", pos->title);
            fprintf(file, "%s\n", pos->company);
            fprintf(file, "%s\n", pos->description);
        }

        fprintf(file, "%s\n", "END");
    }

    fclose(file);
}

void load_app_data(app_data_t* app_data) {
    FILE* file = fopen("data.txt", "r");
    if (!file) {
        app_data->resume_count = 0;
        return;
    }

    char line[512];
    size_t resume_count = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = '\0';
        if (strcmp(line, "NEW") != 0)
            break;

        resume_t* resume = &app_data->resumes[resume_count++];
        memset(resume, 0, sizeof(*resume));

        if (!fgets(resume->name, sizeof(resume->name), file)) break;
        resume->name[strcspn(resume->name, "\r\n")] = '\0';

        if (!fgets(resume->surname, sizeof(resume->surname), file)) break;
        resume->surname[strcspn(resume->surname, "\r\n")] = '\0';

        if (!fgets(resume->phone, sizeof(resume->phone), file)) break;
        resume->phone[strcspn(resume->phone, "\r\n")] = '\0';

        while (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\r\n")] = '\0';
            if (strcmp(line, "END") == 0)
                break;
            if (strcmp(line, "POS") == 0) {
                position_t* pos = malloc(sizeof(*pos));
                pos->next = NULL;

                fgets(pos->title, sizeof(pos->title), file);
                pos->title[strcspn(pos->title, "\r\n")] = '\0';

                fgets(pos->company, sizeof(pos->company), file);
                pos->company[strcspn(pos->company, "\r\n")] = '\0';

                fgets(pos->description, sizeof(pos->description), file);
                pos->description[strcspn(pos->description, "\r\n")] = '\0';

                if (resume->last_position == NULL)
                    resume->first_position = resume->last_position = pos;
                else {
                    resume->last_position->next = pos;
                    resume->last_position = pos;
                }
            }
        }
    }

    app_data->resume_count = resume_count;
    fclose(file);
}