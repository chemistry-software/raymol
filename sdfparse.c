#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ATOMS 1000
#define MAX_LINE_LENGTH 256

typedef struct {
    float x, y, z;
    char atom_type[3];
} Atom;

typedef struct {
    Atom atoms[MAX_ATOMS];
    int num_atoms;
} Molecule;

Molecule parseSDF(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(1);
    }

    Molecule mol;
    mol.num_atoms = 0;

    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        // Check for the line containing atom coordinates
        printf("%s", line);
        if (strlen(line) > 3 && isdigit(line[0])) {
            sscanf(line, "%*d%f%f%f%2s", &mol.atoms[mol.num_atoms].x,
                   &mol.atoms[mol.num_atoms].y, &mol.atoms[mol.num_atoms].z,
                   mol.atoms[mol.num_atoms].atom_type);
            mol.num_atoms++;
        }
    }

    fclose(file);
    return mol;
}
