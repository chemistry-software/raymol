#include "debug.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ATOMS 1000
#define MAX_LINE_LENGTH 256
#define MAX_NEIGHBOURS 4

typedef struct Atom {
  float x, y, z;
  char atom_type[3];
  int idx;
  int num_neighbours;
  int bond_orders[MAX_NEIGHBOURS];
  struct Atom *neighbours[MAX_NEIGHBOURS];
  bool bonds_drawn;
} Atom;

typedef struct Molecule {
  Atom atoms[MAX_ATOMS];
  int num_atoms;
} Molecule;

void addBond(Atom *atom1, Atom *atom2, int bond_order);

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
    // Parse the atoms and their coordinates
    if (isdigit(line[4]) && isdigit(line[6]) && isalpha(line[31])) {
      sscanf(line, "\t%f\t%f\t%f\t%s", &mol.atoms[mol.num_atoms].x,
             &mol.atoms[mol.num_atoms].y, &mol.atoms[mol.num_atoms].z,
             mol.atoms[mol.num_atoms].atom_type);
      mol.atoms[mol.num_atoms].idx = mol.num_atoms + 1;
      mol.atoms[mol.num_atoms].bonds_drawn = false;
      ++mol.num_atoms;
    }
    // Parse the connectivity info block, atom count in SDF starts at 1
    if (isdigit(line[2]) && isdigit(line[20])) {
      int atom_idx, neighbour_idx, bond_order;
      sscanf(line, "%d %d %d", &atom_idx, &neighbour_idx, &bond_order);
      D printf("idx %d, buurman %d, multipliciteit %d\n", atom_idx,
               neighbour_idx, bond_order);
      addBond(&mol.atoms[atom_idx - 1], &mol.atoms[neighbour_idx - 1],
              bond_order);
    }
  }

  if (fclose(file)) {
    fprintf(stderr, "Error CLOSING file %s for some reason!\n", filename);
    exit(1);
  };

  return mol;
}

void addBond(Atom *atom1, Atom *atom2, int bond_order) {
  if (atom1->num_neighbours < MAX_NEIGHBOURS &&
      atom2->num_neighbours < MAX_NEIGHBOURS) {
    atom1->neighbours[atom1->num_neighbours] = atom2;
    atom1->bond_orders[atom1->num_neighbours] = bond_order;
    atom1->num_neighbours++;

    atom2->neighbours[atom2->num_neighbours] = atom1;
    atom2->bond_orders[atom2->num_neighbours] = bond_order;
    atom2->num_neighbours++;
  } else {
    D printf("ERROR: MAX_NEIGHBOURS exceeded!\n");
  }
}

void normalizeCoordinates(Molecule *mol, int width, int height) {
  float min_x = mol->atoms[0].x, max_x = mol->atoms[0].x;
  float min_y = mol->atoms[0].y, max_y = mol->atoms[0].y;
  float min_z = mol->atoms[0].z, max_z = mol->atoms[0].z;

  // Find min and max coords
  for (int i = 1; i < mol->num_atoms; i++) {
    if (mol->atoms[i].x < min_x)
      min_x = mol->atoms[i].x;
    if (mol->atoms[i].x > max_x)
      max_x = mol->atoms[i].x;
    if (mol->atoms[i].y < min_y)
      min_y = mol->atoms[i].y;
    if (mol->atoms[i].y > max_y)
      max_y = mol->atoms[i].y;
    if (mol->atoms[i].z < min_z)
      min_z = mol->atoms[i].z;
    if (mol->atoms[i].z > max_z)
      max_z = mol->atoms[i].z;
  }

  // Scaling factors
  float scale_x = (float)width / (max_x - min_x) / 100;
  float scale_y = (float)height / (max_y - min_y) / 100;
  // Yeah well there is no depth to a screen lol
  float scale_z = (float)height / (max_z - min_z) / 100;

  // Normalize coordinates, lets hope we never need the original coords lmao
  for (int i = 0; i < mol->num_atoms; i++) {
    mol->atoms[i].x = (mol->atoms[i].x - min_x) * scale_x;
    mol->atoms[i].y = (mol->atoms[i].y - min_y) * scale_y;
    mol->atoms[i].z = (mol->atoms[i].z - min_z) * scale_z;
  }
}
