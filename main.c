#include "debug.h"
#include "raylib.h"
#include "sdfparse.c"
#include <complex.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------
void drawAtom(Atom atom);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 1200;
  const int screenHeight = 800;

  // Molecule mol = parseSDF("./resources/mol.sdf");
  // Molecule mol = parseSDF("./resources/methyl-vinyl-ketone.sdf");
  Molecule mol = parseSDF("./resources/sildenafil.sdf");
  // normalizeCoordinates(&mol, screenWidth, screenHeight);

  D printf("Number of atoms: %d\n", mol.num_atoms);
  for (int i = 0; i < mol.num_atoms; i++) {
    D printf("Atom %d: %s (%.4f, %.4f, %.4f) - neighbours: %d\n", i + 1,
             mol.atoms[i].atom_type, mol.atoms[i].x, mol.atoms[i].y,
             mol.atoms[i].z, mol.atoms[i].num_neighbours);
  }

  InitWindow(screenWidth, screenHeight, "LOL dongs");

  SetTargetFPS(60); // Set desired framerate (frames-per-second)

  Camera3D camera = {0};
  camera.position = (Vector3){10.0f, 10.0f, 10.0f}; // Set camera position
  camera.target = (Vector3){5.0f, 5.0f, 5.0f};      // Set camera target
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};          // Set camera up vector
  camera.fovy = 45.0f;                              // Set camera field of view
  // camera.projection = CAMERA_ORTHOGRAPHIC;             // Camera projection type
  camera.projection = CAMERA_PERSPECTIVE; // Camera projection type
  int cameraMode = CAMERA_THIRD_PERSON;

  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------
    if (IsKeyPressed(KEY_ONE)) {
      cameraMode = CAMERA_FREE;
      camera.up = (Vector3){0.0f, 1.0f, 0.0f}; // Reset roll
    }

    if (IsKeyPressed(KEY_TWO)) {
      cameraMode = CAMERA_FIRST_PERSON;
      camera.up = (Vector3){0.0f, 1.0f, 0.0f}; // Reset roll
    }

    if (IsKeyPressed(KEY_THREE)) {
      cameraMode = CAMERA_THIRD_PERSON;
      camera.up = (Vector3){0.0f, 1.0f, 0.0f}; // Reset roll
    }

    if (IsKeyPressed(KEY_FOUR)) {
      cameraMode = CAMERA_ORBITAL;
      camera.up = (Vector3){0.0f, 1.0f, 0.0f}; // Reset roll
    }

    if (IsKeyPressed('Z'))
      camera.target = (Vector3){0.0f, 0.0f, 0.0f};

    UpdateCamera(&camera, cameraMode);
    // UpdateCamera(&camera, CAMERA_ORBITAL);

    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(camera);

    // Draw atoms
    for (int i = 0; i < mol.num_atoms; i++) {
      drawAtom(mol.atoms[i]);

      Color bondColor = GREEN;

      for (int j = 0; j < mol.atoms[i].num_neighbours; j++) {
        if (!mol.atoms[i].bonds_drawn) {
          if (mol.atoms[i].bond_orders[j] > 1) {
            bondColor = BLUE;
          }

          DrawCylinderWiresEx(
              (Vector3){mol.atoms[i].x, mol.atoms[i].y, mol.atoms[i].z},
              (Vector3){mol.atoms[i].neighbours[j]->x,
                        mol.atoms[i].neighbours[j]->y,
                        mol.atoms[i].neighbours[j]->z},
              0.1f, 0.1f, 20, bondColor);
        }
        mol.atoms[i].bonds_drawn = true;
      }
    }

    EndMode3D();

    // Print atom coordinates
    DrawText("Atom Coordinates:", 20, 20, 20, DARKGRAY);
    for (int i = 0; i < mol.num_atoms; i++) {
      DrawText(TextFormat("Atom %d: %s (%.4f, %.4f, %.4f) neighbours: %d",
                          mol.atoms[i].idx, mol.atoms[i].atom_type,
                          mol.atoms[i].x, mol.atoms[i].y, mol.atoms[i].z,
                          mol.atoms[i].num_neighbours),
               20, 40 + i * 10, 8, DARKGRAY);

      // Convenient place to reset the bond drawn flags
      mol.atoms[i].bonds_drawn = false;
    }

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------

  // TODO: Unload all loaded data (textures, fonts, audio) here!

  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return EXIT_SUCCESS;
}

void drawAtom(Atom atom) {
  Color color;
  float radius;

  if (strcmp(atom.atom_type, "C") == 0) {
    color = BLACK;
    radius = 70.0f / 100;
  } else if (strcmp(atom.atom_type, "O") == 0) {
    color = RED;
    radius = 60.0f / 100;
  } else if (strcmp(atom.atom_type, "H") == 0) {
    color = LIGHTGRAY;
    radius = 25.0f / 100;
  } else if (strcmp(atom.atom_type, "S") == 0) {
    color = YELLOW;
    radius = 100.0f / 100;
  } else {
    color = GRAY;
    radius = 25.0f / 100;
  }

  DrawSphereWires((Vector3){atom.x, atom.y, atom.z}, radius, 25, 25, color);
}
