#include "debug.h"
#include "elementdata.c"
#include "raylib.h"
#include "sdfparse.c"
#include <complex.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------
void drawAtom(Atom atom, Model sphereModel);
void drawBond(Atom atom);
static Mesh GenMeshCustom(void);

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

  int element_count;
  Element *elements = parseCSV("resources/periodictable.csv", &element_count);

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
  // camera.projection = CAMERA_ORTHOGRAPHIC;             // Camera projection
  // type
  camera.projection = CAMERA_PERSPECTIVE; // Camera projection type
  int cameraMode = CAMERA_THIRD_PERSON;

  // Model sphereModel = LoadModelFromMesh(GenMeshCustom());  
  Model sphereModel = LoadModelFromMesh(GenMeshSphere(1.0f, 25, 25));


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
      drawAtom(mol.atoms[i], sphereModel);
      drawBond(mol.atoms[i]);
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

  free(elements);
  UnloadModel(sphereModel);

  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return EXIT_SUCCESS;
}

void drawBond(Atom atom) {
  Color bondColor = GREEN;
  for (int j = 0; j < atom.num_neighbours; j++) {
    if (!atom.bonds_drawn) {
      if (atom.bond_orders[j] == 2) {
        bondColor = BLUE;
      } else if (atom.bond_orders[j] >= 3) {
        bondColor = RED;
      }

      DrawCylinderWiresEx((Vector3){atom.x, atom.y, atom.z},
                          (Vector3){atom.neighbours[j]->x,
                                    atom.neighbours[j]->y,
                                    atom.neighbours[j]->z},
                          0.1f, 0.1f, 20, bondColor);
    }
    atom.bonds_drawn = true;
  }
}

void drawAtom(Atom atom, Model sphereModel) {
  Color color;
  float radius;
  int scalingFactor = 100;

  // radius is empirical from
  // https://en.wikipedia.org/wiki/Atomic_radii_of_the_elements_(data_page)
  if (strcmp(atom.atom_type, "C") == 0) {
    color = BLACK;
    radius = 70.0f / scalingFactor;
  } else if (strcmp(atom.atom_type, "O") == 0) {
    color = RED;
    radius = 60.0f / scalingFactor;
  } else if (strcmp(atom.atom_type, "H") == 0) {
    color = LIGHTGRAY;
    radius = 25.0f / scalingFactor;
  } else if (strcmp(atom.atom_type, "S") == 0) {
    color = YELLOW;
    radius = 100.0f / scalingFactor;
  } else if (strcmp(atom.atom_type, "N") == 0) {
    color = BLUE;
    radius = 65.0f / scalingFactor;
  } else {
    color = GRAY;
    radius = 25.0f / scalingFactor;
  }

  DrawModelWires(sphereModel, (Vector3){atom.x, atom.y, atom.z}, radius, color);

}

// Generate a simple triangle mesh from code
static Mesh GenMeshCustom(void)
{
    Mesh mesh = { 0 };
    mesh.triangleCount = 1;
    mesh.vertexCount = mesh.triangleCount*3;
    mesh.vertices = (float *)MemAlloc(mesh.vertexCount*3*sizeof(float));    // 3 vertices, 3 coordinates each (x, y, z)
    mesh.texcoords = (float *)MemAlloc(mesh.vertexCount*2*sizeof(float));   // 3 vertices, 2 coordinates each (x, y)
    mesh.normals = (float *)MemAlloc(mesh.vertexCount*3*sizeof(float));     // 3 vertices, 3 coordinates each (x, y, z)

    // Vertex at (0, 0, 0)
    mesh.vertices[0] = 0;
    mesh.vertices[1] = 0;
    mesh.vertices[2] = 0;
    mesh.normals[0] = 0;
    mesh.normals[1] = 1;
    mesh.normals[2] = 0;
    mesh.texcoords[0] = 0;
    mesh.texcoords[1] = 0;

    // Vertex at (1, 0, 2)
    mesh.vertices[3] = 1;
    mesh.vertices[4] = 0;
    mesh.vertices[5] = 2;
    mesh.normals[3] = 0;
    mesh.normals[4] = 1;
    mesh.normals[5] = 0;
    mesh.texcoords[2] = 0.5f;
    mesh.texcoords[3] = 1.0f;

    // Vertex at (2, 0, 0)
    mesh.vertices[6] = 2;
    mesh.vertices[7] = 0;
    mesh.vertices[8] = 0;
    mesh.normals[6] = 0;
    mesh.normals[7] = 1;
    mesh.normals[8] = 0;
    mesh.texcoords[4] = 1;
    mesh.texcoords[5] =0;

    // Upload mesh data from CPU (RAM) to GPU (VRAM) memory
    UploadMesh(&mesh, false);

    return mesh;
}
