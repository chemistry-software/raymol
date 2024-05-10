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
        if (strlen(line) > 3 && isdigit(line[4]) && isalpha(line[31])) {
            // this wonky shit assumes tab separated coords
            sscanf(line, "%f\t%f\t%f\t%s", &mol.atoms[mol.num_atoms].x,
                   &mol.atoms[mol.num_atoms].y, &mol.atoms[mol.num_atoms].z,
                   mol.atoms[mol.num_atoms].atom_type);
            mol.num_atoms++;
        }
    }

    fclose(file);
    return mol;
}

void normalizeCoordinates(Molecule *mol, int width, int height) {
    float min_x = mol->atoms[0].x, max_x = mol->atoms[0].x;
    float min_y = mol->atoms[0].y, max_y = mol->atoms[0].y;
    float min_z = mol->atoms[0].z, max_z = mol->atoms[0].z;

    // Find min and max coords
    for (int i = 1; i < mol->num_atoms; i++) {
        if (mol->atoms[i].x < min_x) min_x = mol->atoms[i].x;
        if (mol->atoms[i].x > max_x) max_x = mol->atoms[i].x;
        if (mol->atoms[i].y < min_y) min_y = mol->atoms[i].y;
        if (mol->atoms[i].y > max_y) max_y = mol->atoms[i].y;
        if (mol->atoms[i].z < min_z) min_z = mol->atoms[i].z;
        if (mol->atoms[i].z > max_z) max_z = mol->atoms[i].z;
    }

    // Scaling factors
    float scale_x = (float)width / (max_x - min_x);
    float scale_y = (float)height / (max_y - min_y);
    // Yeah well there is no depth to a screen lol
    float scale_z = (float)height / (max_z - min_z);

    // Normalize coordinates, lets hope we never need the original coords lmao
    for (int i = 0; i < mol->num_atoms; i++) {
        mol->atoms[i].x = (mol->atoms[i].x - min_x) * scale_x;
        mol->atoms[i].y = (mol->atoms[i].y - min_y) * scale_y;
        mol->atoms[i].z = (mol->atoms[i].z - min_z) * scale_z;
    }
}

