// Periodic Table CVS file from https://github.com/Bowserinator/Periodic-Table-JSON

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ELEMENTS 120
#define MAX_LINE_LENGTH 2048

typedef struct {
    char name[50];
    char appearance[100];
    double atomic_mass;
    double boil;
    char category[50];
    double density;
    char discovered_by[50];
    double melt;
    double molar_heat;
    char named_by[50];
    int number;
    int period;
    int group;
    char phase[10];
    char source[100];
    char bohr_model_image[200];
    char bohr_model_3d[200];
    char spectral_img[200];
    char summary[500];
    char symbol[3];
    int xpos;
    int ypos;
    int wxpos;
    int wypos;
    char shells[50];
    char electron_configuration[50];
    char electron_configuration_semantic[50];
    double electron_affinity;
    double electronegativity_pauling;
    char ionization_energies[200];
    char cpk_hex[10];
    char block[2];
    char image_title[200];
    char image_url[200];
    char image_attribution[200];
} Element;

// Function to trim leading and trailing whitespaces
char* trimWhitespace(char* str) {
    char* end;

    // Trim leading space
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

// Function to parse a CSV line considering quoted fields
void parseCSVLine(char* line, char** fields, int field_count) {
    int field_index = 0;
    int in_quotes = 0;
    char* field_start = line;

    for (char* p = line; *p; p++) {
        if (*p == '\"') {
            in_quotes = !in_quotes;  // Toggle quote state
        } else if (*p == ',' && !in_quotes) {
            *p = '\0';
            fields[field_index++] = trimWhitespace(field_start);
            field_start = p + 1;
        }
    }

    fields[field_index] = trimWhitespace(field_start);
}

// Function to parse the CSV file
Element* parseCSV(const char* filename, int* element_count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open file");
        return NULL;
    }

    Element* elements = (Element*)malloc(MAX_ELEMENTS * sizeof(Element));
    if (!elements) {
        perror("Unable to allocate memory");
        fclose(file);
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    fgets(line, sizeof(line), file); // Skip the header line

    *element_count = 0;
    while (fgets(line, sizeof(line), file)) {
        if (*element_count >= MAX_ELEMENTS) break;

        char* fields[35] = {0};  // Array to hold all fields in the CSV line
        parseCSVLine(line, fields, 35);

        Element elem = {0};  // Initialize all struct members to zero or empty

        strncpy(elem.name, fields[0], sizeof(elem.name) - 1);
        strncpy(elem.appearance, fields[1], sizeof(elem.appearance) - 1);
        elem.atomic_mass = atof(fields[2]);
        elem.boil = atof(fields[3]);
        strncpy(elem.category, fields[4], sizeof(elem.category) - 1);
        elem.density = atof(fields[5]);
        strncpy(elem.discovered_by, fields[6], sizeof(elem.discovered_by) - 1);
        elem.melt = atof(fields[7]);
        elem.molar_heat = atof(fields[8]);
        strncpy(elem.named_by, fields[9], sizeof(elem.named_by) - 1);
        elem.number = atoi(fields[10]);
        elem.period = atoi(fields[11]);
        elem.group = atoi(fields[12]);
        strncpy(elem.phase, fields[13], sizeof(elem.phase) - 1);
        strncpy(elem.source, fields[14], sizeof(elem.source) - 1);
        strncpy(elem.bohr_model_image, fields[15], sizeof(elem.bohr_model_image) - 1);
        strncpy(elem.bohr_model_3d, fields[16], sizeof(elem.bohr_model_3d) - 1);
        strncpy(elem.spectral_img, fields[17], sizeof(elem.spectral_img) - 1);
        strncpy(elem.summary, fields[18], sizeof(elem.summary) - 1);
        strncpy(elem.symbol, fields[19], sizeof(elem.symbol) - 1);
        elem.xpos = atoi(fields[20]);
        elem.ypos = atoi(fields[21]);
        elem.wxpos = atoi(fields[22]);
        elem.wypos = atoi(fields[23]);
        strncpy(elem.shells, fields[24], sizeof(elem.shells) - 1);
        strncpy(elem.electron_configuration, fields[25], sizeof(elem.electron_configuration) - 1);
        strncpy(elem.electron_configuration_semantic, fields[26], sizeof(elem.electron_configuration_semantic) - 1);
        elem.electron_affinity = atof(fields[27]);
        elem.electronegativity_pauling = atof(fields[28]);
        strncpy(elem.ionization_energies, fields[29], sizeof(elem.ionization_energies) - 1);
        strncpy(elem.cpk_hex, fields[30], sizeof(elem.cpk_hex) - 1);
        strncpy(elem.block, fields[31], sizeof(elem.block) - 1);
        strncpy(elem.image_title, fields[32], sizeof(elem.image_title) - 1);
        strncpy(elem.image_url, fields[33], sizeof(elem.image_url) - 1);
        strncpy(elem.image_attribution, fields[34], sizeof(elem.image_attribution) - 1);

        elements[*element_count] = elem;
        (*element_count)++;
    }

    fclose(file);
    return elements;
}

int main() {
    int element_count;
    Element* elements = parseCSV("resources/periodictable.csv", &element_count);
    if (elements) {
        for (int i = 0; i < element_count; i++) {
            printf("Element: %s (%s), Atomic Mass: %.2f, Atomic Radius: %.2f\n", elements[i].name, elements[i].symbol, elements[i].atomic_mass, elements[i].electron_affinity);
        }
        free(elements);
    }
    return 0;
}
