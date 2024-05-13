#include "raylib.h"
#include "sdfparse.c"

// #define DEBUG 1

#ifdef DEBUG
#  define D
#else
#  define D for(;0;)
#endif

//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------
void drawAtom(Atom atom);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1200;
    const int screenHeight = 800;

    // Molecule ethanol = parseSDF("./resources/ethanol.sdf");
    Molecule ethanol = parseSDF("./resources/methyl-vinyl-ketone.sdf");
    normalizeCoordinates(&ethanol, screenWidth, screenHeight);

    D printf("Number of atoms: %d\n", ethanol.num_atoms);
    for (int i = 0; i < ethanol.num_atoms; i++) {
        D printf("Atom %d: %s (%.4f, %.4f, %.4f)\n", i+1, ethanol.atoms[i].atom_type, ethanol.atoms[i].x, ethanol.atoms[i].y, ethanol.atoms[i].z);
    }

    InitWindow(screenWidth, screenHeight, "LOL dongs");

    SetTargetFPS(60);               // Set desired framerate (frames-per-second)
    
    
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Set camera position
    camera.target = (Vector3){ 5.0f, 5.0f, 5.0f };   // Set camera target
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };       // Set camera up vector
    camera.fovy = 45.0f;                             // Set camera field of view
    // camera.projection = CAMERA_ORTHOGRAPHIC;             // Camera projection type
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type
                                                        //
    int cameraMode = CAMERA_FIRST_PERSON;

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyPressed(KEY_ONE))
        {
            cameraMode = CAMERA_FREE;
            camera.up = (Vector3){ 0.0f, 1.0f, 0.0f }; // Reset roll
        }

        if (IsKeyPressed(KEY_TWO))
        {
            cameraMode = CAMERA_FIRST_PERSON;
            camera.up = (Vector3){ 0.0f, 1.0f, 0.0f }; // Reset roll
        }

        if (IsKeyPressed(KEY_THREE))
        {
            cameraMode = CAMERA_THIRD_PERSON;
            camera.up = (Vector3){ 0.0f, 1.0f, 0.0f }; // Reset roll
        }

        if (IsKeyPressed(KEY_FOUR))
        {
            cameraMode = CAMERA_ORBITAL;
            camera.up = (Vector3){ 0.0f, 1.0f, 0.0f }; // Reset roll
        }
        
        if (IsKeyPressed('Z')) camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
        
        UpdateCamera(&camera, cameraMode);
        // UpdateCamera(&camera, CAMERA_ORBITAL);


        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                // Draw atoms
                for (int i = 0; i < ethanol.num_atoms; i++) {
                    drawAtom(ethanol.atoms[i]);
                    
                    Color bondColor = GREEN;
                    if (ethanol.atoms[i].bond_order != 1) { 
                      bondColor = BLUE;
                    }
                    D printf("binding %d\n", ethanol.atoms[i].bond_order);

                    if (ethanol.atoms[i].neighbour1 != NULL) {
                        DrawCylinderWiresEx(
                           (Vector3){ ethanol.atoms[i].x, ethanol.atoms[i].y, ethanol.atoms[i].z },
                           (Vector3){ ethanol.atoms[i].neighbour1->x, ethanol.atoms[i].neighbour1->y, ethanol.atoms[i].neighbour1->z },
                           0.1f, 0.1f, 20, bondColor
                        );
                    }
                    if (ethanol.atoms[i].neighbour2 != NULL) {
                        DrawCylinderWiresEx(
                           (Vector3){ ethanol.atoms[i].x, ethanol.atoms[i].y, ethanol.atoms[i].z },
                           (Vector3){ ethanol.atoms[i].neighbour2->x, ethanol.atoms[i].neighbour2->y, ethanol.atoms[i].neighbour2->z },
                           0.1f, 0.1f, 20, bondColor
                        );
                    }
                    if (ethanol.atoms[i].neighbour3 != NULL) {
                        DrawCylinderWiresEx(
                           (Vector3){ ethanol.atoms[i].x, ethanol.atoms[i].y, ethanol.atoms[i].z },
                           (Vector3){ ethanol.atoms[i].neighbour3->x, ethanol.atoms[i].neighbour3->y, ethanol.atoms[i].neighbour3->z },
                           0.1f, 0.1f, 20, bondColor
                        );
                    }
                    if (ethanol.atoms[i].neighbour4 != NULL) {
                        DrawCylinderWiresEx(
                           (Vector3){ ethanol.atoms[i].x, ethanol.atoms[i].y, ethanol.atoms[i].z },
                           (Vector3){ ethanol.atoms[i].neighbour4->x, ethanol.atoms[i].neighbour4->y, ethanol.atoms[i].neighbour4->z },
                           0.1f, 0.1f, 20, bondColor
                        );
                    }
                }

            EndMode3D();

            // Print atom coordinates
            DrawText("Atom Coordinates:", 20, 20, 20, DARKGRAY);
            for (int i = 0; i < ethanol.num_atoms; i++) {
                DrawText(TextFormat("Atom %d: %s (%.4f, %.4f, %.4f)", ethanol.atoms[i].idx, ethanol.atoms[i].atom_type, ethanol.atoms[i].x, ethanol.atoms[i].y, ethanol.atoms[i].z), 20, 40 + i * 10, 8, DARKGRAY);
            }


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    // TODO: Unload all loaded data (textures, fonts, audio) here!

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void drawAtom(Atom atom)
{
    Color color;
    float radius;

    // Assign color based on atom type
    if (strcmp(atom.atom_type, "C") == 0) {
        color = BLACK;
        radius = 70.0f/100;
    } else if (strcmp(atom.atom_type, "O") == 0) {
        color = RED;
        radius = 60.0f/100;
    } else if (strcmp(atom.atom_type, "H") == 0) {
        color = LIGHTGRAY;
        radius = 25.0f/100;
    } else {
        // Default color if atom type is unknown
        color = GRAY;
        radius = 25.0f/100;
    }

    // DrawSphere((Vector3){atom.x, atom.y, atom.z}, radius, color);
    DrawSphereWires((Vector3){atom.x, atom.y, atom.z}, radius, 25, 25, color);
} 
