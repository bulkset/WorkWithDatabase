#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ENTRIES 100

typedef struct {
    char name[50];
    char surname[50];
    int age;
    int birth_date;
    char profession[50];
} Database;

int isInteger(const char *input) {
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] < '0' || input[i] > '9') {
            return 0;
        }
    }
    return 1;
}

int isInRange(int value, int lower, int upper) {
    return value >= lower && value <= upper;
}

void setData(Database *data, int n) {
    char input[50];
    for (int i = 0; i < n; i++) {
        printf("%d. name: ", i + 1);
        scanf("%s", data[i].name);

        if (strcmp(data[i].name, ".exit") == 0) {
            break;
        }

        printf("%d. surname: ", i + 1);
        scanf("%s", data[i].surname);

        if (strcmp(data[i].surname, ".exit") == 0) {
            break;
        }

        const char *prompts[] = {"age", "birth date", "profession"};

        for (int j = 0; j < 3; j++) {
            do {
                printf("%d. %s: ", i + 1, prompts[j]);
                scanf("%s", input);

                if (strcmp(input, ".exit") == 0) {
                    break;
                }

                if (j == 2) {
                    strcpy(data[i].profession, input);
                    break;
                }

                if (!isInteger(input)) {
                    printf("Error! Enter %s in numeric format.\n", prompts[j]);
                } else {
                    int value = atoi(input);
                    if (j == 0 && !isInRange(value, 7, 99)) {
                        printf("Error! %s must be between 7 and 99.\n", prompts[j]);
                    } else {
                        j == 0 ? (data[i].age = value) : (data[i].birth_date = value);
                        break;
                    }
                }
            } while (1);
        }
    }
}

void getData(const Database *data, int n, const char *filename) {
    FILE *outFile = fopen(filename, "a");
    if (outFile == NULL) {
        fprintf(stderr, "Error opening file for writing.\n");
        exit(1);
    }

    fprintf(outFile, "| %-15s | %-15s | %-5s | %-15s | %-15s |\n", "name", "surname", "age", "birth date", "profession");
    fprintf(outFile, "|-----------------|-----------------|-------|-----------------|-----------------|\n");

    for (int i = 0; i < n; i++) {
        if (strcmp(data[i].name, ".exit") == 0 || data[i].name[0] == '\0' || data[i].surname[0] == '\0') {
            continue;  
        }

        fprintf(outFile, "| %-15s | %-15s | %-5d | %-15d | %-15s |\n", data[i].name, data[i].surname, data[i].age, data[i].birth_date, data[i].profession);
    }

    fclose(outFile);
}

int main() {
    Database *entries = malloc(MAX_ENTRIES * sizeof(Database));
    if (entries == NULL) {
        fprintf(stderr, "Memory allocation error for entries.\n");
        exit(1);
    }

    int existingEntries = 0;

    FILE *file = fopen("database.txt", "r");
    if (file) {
        while (fscanf(file, "%*s %*s %*d %*d %*s") == 0) {
            existingEntries++;
        }

        rewind(file);

        for (int i = 0; i < existingEntries; i++) {
            fscanf(file, "%s %s %d %d %s", entries[i].name, entries[i].surname, &entries[i].age, &entries[i].birth_date, entries[i].profession);
        }

        fclose(file);
    }

    printf("Enter data. Enter '.exit' to finish.\n");

    setData(entries + existingEntries, MAX_ENTRIES - existingEntries);

    char timestamp[20];
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timestamp, sizeof(timestamp), "%d%H%M%S", timeinfo);

    char filename[50];
    snprintf(filename, sizeof(filename), "database_%s.txt", timestamp);
    getData(entries, MAX_ENTRIES, filename);

    free(entries);

    return 0;
}
